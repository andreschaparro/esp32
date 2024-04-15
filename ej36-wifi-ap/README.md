# Ejemplo 36: Crear una red wifi con el ESP32 donde otros dispositivos podran conectarse en ESP-IDF

## Introduccion

En el ejemplo, crearemos una red wifi con el _ESP32_. A la cual nos podremos conectar utilizando nuestro celular. Ese modo de funcionamiento se llama _access point_.

Como veremos mas adelante, se puede diseñar una pagina web de configuracion para el _ESP32_. Y este modo, sera util para acceder a la misma antes de cargar los datos de _ssid_ y _password_ de la red _wifi_ a la que nos conectaremos.

No sw pueden conectar mas de 4 dispositivos a la vez al _ESP32_.

Bibliotecas a incluir:

- **esp_mac.h**

## Inicializar el stack TCP/IP en modo access point en ESP-IDF

1. Llamar a la funcion _esp_netif_init_.
2. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
3. Llamar a la funcion _esp_event_loop_create_default_.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
5. Crear una variable llamada _ap_netif_ del tipo _esp_netif_t\*_.
6. Llamar a la funcion _esp_netif_create_default_wifi_ap_.
7. Guardar el valor retornado en _ap_netif_.
8. Llamar a la macro _assert_.
9. Pasarle como parametro _ap_netif_.

## Configurar el modulo wifi en modo access point en ESP-IDF

El modo _station_ se usa cuando conectamos nuestro _ESP32_ a una red _wifi_.

1. Crear una variable llamada _cfg_ del tipo _wifi_init_config_t_.
2. Llamar a la macro _WIFI_INIT_CONFIG_DEFAULT_.
3. Guardar el valor retornado en _cfg_.
4. Llamar la funcion _esp_wifi_init_.
5. Pasarle como parametro un punteo a _cfg_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
7. Llamar a la funcion _esp_wifi_set_mode_.
8. Pasarle como parametro _WIFI_MODE_AP_.
9. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
10. Llamar a la funcion _esp_wifi_start_.
11. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Registrar un handler a los wifi events en modo access point en ESP-IDF

1. Llamar a la funcion _esp_event_handler_instance_register_.
2. Pasarle como primer parametro _WIFI_EVENT_.
3. Pasarle como segundo parametro _ESP_EVENT_ANY_ID_.
4. Pasarle como tercer parametro un puntero al _handler_. En este caso debe valer _&wifi_event_handler_.
5. Pasarle como cuarto parametro _NULL_.
6. Pasarle como quinto parametro _NULL_.
7. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Definicion del handler para los wifi events en modo access point en ESP-IDF

1. Crear una funcion llamada _wifi_event_handler_ que:
   - Devuelve _void_.
   - El primer argumento es _void \*arg_.
   - El segundo argumento es _esp_event_base_t event_base_.
   - El tercer argumento es _int32_t event_id_.
   - El cuarto argumento es _void \*event_data_.
2. Crear un swith utilizando _event_id_.

El evento _WIFI_EVENT_AP_STACONNECTED_ se produce cada vez que un dispositivo se conecta. Se puede utilizar para llevar un registro de las _mac address_ y la cantidad de dispositivos conectados.

El evento _WIFI_EVENT_AP_STADISCONNECTED_ se puede producir en los siguientes casos:

- Cuando se llama a las funciones _esp_wifi_disconnect_ o _esp_wifi_deauth_sta_ para forzar la desconexion de un dispositivo.
- Si tras 5 minutos un dispositivo no envia paquetes. Este tiempo, se puede modificar llamando a la funcion _esp_wifi_set_inactive_time_.
- Cuando la estacion deshabilita el modo _access point_.

Se puede utilizar para llevar un registro de las _mac address_ y la cantidad de dispositivos conectados.

## Cargar ssid y password para poder conectarse a la red wifi en modo access point en ESP-IDF

1. Crear una constante llamada _ap_ssid_ del tipo _char\*_ donde este almacenado el _ssid_ del _ESP32_.
2. Crear una constante llamada _ap_pwd_ del tipo _char\*_ donde este almacenado el _pasword_ del _ESP32_.
3. Crear una variable llamada _wifi_config_ del tipo _wifi_config_t_ e inicializarla en _{}_.
4. Llamar a la funcion _strncpy_.
5. El primer parametro debe ser _(char \*)wifi_config.ap.ssid_.
6. El segundo parametro debe ser _ap_ssid_.
7. El tercer parametro debe ser _sizeof(wifi_config.ap.ssid) - 1_.
8. Llamar a la funcion _strncpy_.
9. El primer parametro debe ser _(char \*)wifi_config.ap.password_.
10. El segundo parametro debe ser _ap_pwd_.
11. El tercer parametro debe ser _sizeof(wifi_config.ap.password) - 1_.
12. _wifi_config.ap.channel_ debe valer _1_.
13. _wifi_config.ap.max_connection_ debe valer _4_.
14. _wifi_config.ap.authmode_ debe valer _WIFI_AUTH_WPA2_PSK_.
15. Llamar a la funcion _esp_wifi_set_config_.
16. Pasarle como primer parametro _WIFI_IF_AP_.
17. Pasarle como segundo parametro un puntero a _wifi_config_.
18. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: Es importante el orden en que se realiza la inicializacion. Ver el codigo para seguirlo con mas claridad.
