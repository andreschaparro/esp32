# Software Timer en FreeRTOS

Los timers sirven para generar eventos de _timeout_ y a futuro ejecutar una funcion de _callback_.

Usan el tick de FreeRTOS. Por lo que, estan limitados temporalmente por la duracion del tick.

Incluir la biblioteca:

- "freertos/timers.h"

El prototipo de la funcion de callback:

- devuelve _void_.
- tiene como parametro _Timer_Handle_t_ _xTimer_.

Dentro de la callback se pueden utilizar funcion de FreeRTOS que no sean bloqueantes y si tienen el parametro _TicksToWait_ siempre debe valer **0**.

En el ejemplo, volveremos a hacer lo mismos que en el ejemplo 2 donde parpadeaba un led. Pero, lo haremos con un software timer, un semaforo binario, y una tarea

## Crear el timer

1. Crear una constante del tipo TickType_t llamada **tim_period**.
2. Inicializarla con la macro _pdMS_TO_TICKS()_ con el valor **500**.
3. Crear una instancia de la estructura _TimerHandle_t_ llamada **tim**.
4. Inicializarla en **NULL**.
5. Llamar a la funcion _xTimerCreate()_.
6. Pasarle como primer parametro un String que sera el nombre del timer a modo de informacion.
7. Pasarle como segundo parametro **tim_period**.
8. Pasarle como tercer parametro pdTRUE si queremos que el timer se dispare periodicamente, o pdFALSE para que se dispare una unica vez. En este caso, elegir **pdTRUE**
9. Pasarle como cuarto parametro un puntero a una variable, un array, o una escructura. Permitira, compartir informacion entre la tarea que creo al timer y la _callback_. Pasarle **NULL** casteado como **(void \*)**.
10. Pasarle como quinto parametro la funcion de _callback_ que se llama **tim_callback**.
11. Verificar con la macro _configASSERT()_ que **tim** sea distinto de NULL.

## Arrancar el timer

1. Crear una variable del tipo _BaseType_t_ llamada **result**.
2. Llamar a la macro _xTimerStart()_.
3. Pasarle como primer parametro **tim**.
4. Pasarle como segundo parametro 0 casteado como (TickType_t). Este valor, es la cantidad de ticks a esperar si no es posible arrancar el timer.
5. Si el valor devuelto por _xTimerStart()_ es igual a pdTRUE se tuvo exito.

## Detener un timer

Es similar a arrancarlo pero llamando a la macro _xTimerStop()_.

## Reiniciar un timer

El mismo debe estar corriendo, y es similar a arrancarlo pero llamando a la macro _xTimerReset()_.

## Borrar un timer

Es similar a arrancarlo pero llamando a la macro _xTimerDelete()_.
