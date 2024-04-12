# Ejemplo 32: Scanner de redes wifi en ESP-IDF

## Introduccion

En el ejemplo, buscaremos las redes _wifi_ disponibles a las que podamos conectarnos y mostraremos la informacion de las primeras 10 que tengan mejor señal en el _ESP-IDF Monitor device_.

Cuando nos conectamos a una red _wifi_, decimos que el _ESP32_ opera en el modo de funcionamiento _station_.

En todas las aplicaciones donde se trabaje con la _api wifi_, sera necesario inicializar el _NVS_. Alli, se guardara la configuracion en caso de que se reinicie el _ESP32_.

Cuando se trabaja con la _api wifi_ se pueden retornar 4 tipos de valores:

- _ESP_OK_.
- Errores de los que podemos recuperarnos como _ESP_ERR_NO_MEM_.
- Errores de los que no podemos recuperarnos pero no son criticos.
- Errores de los que no podemos recuperarnos pero son criticos.

Que un error sea critico o no, depende de la _api_ y de la aplicacion que estemos programando.

**Por ello, siempre verificar si hubo errores y escribir los handlers correspondientes.**

- Para errores de los que nos podemos recuperar, podemos llamar a _vTaskDelay_ y esperar uno microsegundos para hacer otro intento.
- Para errores de los que no nos podemos recuperar y no son criticos, es bueno imprimir la informacion con la macro _ESP_LOGE_.
- Para errores de los que no nos podemos recuperar y son criticos, la macro _assert_ es una buena opcion.

Aunque la macro _ESP_ERR_CHECK_ es el _handler_ de error mas utilizado, es recomedable que con la _api wifi_ escribamos nuestros propios _handlers_.

Otro aspecto a tener en cuenta cuando se trabaja con la _api wifi_, es que para las variables, cuyo tipo deriva de un _struct_, hay macros de incicializacion definidas en la _api wifi_ que deben usarse siempre.

Mas adelante, se explicara como funciona el _driver wifi_ con mas detalle.

Bibliotecas a incluir:

- **esp_wifi**

## Inicializar el stack TCP/IP en modo station en ESP-IDF

1. Llamar a la funcion _esp_netif_init_.
2. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
3. Llamar a la funcion _esp_event_loop_create_default_.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
5. Crear una variable llamada _sta_netif_ del tipo _esp_netif_t\*_.
6. Llamar a la funcion _esp_netif_create_default_wifi_sta_.
7. Guardar el valor retornado en _sta_netif_.
8. Llamar a la macro _assert_.
9. Pasarle como parametro _sta_netif_.

## Configurar el modulo wifi en modo station en ESP-IDF

El modo _station_ se usa cuando conectamos nuestro _ESP32_ a una red _wifi_.

1. Crear una variable llamada _cfg_ del tipo _wifi_init_config_t_.
2. Llamar a la macro _WIFI_INIT_CONFIG_DEFAULT_.
3. Guardar el valor retornado en _cfg_.
4. Llamar la funcion _esp_wifi_init_.
5. Pasarle como parametro un punteo a _cfg_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
7. Llamar a la funcion _esp_wifi_set_mode_.
8. Pasarle como parametro _WIFI_MODE_STA_.
9. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
10. Llamar a la funcion _esp_wifi_start_.
11. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Buscar las redes wifi con mejor señal y mostrar su informacion en ESP-IDF

1. Crear una constante llamada _scan_list_size_ del tipo _uint16_t_.
2. El valor de _scan_list_size_ debe ser _10_.
3. Crear una variable llamada _ap_info_count_ del tipo _uint16_t_.
4. El valor inicial de _ap_info_count_ debe ser _scan_list_size_.
5. Crear un array llamado _ap_info_ del tipo _wifi_ap_record_t_ con un tamaño de _scan_list_size_ elementos.
6. Crear una variable llamada _ap_count_ del tipo _uint16_t_.
7. El valor inicial de _ap_count_ debe ser _0_.
8. Llamar a la funcion _memset_.
9. Pasarle como primer parametro _ap_info_.
10. Pasarle como segundo parametro _0_.
11. Pasarle como tercer parametro _sizeof(ap_info)_.
12. Llamar a la funcion _esp_wifi_scan_start_.
13. Pasarle como primer parametro _NULL_.
14. Pasarle como segundo parametro _true_.
15. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
16. Llamar a la funcion _esp_wifi_scan_get_ap_records_.
17. Pasarle como primer parametro un puntero a _ap_info_count_.
18. Pasarle como segundo parametro _ap_info_.
19. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
20. Llamar a la funcion _esp_wifi_scan_get_ap_num_.
21. Pasarle como parametro un puntero a _ap_count_.
22. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

En _ap_count_ tendremos el numero total de redes _wifi_ detectadas.

En _ap_info_count_ tendremos las primeras 10 redes _wifi_ que tengan mejor señal .

En cada elemento del tipo _wifi_ap_record_t_ perteneciente al array _ap_info_ tendremos:

- El miembro _.ssid_ con el nombre de la red.
- El miembro _.rssi_ con el nivel de señal.
- El miembro _.authmode_ con el modo de autenticacion.
- El miembro _.primary_ con el numero de canal.
