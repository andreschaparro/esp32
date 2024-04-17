# Ejemplo 40: Sitio web montado sobre SPIFFS en ESP-IDF

## Introduccion

Un sitio web es conformado por muchos arhivos y carpetas. Lo minimo que podemos tener es un archivo llamado _index.html_ que sera nuestra pagina _web_ de inicio.

El lenguaje _HTML_ es un lenguaje de marcado o _tags_. Actualmente, se trabaja con la version _HTML 5_.

Es el corazon del diseño de las paginas _web_, dado que permite hacer el _maquetado_ del la misma, lo que es lo mismo que decir: que tiene, donde, y como esta ordenado.

En el ejemplo, crearemos un sitio web modificando el _root_get_handler_ que habiamos hecho en ejemplos anteriores. El archivo _index.html_ estara alojado en el _SPIFFS_ y se lo devolveremos al navegador _web_ como un recurso cuando nos haga una peticion con _GET_.

Para mas informacion sobre _HTML_ consultar la siguiente pagina: https://www.w3schools.com/html/

## Hacer la peticion con POST y esperar la respuesta del servidor utilizando AJAX

Dentro del codigo _HTML_ hay un _script_ de _Javascript_. El mismo se utiliza poder realizar la peticion indicando que se utilize el metodo _POST_. Y se aclara, que dentro del mensaje, se debe enviar un objeto _jSON_ que tiene un campo llamado _is_on_ que puede valer _true_ o _false_.

El _script_ utiliza un tipo especial de peticion que se llama _AJAX_ y permite:

- Indicar el metodo a utilizar _GET_ o _POST_.
- Interactuar con la pagina _web_ sin que la misma se congele hasta que se resuelve la peticion. Es decir, la peticion se vuelve _asincronica_.
- Que no sea necesario recargar la pagina si se desea actualizar el contenido de la misma. Puede hacerse, al recibir la respuesta de la peticion _AJAX_.
- Transferir distintos tipo de objetos en el _body_ del mensaje, como por ejemplo: _jSON_ o texto sin formato.

A continuacion, se muestra graficamente como se resuelve una peticion _AJAX_:

![AJAX](/ej40-website-spiffs/ajax.png "AJAX")

Para mas informacion consultar en:

- https://www.w3schools.com/js/js_es5.asp
- https://www.w3schools.com/xml/ajax_intro.asp

## Conclusiones

Este es un ejemplo integrador porque:

- Creamos un nuevo modulo llamado _api_spiffs_ para inicializar el file system _SPIFFS_.
- Utilizamos el _SPIFFS_ para subir el archivo _index.html_ que es nuestro sitio _web_.
- Utilizamos la _api_wifi_station_ para conectarnos a una red _wifi_.
- utilizamos el modulo _mDNS_ de _Espressif_ para acceder a nuestro _ESP32_ sin saber su direccion _IP_.
- Creamos un _webserver_ que:
  - Responde a una peticion hecha con _GET_ en el recurso _/_, devolviendo el archivo _index.html_ para crear la pagina _web_ en el navegador.
  - Responde a una peticion hecha con _POST_ en el recurso _/api/led_ encendiendo o apagando un _led_ segun el valor del campo _is_on_ del objeto _jSON_ enviado en el _body_ del mensaje.
- Utilizamos la pagina _web_ para generar la peticion de _POST_ en lugar de utilizar _Postman_ mediante _AJAX_.
