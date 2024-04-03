# Ejemplo 30: File system para memoria flash conectada por SPI (SPIFFS) en ESP-IDF

## Introduccion

El _SPIFFS_ esta pensado para almacernar archivos en la memoria flash conectada por el bus de comunicacion _SPI_.

Pero posee las siguientes limitaciones:

- No soporta directorios. Todos los archivos quedan montados sobre la ruta _/spiffs_.
- No tiene un comportamiento deterministico, por lo que una operacion de escritura puede tardar mas que otra.
- No detecta o maneja sectores con errores.
- Puede utilizar, como mucho, un 75% del espacio disponible originalmente.
- Cuando se esta llenando la memoria, puede tardar muchos segundos en la busqueda de espacio libre para hacer una operacion de escritura.
- Borrar un archivo, puede dejar sectores inservibles a futuro.
- Una falla de alimentacion puede corromper todo el sistema de archivos.

No es recomendable hacer operaciones de escritura, solo de lectura, por lo que no sirve para hacer un _datalogger_. En cambio, si sirve para almacenar los archivos de un _webserver_.

En este caso, no se utiliza el bus de comunicacion _SPI_ de forma directa, sino que esta enmascarada por el _ESP-IDF_. Esto, se llama _wrapper_ en programacion.

En el ejemplo 32, explicaremos las caracteristicas del bus de comunicacion _SPI_ cuando conectemos una tarjeta _SD_ al _ESP32_.

Bibliotecas a incluir:

- **esp_spiffs.h**

## Particion de la flash para el SPIFFS en ESP-IDF

Es necesario crear una particion dentro de la flash para almacenar el _SPIFFS_.

1. Ejecutar el _ESP-IDF SDK Configuration Editor (menuconfig)_.
2. Ir a _Partition Table_.
3. Seleccionar _Custom partition table CSV_.
4. Crear un archivo llamado _partitions_ con extension _.csv_ en el directorio raiz del proyecto.
5. Copiar al archivo _partitions.csv_ el siguiente contenido:

   \# ESP-IDF Partition Table

   \# Name, Type, SubType, Offset, Size, Flags

   nvs, data, nvs, 0x9000, 0x6000,

   phy_init, data, phy, 0xf000, 0x1000,

   factory, app, factory, 0x10000, 1M,

   storage, data, spiffs, , 0xF0000,

6. Volver a compilar.

El valor por defecto de _Partition Table_ es _Single factory app, no OTA_.

Dicha opcion, tiene el mismo contenido que el copiado en el paso 5, excepto por la linea de _storage, data, spiffs, , 0xF0000,_.

Mas adelante, se veran otros esquemas de particionado para habilitar mas funcionalidades del _ESP-IDF_.

Agregar este paso a la preparacion de todos los proyectos a partir de ahora.

## Inicializar el SPIFFS en ESP-IDF

1. Crear una variable llamada _conf_ del tipo _esp_vfs_spiffs_conf_t_:
   - El miembro _.base_path_ debe valer _"/spiffs"_.
   - El miembro _.partition_label_ debe valer _NULL_.
   - El miembro _.max_files_ debe valer _5_.
   - El miembro _.format_if_mount_failed_ debe valer _true_.
2. Crear una variable llamada _ret_ del tipo _esp_vfs_spiffs_conf_t_.
3. Llamar a la funcion _esp_vfs_spiffs_register_.
4. Pasarle como parametro un puntero a _conf_.
5. Guardar el valor retornado en _ret_.

Si _ret_ es igual a _ESP_OK_ no hay errores.

En cambio, si _ret_ es igual a _ESP_FAIL_ indica que no se pudo montar el _file system_.

En cambio, si _ret_ es igual a _ESP_ERR_NOT_FOUND_ indica que no hay una particion en la _flash_ definida para el _SPIFFS_.

En cambio, si _ret_ tiene otro valor,

1. Llamar a la funcion _esp_err_to_name_.
2. Pasarle como parametro _ret_.
3. Hacer un _log_ con el formato _%s_ del valor retornado.

## Obtener informacion del SPIFFS y formatear en caso de error en ESP-IDF

1. Crear una variable llamada _total_ del tipo _size_t_ e inicializarla en _0_.
2. Crear una variable llamada _used_ del tipo _size_t_ e inicializarla en _0_.
3. Llamar a la funcion _esp_spiffs_info_.
4. Pasarle como primer paramtro _conf.partition_label_.
5. Pasarle como segundo parametro un puntero a _total_.
6. Pasarle como tercer parametro un puntero a _used_.
7. Guardar el valor retornado en _ret_.

Si _ret_ es igual a _ESP_OK_ no hay error, y el tamaño de la particion esta en _total_, y el espacio utilizado en _used_.

En cambio, si _ret_ tiene otro valor:

1. Llamar a la funcion _esp_spiffs_format_.
2. Pasarle como parametro _conf.partition_label_.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Verificar la consistencia del SPIFFS en ESP-IDF

Si _used_ es mayor a _total_:

1. Llamar a la funcion _esp_spiffs_check_
2. Pasarle como parametro _conf.partition_label_.
3. Guardar el valor retornado en _ret_.

Si _ret_ es igual a _ESP_OK_ no hay error.

En cambio, si _ret_ tiene otro valor:

1. Llamar a la funcion _esp_err_to_name_.
2. Pasarle como parametro _ret_.
3. Hacer un _log_ con el formato _%s_ del valor retornado.

## Desmontar y dehabilitar el SPIFFS en ESP-IDF

1. Llamar a la funcion _esp_vfs_spiffs_unregister_.
2. Pasarle como parametro _conf.partition_label_.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Escribir y leer un archivo en ESP-IDF

Se hace utilizando las bibliotecas estandard de C y POSIX para manejar archivos.

## Conclusiones

Este ejemplo es integrador porque:

- Utilizamos un sistema de almacenamiento de archivos como _SPIFFS_.
- Realizamos un caso de particionado de la memoria _flash_.

NOTA: Hay una biblioteca alternativa a _SPIFFS_ que se llama _LittleFS_. Y, esta dentro de los modulos mantenidos por _Espressif_.
