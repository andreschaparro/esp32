# Ejemplo 31: Subir archivos presentes en una carpeta del proyecto al SPIFFS en ESP-IDF

## Introduccion

Se subira a la particion del _SPIFFS_ los archivos de la carpeta _spiffs_dir_, la cual esta en el directorio raiz del proyecto.

En este caso, solo hay un archivo llamado _test.txt_.

Finalmente, se repetira un programa muy similar al del ejemplo 30 pero donde solo se leera el archivo _test.txt_.

NOTA: El directorio _spiffs_dir_ y el archivo _test.txt_ crearlos manualmente.

## Modificacion del CMakeLists.txt de la carpeta main

1. Agregar al final del archivo _spiffs_create_partition_image(storage ../spiffs_dir FLASH_IN_PROJECT)_.
2. Ejecutar _ESP-IDF Build, Flash and Monitor_.

Dentro de la carpeta _build_ podremos ver un archivo llamado _storage.bin_, que corresponde a la particion del _SPIFFS_, y donde estan embebidos todos los archivos del directorio _spiffs_dir_.

La directiva _FLASH_IN_PROJECT_ es para que se graben programa y archivos simultaneamente en el paso 2.
