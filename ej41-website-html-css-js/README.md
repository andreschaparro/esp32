# Ejemplo 41: Sitio web con archivos HTML, CSS, y JS montado sobre SPIFFS en ESP-IDF

## Introduccion

Ahora, ademas tendremos un archivo llamado _scripts.js_ con las funciones de _Javascript_ y un archivo _theme.css_ con los estilos de la pagina _web_.

El modo de separar el codigo que genera una pagina _web_ para que este mas ordenado es:

- La estructura en un archivo _.html_.
- La presentacion en un archivo _.css_.
- Los _scripts_ en un archivo _.js_.

El lenguaje _CSS_ que significa hojas de estilo en cascada, permite cambiar la presentacion de los elementos de un documento _html_ utilizando:

- Un _selector_ para elegir el elmento:
  - _._ para seleccionar los elementos _html_ de una misma _clase_.
  - _#_ para seleccionar un elemento _html_ con un determinado _id_.
  - _etiqueta_ para seleccionar todos los elementos del mismo tipo.
- Propiedades y valores.

Por ejemplo:

    .ejemplo_clase{
        color: rgb(255,0,0);
    }
    #ejemplo_id{
        color: rgb(0,255,0);
    }
    h1{
        color: rgb(0,0,255);
    }

Para obtener mas informacion sobre _CSS_:

https://www.w3schools.com/css/

Para mas informacion sobre la estructura y la presentacion de una pagina _web_ se utliza el inspeccionador del navegador:

https://firefox-source-docs.mozilla.org/devtools-user/page_inspector/index.html

_Javascript_ es un lenguaje interpretado que se utiliza del lado del navegador _web_ para mejorar la interaccion del usuario, como ya se vio con _AJAX_. Hoy en dia, se utilizan dos versiones: _ES5_ y _ES6_.

Para obtener mas informacion sobre _Javascript_:

https://www.w3schools.com/js/default.asp

Lo que haremos en este ejemplo, sera:

- Poder leer diferentes tipos de archivos para responder peticiones _GET_ del navegador, devolviendolos en el formato correcto.
- Poder recuperar los archivos _scripts.js_ y _theme.css_ que utiliza _index.html_ para construir la pagina _web_ en el navegador.

## Responder a una peticion hecha con GET con archivos que no son HTML en ESP-IDF

Modificaremos el _handler_ llamado _root_get_handler_:

1. Crear un array de _600_ elementos llamado _path_ del tipo _char_.
2. Llamar a la funcion _memset_.
3. Pasarle como primer parametro un puntero a _path_.
4. Pasarle como segundo parametro _'\0'_.
5. Pasarle como tercer parametro _sizeof(path)_.
6. Llamar a la funcion _sprintf_.
7. Pasarle como primer parametro _path_.
8. Pasarle como segundo parametro _"/spiffs%s"_.
9. Pasarle como tercer parametro _req->uri_.

Con lo hecho hasta ahora, en la variable _path_ nos queda la ruta del archivo a leer, excepto para el caso de _/index.hml_ porque _req->uri_ valdra _/_.

10. Crear una variable llamada _ext_ del tipo _char \*_.
11. Llamar a la funcion _strrchr_.
12. Pasarle como primer parametro _req->uri_.
13. Pasarle como segundo parametro _'.'_.
14. Guardar el valor retornado en _ext_.

Con esto ultimo, _ext_ queda apuntando a la posicion dentro de _path_ donde se econtro la ultima ocurrencia del caracter _._, es decir, a la extension del archivo a leer.

Si la extension es _".html"_ debemos llamar a la funcion _httpd_resp_set_type(req, "text/html")_ para indicarle al navegador que le devolvemos un archivo _.html_.

Si la extension es _".js"_ debemos llamar a la funcion _httpd_resp_set_type(req, "application/javascript")_ para indicarle al navegador que le devolvemos un archivo _.js_.

Si la extension es _".css"_ debemos llamar a la funcion _httpd_resp_set_type(req, "text/css")_ para indicarle al navegador que le devolvemos un archivo _.css_.

Si la extension es _".png"_ debemos llamar a la funcion _httpd_resp_set_type(req, "image/png")_ para indicarle al navegador que le devolvemos una imagen con formato _.png_.

Si la extension es _".ico"_ debemos llamar a la funcion _httpd_resp_set_type(req, "image/x-icon")_ para indicarle al navegador que le devolvemos una imagen con formato _.ico_.

Si la extension es _".svg"_ debemos llamar a la funcion _httpd_resp_set_type(req, "text/xml")_ para indicarle al navegador que le devolvemos una imagen con formato _.svg_.

Cuando el navegador haga la peticion con _GET_ al recurso _/_ debemos devolverle el archivo _index.html_. Para lo cual, debemos modificar el valor de la variable _path_ llamando a la funcion _strcat(path, "index.html")_.

## Responder peticiones GET en cadena dentro de un mismo recurso del webserver en ESP-IDF

Una vez que el _servidor_ devuelva el archivo _index.html_, se crearan dos peticiones mas para solicitar los archivos _theme.css_ y _scripts.js_.

La peticion del archivo _theme.css_ intentara acceder al recurso _/theme.css_. Y de la misma forma, la peticion del archivo _scripts.js_ intentara acceder al recurso _/scripts.js_. Pero estos recursos no existen, y por lo tanto, aparecera un error _404_ en la consola por cada peticion.

No hay que confundir el hecho de que el archivo exista en el _SPIFFS_ con que exista como recurso del _webserver_ a traves de una _uri_.

La solucion es recurrir a una _wildcard_. La misma, nos permite que cada peticion que ingrese con una _uri_ que tenga el formato _"/algo"_ utilice el mismo _handler_ que _/_. De esta forma, las peticiones _/theme.css_ y _/scripts.js_ se resolveran con _root_get_handler_.

Para ello, debemos modificar la funcion _webserver_init_:

1. Antes de llamar a la funcion _httpd_start_, hacer que _config.uri_match_fn_ sea igual a _httpd_uri_match_wildcard_.
2. En _root_uri_ el miembro _.uri_ debe valer _"/\*"_.
