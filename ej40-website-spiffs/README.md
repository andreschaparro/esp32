# Ejemplo 40: Sitio web montado sobre SPIFFS en ESP-IDF

## Introduccion

Un sitio web es conformado por muchos arhivos y carpetas. Lo minimo que podemos tener es un archivo llamado _index.html_ que sera nuestra pagina _web_ de inicio.

El lenguaje _HTML_ es un lenguaje de _etiquetas_. Actualmente, se trabaja con la version _HTML 5_.

Es el corazon del diseño de las paginas _web_, dado que permite definir su estructura o _maquetado_. Lo que es lo mismo que decir: que tiene, donde, y como esta ordenado.

En el ejemplo, crearemos un sitio web modificando el _root_get_handler_ que habiamos hecho en ejemplos anteriores. El archivo _index.html_ estara alojado en el _SPIFFS_ y se lo devolveremos al navegador _web_ como un recurso cuando nos haga una peticion con _GET_.

Para mas informacion sobre _HTML_ consultar la siguiente pagina: https://www.w3schools.com/html/

## Crear un handler que atienda la peticion de un recurso con el metodo GET para que el webserver devuelva el archivo index.html en ESP-IDF

Modificaremos el _handler_ llamado _root_get_handler_:

1. Crear una variable llamada _fd_ del tipo _FILE \*_.
2. Llamar a la funcion _fopen_.
3. Pasarle como primer parametro _"/spiffs/index.html"_.
4. Pararle como segundo parametro _"r"_.

Si _fd_ vale _NULL_:

5. Llamar a la funcion _httpd_resp_send_err_.
6. Pasarle como primer parametro _req_.
7. Pasarle como segundo parametro _500_INTERNAL_SERVER_ERROR_.
8. Pasarle como tercer parametro _"Error al leer el archivo"_.
9. Retornar _ESP_FAIL_.

En caso contrario:

10. Crear un array de 100 elementos llamado _buffer_ del tipo _char_.
11. Llamar a la funcion _memset_.
12. Pasarle como primer parametro un puntero a _buffer_.
13. Pasarle como segundo parametro _'\0'_.
14. Pasarle como tercer parametro _sizeof(buffer)_.
15. Crear una variable llamada _read_bytes_ del tipo _size_t_.
16. Crear un bucle _do while_ que se ejecute mientras _read_bytes_ sea mayor a _0_.

Ahora, dentro del bucle:

17. Llamar a la funcion _fread_.
18. Pasarle como primer parametro _buffer_.
19. Pasarle como segundo parametro _sizeof(char)_.
20. Pasarle como tercer parametro _sizeof(buffer)_.
21. Pasarle como cuarto parametro _fd_.
22. Guardar el valor retornado en _read_bytes_.

Si _read_bytes_ es igual a _-1_ hubo un error. En cambio, si es mayor a _0_ se pudo leer parte del archivo y se puede proceder a enviarlo:

23. Crear una variable llamada _ret_ del tipo _esp_err_t_.
24. Llamar a la funcion _httpd_resp_send_chunk_.
25. Pasarle como primer parametro _req_.
26. Pasarle como segundo parametro _buffer_.
27. Pasarle como tercer parametro _read_bytes_.
28. Guardar el valor retornado en _ret_.

Si _ret_ es distinto a _ESP_OK_:

29. Llamar a la funcion _fclose_.
30. Pasarle como parametro _fd_.
31. Hacer un log del error.
32. Llamar a la funcion _httpd_resp_send_err_.
33. Pasarle como primer parametro _req_.
34. Pasarle como segundo parametro _HTTPD_500_INTERNAL_SERVER_ERROR_.
35. Pasarle como tercer parametro _"Error al enviar el archivo"_.
36. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
37. Retornar _ESP_FAIL_.

Una vez que se termina de recorrer el archivo con el bucle _do while_:

38. Llamar a la funcion _fclose_.
39. Pasarle como parametro _fd_.
40. Llamar a la funcion _httpd_resp_send_chunk_.
41. Pasarle como primer parametro _req_.
42. Pasarle como segundo parametro _NULL_.
43. Pasarle como tercer parametro _0_.
44. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
45. Retornar _ESP_OK_.

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
- Utilizamos el _SPIFFS_ para subir el archivo _index.html_.
- Utilizamos la _api_wifi_station_ para conectarnos a una red _wifi_.
- Utilizamos el modulo _mDNS_ de _Espressif_ para acceder a nuestro _ESP32_ sin saber su direccion _IP_.
- Creamos un _webserver_ que:
  - Responde a una peticion hecha con _GET_ en el recurso _/_, devolviendo el archivo _index.html_ para crear la pagina _web_ en el navegador.
  - Responde a una peticion hecha con _POST_ en el recurso _/api/led_ encendiendo o apagando un _led_ segun el valor del campo _is_on_ del objeto _jSON_ enviado en el _body_ del mensaje.
- Utilizamos la pagina _web_ para generar la peticion de _POST_ en lugar de utilizar _Postman_ mediante _AJAX_.
