function cambiarLed(estado) {
    var xhttp = new XMLHttpRequest();
    var url = 'http://esp32-id1/api/led';
    var data = {
        is_on: estado,
    };
    var jsonData = JSON.stringify(data);
    xhttp.open('POST', url, true);
    xhttp.setRequestHeader('Content-Type', 'application/json');
    xhttp.onreadystatechange = function () {
        if (xhttp.readyState === XMLHttpRequest.DONE) {
            if (xhttp.status === 200) {
                console.log(xhttp.responseText);
            } else {
                console.error('Hubo un problema con la solicitud.');
            }
        }
    };
    xhttp.send(jsonData);
};