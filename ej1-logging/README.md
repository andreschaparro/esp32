# Ejemplo 1: Logging en ESP-IDF

## Preparacion

1. Ejecutar _ESP-IDF SDK Configuration Editor (menuconfig)_.
2. Ir a _Serial flasher config_.
3. En _Flash SPI speed_ seleccionar **80 Mhz**.
4. En _Flash size_ seleccionar **4 MB**.
5. En el directorio _.vscode_ abrir el archivo _c_cpp_properties.json_.
6. Luego de _"compilerPath"_ agregar dos elementos: **"cStandard": "c11"** y **"cppStandard": "c++17"**.
7. Luego de _"browse"_ agregar **"compileCommands": "${workspaceFolder}/build/compile_commands.json"**.
8. Ejecutar _ESP-IDF Flash device_ y seleccionar **UART**
9. Ejecutar _ESP-IDF Build project_.

NOTA 1: la primera ejecucion de _ESP-IDF Build project_ puede demorar varios minutos.

NOTA 2: Esto debe hacerse en todos los ejemplos.

## "esp_log.h"

La biblioteca define 5 macros, que nos permiten tener 5 niveles de log:

1. _ESP_LOGE_ que muestra un log de error en color rojo.
2. _ESP_LOGW_ que muestra un log de warning en color amarillo.
3. _ESP_LOGI_ que muestra un log de informacion en color verde.
4. _ESP_LOGD_ que muestra un log de debug en color neutro.
5. _ESP_LOGV_ que muestra un log de verbose en color neutro.

Los mensajes se veran al ejecutar el _ESP-IDF Monitor device_.

NOTA: Siempre es mas util hacer un _ESP-IDF Build, Flash and Monitor_ para compilar, grabar, y hacer un debug de nuestro programa en un solo paso.

La forma de usar las macros es la siguiente:

ESP_LOGE(tag, format, VA_ARGS)

- tag es un const char\* que se utiliza para identificar en que parte del codigo se produjo el log.
- format y VA_ARGS funcionan de la misma formar que los parametros que se le pasan a printf para generar cadenas de texto.

Por defecto, las macros _ESP_LOGD_ y _ESP_LOGV_ no imprimiran logs en el el monitor de debug. La primera, es util para debug, pero la segunda, no tiene utilidad practica.

Para habilitar la impresion de logs de _ESP_LOGD_ en todo el proyecto:

1. Ejecutar _ESP-IDF SDK Configuration Editor (menuconfig)_.
2. Ir a _Component config_ y dentro a _Log output_.
3. En _Default log verbosity_ seleccionar **debug**.

NOTA: Cuando se altera menuconfig y se recompila el proyecto va a demorar varios minutos.

Para habilitar la impresion de logs de _ESP_LOGD_, para un solo tag:

1. Ejecutar _ESP-IDF SDK Configuration Editor (menuconfig)_.
2. Ir a _Component config_ y dentro a _Log output_.
3. En _Maximum log verbosity_ seleccionar **debug**.
4. En el codigo agregar esp_log_level_set(tag, ESP_LOG_DEBUG).

Lo mas practico, es utilizar la segunda opcion. Agregar, esto ultimo al paso de preparacion.
