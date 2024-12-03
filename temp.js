
function requestNewImage(){
    var req = new XMLHttpRequest();
    req.onreadystatechange = function () {
        if(req.readyState === 4){
            alert(req.response)
        }
    }
    req.open('get', 'http://127.0.0.1:8200/getNewImage', true);
    req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8');
    req.send();
}
