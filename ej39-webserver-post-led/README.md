# Ejemplo 39: Webserver que resuelve peticiones con el Metodo POST para Encender/Apagar un led en ESP-IDF

## Introduccion

En ejemplos anteriores, se utilizo el metodo _GET_ del protocolo _HTTP_ para hacer una peticion al _servidor_ a su recurso _"/"_, tambien llamado _root_ o _index_. Donde el mismo, nos devolvia un _string_ que se mostraba en el navegador _web_.

Ahora, le vamos a enviar una peticion al recurso _/api/led_ del _servidor_. El recurso estara asociado al _led_ en la _GPIO_NUM_2_.

Con la peticion, vendra un dato llamado _is_on_ que si vale _true_ le indicara al _ESP32_ que encienda el _led_, o si vale _false_ que lo apague.

Como la peticion contiene un dato, sera necesario utilizar el metodo _POST_ del protocolo _HTTP_.

El navegador _web_ envia la peticion con el metodo _GET_ cuando ingresamos una _uri_. Por lo que, para enviar una solicitud con el metodo _POST_ utilizaremos un programa llamado _Postman_: https://www.postman.com/downloads/

El dato llamado _is_on_ debera ir dentro del _cuerpo_ o _body_ de la peticion como ya se vio en ejemplos posteriores.

La estructura del _body_ puede armarse de diferentes formas. La forma mas comun, es utilizar el formato de serializacion _jSON_.

_jSON_ es un formato de datos basado en texto, que sigue una sintaxis familiar a los objetos de _JavaScript_:

    {
        "field1": "text",
        "field2": 42,
        "field3": true,
        "field4": [],
    }

Para mas informacion sobre el formato _jSON_ consultar la siguiente pagina: https://developer.mozilla.org/es/docs/Learn/JavaScript/Objects/JSON

Asi, la peticion que enviaremos desde _Postman_ para encender el led sera la siguiente:

![POST-ENCENDIDO](/ej39-webserver-post-led/post_encendido.png "POST-ENCENDIDO")

Mientras que para apagar el led sera:

![POST-APAGADO](/ej39-webserver-post-led/post_apagado.png "POST-APAGADO")

Bibliotecas a incluir:

- **sys/param.h**
- **cJSON.h**

## Crear un handler que atienda la peticion de un recurso con el metodo POST del webserver en ESP-IDF

1. Crear una constante global llamada _led_post_uri_ del tipo _httpd_uri_t_:
   - El miembro _.uri_ debe valer _"/api/led"_.
   - El miembro _.method_ debe valer _HTTP_POST_.
   - El miembro _.handler_ debe valer _led_post_handler_.
   - El miembro _.user_ctx_ debe valer _NULL_.
2. Crear una funcion llamada _led_post_handler_.
3. Debe devolver _esp_err_t_.
4. Crear un array de 100 elementos llamada _content_ del tipo _char_.
5. Llamar a la funcion _memset_.
6. Pasarle como primer parametro un puntero a _content_.
7. Pasarle como segundo parametro _'\0'_.
8. Pasarle como tercer parametro _sizeof(content)_.
9. Crear una variable llamada _recv_size_ del tipo _size_t_.
10. Llamar a la macro _MIN_.
11. Pasarle como primer parametro _req->content_len_.
12. Pasarle como segundo parametro _sizeof(content)_.
13. Guardar el valor retornado en _recv_size_.
14. Crear una variable llamada _ret_ del tipo _int_.
15. Llamar a la funcion _httpd_req_recv_.
16. Pasarle como primer parametro _req_.
17. Pasarle como segundo parametro _content_.
18. Pasarle como tercer parametro _recv_size_.
19. Guardar el valor retornado en _ret_.

En caso de que _ret_ sea menor o igual a _0_ devolver _ESP_FAIL_ y en el caso particular que valga _HTTPD_SOCK_ERR_TIMEOUT_ ,que es _-3_, llamar a _httpd_resp_send_408(req)_.

En cambio, si _ret_ es mayor a _0_ podemos proceder a deserializar el _content_ o _body_ que esta en formato _jSON_.

## Deserializar el body en formato jSON en ESP-IDF

1. Crear una variable llamada _payload_ del tipo _cJSON \*_.
2. Llamar a la funcion _cJSON_Parse_.
3. Pasarle como parametro _content_.
4. Guardar el valor retornado en _payload_.
5. Crear una variable llamada _is_on_json_ del tipo _cJSON \*_.
6. Llamar a la funcion _cJSON_GetObjectItem_.
7. Pasarle como primer parametro _payload_.
8. Pasarle como segundo parametro _"is_on"_.
9. Guardar el valor retornado en _*is_on_json*_.
10. Crear una variable llamada _is_on_ del tipo _bool_.
11. Llamar a la funcion _cJSON_IsTrue_.
12. Pasarle como paraemtro _is_on_json_.
13. Guardar el valor retornado en _is_on_.

Si _is_on_ es _true_ encendemos el _led_. Caso contrario, apagamos el _led_.

14. Llamar a la funcion _cJSON_Delete_.
15. Pasarle como parametro _payload_.
16. Crear una constante llamada _resp_ del tipo _char \*_ que debe valer _"URI POST Response"_.
17. Llamar a la funcion _httpd_resp_send_.
18. Pasarle como primer parametro _req_.
19. Pasarle como segundo parametro _resp_.
20. Pasarle como tercer parametro la macro _HTTPD_RESP_USE_STRLEN_.
21. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
22. Retornar el valor _ESP_OK_.
