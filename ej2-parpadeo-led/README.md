# Ejemplo 2: Parpadeo de un led en ESP-IDF

## Preparacion

1. Ejecutar _ESP-IDF SDK Configuration Editor (menuconfig)_.
2. Ir a _Serial flasher config_.
3. En _Flash SPI speed_ seleccionar **80 Mhz**.
4. En _Flash size_ seleccionar **4 MB**.
5. Ir a _Component config_ y dentro a _Log output_.
6. En _Maximum log verbosity_ seleccionar **debug**.
7. En el directorio _.vscode_ abrir el archivo _c_cpp_properties.json_.
8. Luego de _"compilerPath"_ agregar dos elementos: **"cStandard": "c11"** y **"cppStandard": "c++17"**.
9. Luego de _"browse"_ agregar **"compileCommands": "${workspaceFolder}/build/compile_commands.json"**.
10. Ejecutar _ESP-IDF Flash device_ y seleccionar **UART**
11. Ejecutar _ESP-IDF Build project_.

NOTA 1: la primera ejecucion de _ESP-IDF Build project_ puede demorar varios minutos.

NOTA 2: Esto debe hacerse en todos los ejemplos.

## "driver/gpio.h"

Para definir e inicializar la salida que activa el led:

1. Crear una variable llamada _led_ y del tipo _const gpio_num_t_.
2. Asignale como valor el numero de GPIO con el enum **GPIO_NUM_2**.
3. Llamar a la funcion _gpio_set_direction()_.
4. Pasarle como primer parametro la variable _led_.
5. Pasarle como segundo parametro el enum **GPIO_MODE_OUTPUT**.

Para cambiar el estado del led:

1. Llamar a la funcion _gpio_set_level_.
2. Pasarle como primer parametro la variable _led_.
3. Pasarle como segundo parametro 0 para apagarlo o 1 para encenderlo.

## Delay

El ESP32 tien 2 COREs, uno es el _CORE_0_ y el otro el _CORE_1_, llamados _PRO_CPU_ y _APP_CPU_ respectivamente.

El _app_main_ se ejecuta en el _CORE_0_ y es la unica tarea de FreeRTOS que existe por el momento.

FreeRTOS es apropiativo. Asi, que si entramos al loop infinito de nuestra unica tarea, debemos ceder el uso del CPU periodicamente para evitar que se produzca un error y se reinicie el ESP32.

Para ello, se utiliza la funcion _vTaskDelay()_.

Al hacer esto, ponemos a _app_main_ en estado de bloqueo durante un tiempo. Asi, el comportamiento que se obtiene es similar al de la funcion _delay()_ de Arduino.

Para inicializar y llamar a la funcion que genera el delay:

1. Crear una variable llamada _delay_1000_ms_ y del tipo _const TickType_t_.
2. Asignale como valor el numero de ticks devuelto por la macro _pdMS_TO_TICKS()_.
3. Pasarle como parametro 1000, que el el tiempo de demora en ms.
4. Llamar a la funcion _vTaskDelay()_ dentro del loop infinito.
5. Pasarle como parametro la variable _delay_1000_ms_.

NOTA: el tipo _TickType_t_ se usa para guardar un numero de ticks de FreeRTOS.
