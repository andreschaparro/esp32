# Ejemplo 37: Webserver basico en ESP-IDF

## Introduccion

Un _webserver_ es un _servidor_ que utiliza el protocolo _HTTP_. Dicho protocolo, sigue el patron _cliente-servidor_.

El _servidor_ gestiona y responde peticiones.

Las peticiones son enviadas por una _entidad_ llamada _agente del usuario_, que la mayoria de las veces es un navegador _web_. Siempre es el navegador quien inicia una comunicacion y nunca el servidor.

Por ello, siempre que deseamos ver una pagina _web_, primero debemos ingresar su _uri_ en la barra de direcciones. Luego, el _servidor_ respondera con todos los recursos necesarios para mostrar la pagina _web_. Y finalmente, el navegador _web_ tomara esa respuesta y reconstruira la pagina _web_ con los recursos devueltos.

El protocolo _HTTP_ se apoya sobre el protocolo _TCP_ dentro del modelo _OSI_, porque es mas fiable que el _UDP_. Esto es asi, porque el protocolo _TCP_ permite detectar mensajes perdidos y errores. Es asi, por lo que se dice que el protocolo _TCP_ es _"orientado a la conexion"_.

Ahora, y con un poco mas de detalles, el flujo del protocolo _HTTP_ del cliente es el siguiente:

1. Abrir una conexion _TCP_ para hacer una peticion y recibir una respuesta. Y para ello, puede hacer una conexion nueva, reutilizar una existente, o abrir varias a la vez.
2. Hacer la peticion _HTTP_. Donde la peticion es un mensaje legible en texto plano.
3. Leer la respuesta del servidor. Donde la respuesta es otro tipo de mensaje.
4. Cerrar o reutilizar la conexion _TCP_ para peticiones futuras.

Un mensaje de peticion _HTTP_ tiene el siguiente formato:

- _Metodo_: Por ejemplo _GET_ y _POST_. _GET_ se utiliza cuando solicitamos un recurso y _POST_ cuando enviamos datos de un formulario. Hay otros metodos.
- _Path_: Direccion del recurso pedido, o en otras palabras, su _uri_. Pero sin los elementos: _http://_, _dominio_, y puerto en caso de que no fuera el _80_.
- Version del protocolo.
- Cabeceras _HTTP_ con informacion adicional para el _servidor_.
- Cuerpo del mensaje, como pueden ser los datos de un formulario al hacer un _POST_.

Un mensaje de respuesta _HTTP_ tiene el siguiente formato:

- Version del protocolo.
- Codigo de estado indicando si la peticion fue exitosa con un _200_ o no con un _404_. Hay mas codigos.
- Una descripcion del codigo de estado.
- Cabeceras _HTTP_ adicionales.
- Opcionalmente, el recurso que se ha pedido.

Hay paginas web que pueden estar protegidas con autentificacion. Este, es uno de los usos que se les da a la cabeceras _HTTP_.

En el ejemplo:

- Inicializaremos el _webserver_.
- Crearemos un recurso al que se accedera con el metodo _GET_.
- Crearemos la _api_wifi_ap_ para iniciar el _ESP32_ en modo _access point_ y poder acceder al recurso con un navegador _web_.

En modo _access point_ la direccion _IP_ del _ESP32_ siempre sera _192.168.4.1_. Y la misma servira como _uri_ base del siguiente modo: *http://192.168.4.1/recurso*.

Para leer mas sobre el protocolo _HTTP_: *https://developer.mozilla.org/es/docs/Web/HTTP/Overview*

Bibliotecas a incluir:

- **esp_http_server.h**

## Crear un handler que atienda la peticion de un recurso con el metodo GET del webserver en ESP-IDF

1. Crear una constante global llamada _root_uri_ del tipo _httpd_uri_t_:
   - El miembro _.uri_ debe valer _"/"_.
   - El miembro _.method_ debe valer _HTTP_GET_.
   - El miembro _.handler_ debe valer _root_get_handler_.
   - El miembro _.user_ctx_ debe valer _NULL_.
2. Crear una funcion llamada _root_get_handler_.
3. Debe devolver _esp_err_t_.
4. Debe recibir un parametro del tipo _req_ del tipo _httpd_req_t \*_.
5. Crear una constante llamada _resp_ del tipo _char \*_. Inicializarla con el contenido de la respuesta al navegador _web._ En este caso _"Hola Mundo desde el ESP32!"_.
6. Llamar a la funcion _httpd_resp_send_.
7. Pasarle como primer parametro _req_.
8. Pasarle como segundo parametro _resp_.
9. Pasarle como tercer parametro la macro _HTTPD_RESP_USE_STRLEN_.
10. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
11. Retornar el valor _ESP_OK_.

NOTA: En este caso, se devuelve un texto plano que se mostrara en el navegador web cuando el usuario coloque en la barra de direcciones *http://192.168.4.1*.

## Inicializar el webserver en ESP-IDF

1. Crear una variable global llamada _webserver_ del tipo _httpd_handle_t_.
2. Inicializar _webserver_ en _NULL_.
3. Crear una funcion llamada _webserver_init_.
4. Debe devolver _httpd_handle_t_.
5. Recibe como parametro _void_.
6. Crear una variable llamada _config_ del tipo _httpd_config_t_.
7. Inicializar _config_ con el valor retornado por la macro _HTTPD_DEFAULT_CONFIG_.
8. Crear una variable llamada _server_ del tipo _httpd_handle_t_.
9. Inicializar _server_ en _NULL_.
10. Llamara a la funcion _httpd_start_.
11. Pasarle como primer parametro un puntero a _server_.
12. Pasarle como segundo parametro un puntero a _config_.

Si el valor retornado es igual a _ESP_OK_ el _servidor_ podran registrarse los _handlers_ de los recursos. En caso contrario, no se pudo iniciar el _servidor_.

13. Retornar _server_.
14. Guardar _server_ en _webserver_.
15. Hacer un _assert_ de _webserver != NULL_.
16. Llamara la funcion _vTaskDelete_.
17. Pasarle como parametro _NULL_.

## Registrar un handler que atienda la peticion de un recurso con el metodo GET del webserver en ESP-IDF

1. Llamar a la funcion _httpd_register_uri_handler_.
2. Pasarle como primer parametro _server_.
3. Pasarle como segundo parametro un puntero a _root_uri_.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Detener el webserver en ESP-IDF

1. Llamar a la funcion _httpd_stop_.
2. Pasarle como parametro _webserver_.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

Esto, es util para liberar recursos en caso de que no sea necesario tener el _servidor_ funcionando.
