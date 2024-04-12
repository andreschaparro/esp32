# Ejemplo 35: Sincronizacion de la fecha y hora del del sistema utilizando un server NTP con el ESP-IDF

## Introduccion

El ESP32 utiliza dos timer por hardware para almacenar la fecha y hora del sistema. Uno es el _RTC_ o _real time clock_, y el otro es el _high resolution timer_ visto en el ejemplo 25.

La fecha y hora se podra tomar de varios server _NTP_ publicos. Por ejemplo: _time.windows.com_ y _pool.ntp.org_.

Para poder acceder a los mismos necesitaremos conectar el _ESP32_ a un red _wifi_ con salida a internet.

Bibliotecas a incluir:

- **stdlib.h**
- **time.h**
- **esp_netif_sntp.h**
- **esp_sntp.h**

## Preparacion para poder tener mas de un server NTP en ESP-IDF

1. Ejecutar _ESP-IDF SDK Configuration Editor (menuconfig)_.
2. Ir a _SNTP_.
3. En _Maximum number of NTP servers_ completar _5_.
4. Clic en _Save_.
5. Ejecutar _ESP-IDF Build project_.

## Inicializar la fecha y hora del sistema en ESP-IDF

1. Crear una variable llamada _config_ del tipo _esp_sntp_config_t_.
2. Llamar a la macro _ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE_.
3. Pasarle como primer parametro _5_.
4. Pasarle como segundo parametro el resultado de llamar a la macro _ESP_SNTP_SERVER_LIST_.
5. Pasarle como primer parametro _"time.windows.com"_.
6. Pasarle como segundo parametro _"0.pool.ntp.org"_.
7. Pasarle como tercer parametro _"1.pool.ntp.org"_.
8. Pasarle como cuarto parametro _"2.pool.ntp.org"_.
9. Pasarle como quinto parametro _"3.pool.ntp.org"_.
10. Llamar a la funcion _esp_netif_sntp_init_.
11. Pasarle como primer paramtro un puntero a _config_.
12. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
13. Crear una constante llamada _retry_count_ del tipo _uint8_t_ que valga _15_.
14. Crear un bucle _for_ con una variable llamada _i_ del tipo _uint8_t_ que inicie en _1_.
15. El bucle _for_ se debe ejecutar un maximo de _retry_count_ veces.
16. La variable _i_ debe incrementarse de a _1_.
17. Dentro del bucle _for_ llamar a la funcion _esp_netif_sntp_sync_wait_.
18. Pasarle como parametro _pdMS_TO_TICKS(10 \* 1000)_.
19. Si el valor retornado es _ESP_OK_ salir del bucle _for_ con un _break_.

## Configurar la zona horaria de argentina en ESP-IDF

1. Llamar a la funcion _setenv_.
2. Pasarle como primer parametro _"TZ"_.
3. Pasarle como segundo parametro _"ART3"_.
4. Pasarle como tercer parametro _1_.
5. Llamar a la funcion _tzset_.

## Obtener la fecha y hora del sistema en ESP-IDF.

1. Crear una variable llamada _now_ del tipo _time_t_
2. Inicializar _now_ en _0_.
3. Llamar a la funcion _time_.
4. Pasarle como parametro un puntero a _now_.
5. Crear una instancia global de _struct tm_ llamada _timeinfo_.
6. Inicializarla en _{0}_.
7. Llamar a la funcion _localtime_r_.
8. Pasarle como primer parametro un puntero a _now_.
9. Pasarle como segundo parametro un puntero a _timeinfo_.

# Imprimir la fecha y hora del sistema en ESP-IDF

1. Crear un _array_ global llamado _strftime_buf_ de 64 elementos del tipo _char_.
2. Llamar a la funcion _strftime_.
3. Pasarle como primer parametro _strftime_buf_.
4. Pasarle como segundo parametro _sizeof(strftime_buf)_.
5. Pasarle como tercer parametro _"%c"_.
6. Pasarle como cuarto parametro un puntero a _timeinfo_.
7. Hacer un _log_ de _strftime_buf_ con formato _"%s"_.
