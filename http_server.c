#include <stdio.h> //input/output. printing/reading here
#include <stdlib.h> //needed for exit functions
#include <string.h> //basic string manipulation functionality
#include <arpa/inet.h> //addresses library
#include <unistd.h> //system calls, close(), read() (from files)>
#include <sys/socket.h> //socket creation and usage
#include <sys/types.h> // fork() is here

#define PORT 8200
#define MAXIMUM_CONNECTIONS 3
//create socket
//bind socket to address and port via bind()
//listen() on that port
//accept() incoming clients

int main(){

	int server_socket_descriptor, new_client_socket;
	struct sockaddr_in server_address;
	int address_length = sizeof(server_address);
	//Creating a new tcp socket:
	//(sock_dgram would be UDP)

	//domain, type, protocol. AF_INET is for IPv4. AF_INET6 is for Ipv6,
	//protocol is 0 which refers to Internet Protocol
	server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(server_socket_descriptor == -1){
		perror("Failed to create socket.");
		exit(EXIT_FAILURE);
	}
	
	server_address.sin_family = AF_INET;//so we know what the address looks like (what family is it in)
	server_address.sin_addr.s_addr = INADDR_ANY; //bind socket to all available interfaces.
	server_address.sin_port = htons(PORT); //convert port number to network order (endianness)
	//networking requires that it be in big endian?

	//binding with error check, will do the same for the listen()

	if(bind(server_socket_descriptor,(struct sockaddr *)&server_address, sizeof(server_address)) < 0){
		perror("Bind failed.");
		exit(EXIT_FAILURE);
	}

	if(listen(server_socket_descriptor, MAXIMUM_CONNECTIONS) < 0){
		perror("Listen failed.");
		exit(EXIT_FAILURE);
	}

	printf("Server operating and listening on port %d. \n", PORT);

	new_client_socket = accept(server_socket_descriptor, (struct sockaddr *)&server_address, (socklen_t*)&address_length);
	if(new_client_socket < 0){
	perror("Server accept failed.");
	exit(EXIT_FAILURE);
	}

	if(fork() == 0){
		close(server_socket_descriptor); //closing the server socket in the child process, not needed in parent process
		char buffer[1024] = {0}; //need a buffer, not dissimilar from the other thing
		int valread = read(new_client_socket, buffer, 1024);
		if(valread < 0){
			perror("Failed to read from client.");
			exit(EXIT_FAILURE);
			}
	printf("Request received:\n%s\n", buffer);

	//response code
	//probably clear buffer here and take an SQL request

	const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h2>Welcome to the Ark.</h2> <button onclick=\"requestNewImage()\"> Request Image </button>";
	send(new_client_socket, response, strlen(response), 0);
	printf("Response has been sent.\n");
	}
	else{

		close(new_client_socket);
		exit(0); //close child process here.
	}


return 0;

}
