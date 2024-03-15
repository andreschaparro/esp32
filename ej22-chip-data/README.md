# Ejemplo 22: Obtener informacion del SOC ESP32 que tenemos en ESP-IDF

Obtendremos varios datos utiles del _SOC_:

- Modelo.
- Disponibilidad de protocolos de comunicacion inalambricos.
- Si posee o no memorias embebidas.
- MAC address del fabricante y de la interfaz _Wifi_.
- Tamaño de la flash conectada por protocolo _SPI_.

Bibliotecas a incluir:

- **esp_chip_info.h**
- **esp_mac.h**
- **esp_flash.h**

## Uso de esp_chip_info.h

1. Crear una variable llamada _chip_info_ del tipo _esp_chip_info_t_.
2. Llamar a la funcion _esp_chip_info_.
3. Pasarle como parametro un puntero a _chip_info_.
4. Los campos de _chip_info_ tendran la siguiente informacion:
   - _model_ es un _enum_ que puede valer: _CHIP_ESP32_, _CHIP_ESP32C2_, _ESP32C3_, _ESP32C6_, _ESP32H2_, _ESP32S2_, _ESP32S3_.
   - _features_ es un _uint32_t_ donde cada bit representa una caracteristica que posee si es ta en _1_ o que no tiene si esta en _0_.
   - _revision_ es un _uint16_t_ con el formato _MXX_ que se debe interptretar como _M.XX_, que es la version del _SOC_.
   - _cores_ informa el numero de _COREs_.

NOTA: En el caso de _features_, consultar la documentacion oficial para saber el significado de cada uno.

NOTA: Repasar el uso del operador ternario de C.

## Uso de esp_mac.h

1. Crear un array de 6 elementos llamado _factory_mac_addr_ del tipo _uint8_t_
2. Llamar a la funcion _esp_read_mac_.
3. Pasarle como primer parametro _factory_mac_addr_.
4. Pasarle como segundo parametro _ESP_MAC_EFUSE_FACTORY_.
5. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

El valor en _factory_mac_addr_ sera la mac address que por defecto graba _Espressif_.

6. Crear un array de 6 elementos llamado _wifi_sta_mac_addr_ del tipo _uint8_t_
7. Llamar a la funcion _esp_read_mac_.
8. Pasarle como primer parametro _wifi_sta_mac_addr_.
9. Pasarle como segundo parametro _ESP_MAC_WIFI_STA_.
10. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

El valor en _wifi_sta_mac_addr_ sera la mac address que tiene la interfaz de comunicacion _Wifi_ cuando se conecta a un _Access Point_.

NOTA: Es posible cambiar la _mac_address_ de _Espressif_ por otra diferente. Para ello, consultar la documentacion.

NOTA: Consultar la documentacion, para saber como obtener las mac address de otras interfaces de comunicacion.

## Uso de esp_flash.h

1. Crear una variable llamada _flash_size_ del tipo _uint32_t_.
2. Inicializar la variable _flash_size_ en 0.
3. Llamar a la funcion _esp_flash_get_physical_size_.
4. Pasarle como primer parametro _esp_flash_default_chip_.
5. Pasarle como segundo parametro un puntero a _flash_size_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
7. Dividir el valor de _flash_size_ por 1024 dos veces para tener el tamaño de la memoria flash en _MB_.
