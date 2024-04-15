# Ejemplo 33: Eventos de la conexion wifi en ESP-IDF

## Introduccion

En el ejemplo, nos conectaremos a una red _wifi_ y veremos los _wifi events_.

Ademas, empezaremos a modularizar el codigo con funciones para poder reutilizarlo en ejemplos posteriores:

- _nvs_init_ inicializa el _NVS_.
- _wifi_station_init_ inicializa y hace la conexion a la red _wifi_.
- _event_handler_ recibe y atiende los _wifi events_.
- _print_station_disconnected_reason_ imprime la causa por la que fallo la conexion a la red _wifi_.

## Driver wifi en ESP-IDF

![Modelo](/ej33-wifi-events/modelo.png "Modelo")

Al _driver wifi_ se lo puede considerar como una caja negra.

En el modelo, la _Aplication Task_ es nuestro codigo:

- Que va a llamar al a _api wifi_ para inicializar el _wifi_.
- Que va a poder recibir los _wifi events_ utlilizando la biblioteca _esp_event_.

Los _wifi events_ se envian al _default event loop_, que es creado llamando a la funcion _esp_event_loop_create_default_.

Luego, nuestra aplicacion podra tener uno o mas _handlers_ que funcionaran como _event callbacks_.

A un _wifi events_ se le registrara un _handler_ llamando a la funcion _esp_event_handler_register_.

Ademas, los _wifi events_ son utilizados por la biblioteca _esp_netif_ para automatizar tareas sobre el _stack TCP/IP_.

Por todo lo dicho, es necesaria la inicializacion del _stack TCP/IP_ vista anteriormente.

## Registrar un handler a los wifi events en modo station en ESP-IDF

Primero, lo haremos para los eventos producidos por el _driver wifi_:

1. Llamar a la funcion _esp_event_handler_instance_register_.
2. Pasarle como primer parametro _WIFI_EVENT_.
3. Pasarle como segundo parametro _ESP_EVENT_ANY_ID_.
4. Pasarle como tercer parametro un puntero al _handler_. En este caso debe valer _&event_handler_.
5. Pasarle como cuarto parametro _NULL_.
6. Pasarle como quinto parametro _NULL_.
7. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

Segundo, lo haremos para solo para el evento que se produce cuando el ESP32 toma una IP del _access point_ producidos por el _stack TCP/IP_:

1. Llamar a la funcion _esp_event_handler_instance_register_.
2. Pasarle como primer parametro _IP_EVENT_.
3. Pasarle como segundo parametro _IP_EVENT_STA_GOT_IP_.
4. Pasarle como tercer parametro un puntero al _handler_. En este caso debe valer _&event_handler_.
5. Pasarle como cuarto parametro _NULL_.
6. Pasarle como quinto parametro _NULL_.
7. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Definicion del handler para los wifi events en modo station en ESP-IDF

1. Crear una funcion llamada _event_handler_ que:
   - Devuelve _void_.
   - El primer argumento es _void \*arg_.
   - El segundo argumento es _esp_event_base_t event_base_.
   - El tercer argumento es _int32_t event_id_.
   - El cuarto argumento es _void \*event_data_.
2. Crear un swith utilizando _event_id_.

El evento _WIFI_EVENT_STA_START_ se produce despues de llamar a la funcion _esp_wifi_start_ y que la misma retorne _ESP_OK_. Se utiliza comunmente, para llamar a la funcion _esp_wifi_connect_ y que el _ESP32_ intente conectarse al _access point_ por primera vez.

El evento _WIFI_EVENT_STA_DISCONNECTED_ se puede producir en los siguientes casos:

- Cuando se llama a las funciones _esp_wifi_disconnect_ o _esp_wifi_stop_ y el _ESP32_ ya estaba conectado al _access point_.
- Cuando se llama a la funcion _esp_wifi_connect_ pero falla la conexion.
- Cuando se interrumpe la conexion previamente establecida al _access point_ por el motivo que fuese.

Se utiliza comunmente, para que el _handler_ vuelva a llamar a _esp_wifi_connect_ un numero maximo de veces para intentar reconectarse. Y ademas, para mostrar la causa de la desconexion a la red _wifi_. Entre intentos de conexion es conveniente agregar un intervalo de tiempo de 5 segundos llamando a la funcion _vTaskDelay_.

El evento _IP_EVENT_STA_GOT_IP_ se produce cuando el _ESP32_ obtiene una _IP_ valida del _dhcp server_ del _access point_. Se utiliza comunmente, para mostrar la _IP_ que tomo el _SOC_ por el _ESP-IDF: Monitor Device_ y reiniciar el contador de intentos de reconexion a _0_.

## Cargar ssid y password para realizar la conexion a la red wifi en modo station en ESP-IDF

1. Crear una constante llamada _ssid_ del tipo _char\*_ donde este almacenado el _ssid_ de la red _wifi_.
2. Crear una constante llamada _pwd_ del tipo _char\*_ donde este almacenado el _pasword_ de la red _wifi_.
3. Crear una variable llamada _wifi_config_ del tipo _wifi_config_t_ e inicializarla en _{}_.
4. Llamar a la funcion _strncpy_.
5. El primer parametro debe ser _(char \*)wifi_config.sta.ssid_.
6. El segundo parametro debe ser _ssid_.
7. El tercer parametro debe ser _sizeof(wifi_config.sta.ssid) - 1_.
8. Llamar a la funcion _strncpy_.
9. El primer parametro debe ser _(char \*)wifi_config.sta.password_.
10. El segundo parametro debe ser _pwd_.
11. El tercer parametro debe ser _sizeof(wifi_config.sta.password) - 1_.
12. Llamar a la funcion _esp_wifi_set_config_.
13. Pasarle como primer parametro _WIFI_IF_STA_.
14. Pasarle como segundo parametro un puntero a _wifi_config_.
15. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: Es importante el orden en que se realiza la inicializacion. Ver el codigo para seguirlo con mas claridad.

## Notificar a la tarea de una conexion exitosa o fallida a la red wifi en modo station desde el handler utilizando un event group de FreeRTOS

1. Crear un _EventGroupHandle_t_ llamado _bits_wifi_event_group_.
2. Crear dos constantes del tipo _EventBits_t_ llamadas _wifi_connected_bit_ y _wifi_fail_bit_.
3. Inicializar _wifi_connected_bit_ en _BIT0_.
4. Inicializar _wifi_fail_bit_ en _BIT1_.
5. Llamar al a funcion _xEventGroupCreate_
6. Guardar el valor retornado en _bits_wifi_event_group_.
7. Crear una variable del tipo _EventBits_t_ llamada _bits_ dentro de _wifi_station_init_.
8. Llamar a la funcion _xEventGroupWaitBits_.
9. Pasarle como primer parametro _bits_wifi_event_group_.
10. Pasarle como segundo parametro _wifi_connected_bit | wifi_fail_bit_.
11. Pasarle como tercer parametro _pdTRUE_.
12. Pasarle como cuarto parametro _pdFALSE_.
13. Pasarle como quinto parametro _portMAX_DELAY_.
14. Guardar el valor retornado en _bits_

Si el valor en _bits_ es igual a _wifi_connected_bit_ informar por el _ESP-IDF: Monitor Device_ que la conexion fue exitosa. En cambio, si el valor en _bits_ es igual a _wifi_fail_bit_ informar por el _ESP-IDF: Monitor Device_ que la conexion fallo.

En el _WIFI_EVENT_STA_DISCONNECTED_ dentro del handler, si se supera la maxima cantidad de reconexiones posibles:

1. Llamar a la funcion _xEventGroupSetBits_.
2. Pasarle como primer parametro _bits_wifi_event_group_.
3. Pasarle como segundo parametro _wifi_fail_bit_.

En el _IP_EVENT_STA_GOT_IP_ dentro del handler, si se supera la maxima cantidad de reconexiones posibles:

1. Llamar a la funcion _xEventGroupSetBits_.
2. Pasarle como primer parametro _bits_wifi_event_group_.
3. Pasarle como segundo parametro _wifi_connected_bit_.
