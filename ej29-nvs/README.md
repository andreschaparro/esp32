# Ejemplo 29 : Almacenamiento no volatil (NVS) en ESP-IDF

## Introduccion

El _NVS_ esta pensado para almacenar pares _clave-valor_ en una particion de la memoria _flash_.

Cada particion tiene un: _name_, un _type_, y un _subtype_ que las identifica.

En este caso, el _name_ es _nvs_, el _type_ debe _data_, y el _subtype_ es _nvs_ y viene configurada por defecto.

NOTA: En el ejemplo 30 se vera como particionar la _flash_.

Las _claves_ son strings de hasta 15 caracteres.

Los valores pueden ser:

- _uint8_t_, _int8_t_, _uint16_t_, _int16_t_, _uint32_t_, _int32_t_, _uint64_t_, _int64_t_.
- _strings_ terminados en _\'0_
- datos binarios (_blob_).

Los pares _clave-valor_ se agrupan dentro de un _namespace_. El mismo, tambien tendra hasta 15 caracteres.

Esto, permite reutilizar alguna _clave_ sin pisar su _valor_ al hacer escrituras.

Por ejemplo, podria tener varias _claves_ llamadas _pwd_ que sean para diferentes usos.

NOTA: _float_ y _double_ no estan soportados nativamente el momento. Pero, pueden encapsularse dentro de un _struct_ y utilizar _blob_ para guardarlos.

Bibliotecas a incluir:

- **esp_system.h**
- **nvs_flash.h**
- **nvs.h**

## Inicializar el NVS en ESP-IDF

1. Crear una variable llamada _err_ del tipo _esp_err_t_
2. Llamar a la funcion _nvs_flash_init_.
3. Guardar el valor retornado en _err_.
4. Si _err_ es igual a _ESP_ERR_NVS_NO_FREE_PAGES_ o a _ESP_ERR_NVS_NEW_VERSION_FOUND_:
   a. Llamar a la funcion _nvs_flash_erase_.
   b. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
   c. Repetir los pasos 1, 2, y 3.
5. Verificar el valor de _err_ con la macro _ESP_ERROR_CHECK_.

## Creacion del namespace en ESP-IDF

1. Crear una variable llamada _my_handle_ del tipo _nvs_handle_t_.
2. Llamar a la funcion _nvs_open_.
3. Pasarle como primer parametro el _namespace_ al que queremos acceder. En este caso, se llama _storage_.
4. Pasarle como segundo parametro _NVS_READWRITE_ para poder leer y escribir. Caso contrario, pasarle _NVS_READONLY_ para solo leer.
5. Pasarle como tercer parametro un puntero a _my_handle_.
6. Guardar el valor retornado en _err_.
7. Si el valor retornado es distinto de _ESP_OK_, ver el contenido del _string_ retornado por la funcion _esp_err_to_name_, a la que se le pasa como parametro _ret_.
8. Caso contrario, podremos proceder a leer y/o escribir pares _clave-valor_.

## Leer un par clave-valor en ESP-IDF

1. Crear una variable del tipo que se va a leer.
2. Llamar a la funcion correspondiente al tipo de variable a leer:
   - _nvs_get_u8_
   - _nvs_get_u16_
   - _nvs_get_u32_
   - _nvs_get_u36_
   - _nvs_get_i8_
   - _nvs_get_i16_
   - _nvs_get_i32_
   - _nvs_get_i64_
   - _nvs_get_str_
   - _nvs_get_str_
3. Pasarle como primer parametro la variable _my_handle_.
4. Pasarle como segundo parametro la _clave_.
5. Pasarle como tercer parametro un puntero a la variable de destino creada en el paso 1.
6. Guardar el valor retornado en _err_.
7. Con un _switch_ evaluar los posibles valores de _err_:
   - _ESP_OK_ significa que el dato se leyo con exito y esta disponible en variable creada en el paso 1.
   - _ESP_ERR_NVS_NOT_FOUND_ significa que la _clave-valor_ no existe todavia.
   - _default_ hacer un _ESP_LOGE_ del _%s_ retornado por la funcion _esp_err_to_name_ pasandole como parametro _err_.

NOTA: Las funciones _nvs_get_str_ y _nvs_get_str_ pueden requerir otro parametros.

## Escribir un par clave-valor en ESP-IDF

1. Llamar a la funcion correspondiente al tipo de variable que se va a escribir:
   - _nvs_set_u8_
   - _nvs_set_u16_
   - _nvs_set_u32_
   - _nvs_set_u36_
   - _nvs_set_i8_
   - _nvs_set_i16_
   - _nvs_set_i32_
   - _nvs_set_i64_
   - _nvs_set_str_
   - _nvs_set_blob_
2. Pasarle como primer parametro la variable _my_handle_.
3. Pasarle como segundo parametro la _clave_.
4. Pasarle como tercer parametro la variable a escribir.
5. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
6. Llamar a la funcion _nvs_commit_.
7. Pasarle como parametro la variable _my_handle_.
8. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: Las funciones _nvs_set_str_ y _nvs_set_blob_ pueden requerir otro parametros.

NOTA: Los pasos 6, 7, y 8 deben hacerse una vez que se escribieron todos los valores.

## Cerrar el acceso a un namespace en ESP-IDF

1. Llamar a la funcion _nvs_close_.
2. Pasarle como parametro la variable _my_handle_.

NOTA: La funcion _esp_restart_ reinicia el ESP32 por software.
