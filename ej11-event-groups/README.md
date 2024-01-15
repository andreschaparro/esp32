# Ejemplo 11: Event Groups en FreeRTOS

Es otra solucion para problemas de sincronizacion. Es diferente a semaforos y mutex.

Su uso es para esperar una combinacion de eventos.

Es basicamente un grupo de semaforos binarios que funcionan como flags.

En el ejemplo, se crean 3 tareas. Una llamada _tarea_1_, otra _tarea_2_, y otra _tarea_3_. Las primeras 2, deben avisarle a la _tarea_3_ que terminaron de ejecutarse para que la tercera procedera con su ejecucion.

Incluir la biblioteca:

- "freertos/event_groups.h"

## Crear un event group

1. Crear una instancia de la estructura _EventGroupHandle_t_ llamada **grupo**.
2. Llamar a la funcion _xEventGroupCreate()_.
3. Guardar el valor retornado en **grupo**.
4. Verificar con la macro configASSERT() que grupo sea distinto de NULL.

## Levantar un flag en el event group para las dos tareas

1. Crear dos constantes del tipo _EventBits_t_ llamadas **flag_1** y **flag_2**.
2. Inicializar **flag_1** en **BIT0**.
3. Inicializar **flag_2** en **BIT1**.
4. Crear una variable del tipo _EventBits_t_ llamada _flags_ dentro de la _tarea_1_.
5. Dentro la _tarea_1_ llamar a la funcion _xEventGroupSetBits()_.
6. Pasarle como primer parametro **grupo**.
7. Pasarle como segundo parametro **flag_1**.
8. Guardar el valor retornado en _flags_.
9. Hacer un log de los flags para hacer un debug del estado de los bits.
10. Crear una variable del tipo _EventBits_t_ llamada _flags_ dentro de la _tarea_2_.
11. Dentro la _tarea_2_ llamar a la funcion _xEventGroupSetBits()_.
12. Pasarle como primer parametro **grupo**.
13. Pasarle como segundo parametro **flag_1**.
14. Guardar el valor retornado en _flags_.
15. Hacer un log de los flags para hacer un debug del estado de los bits.

## Esperar que se levanten los flags

1. Crear una variable del tipo _EventBits_t_ llamada _flags_ dentro de la _tarea_3_.
2. Dentro la _tarea_3_ llamar a la funcion _xEventGroupWaitBits()_.
3. Pasarle como primer parametro **grupo**.
4. Pasarle como segundo parametro una OR de los flags a esperar. En este caso, **flag_1 | flag_2**.
5. Pasarle como tercer parametro pdTRUE para que baje los flags, o pdFALSE para que los mantenga. En este caso, **pdTRUE**.
6. Pasarle como cuarto parametro pdTRUE si todos los flags deben estar levantados para que la tara deje de esta bloqueada, o pdFALSE si necesitamos solo uno en alto. En este caso, **pdTRUE**.
7. Pasarle como quinto parametro 100 casteado como (TickType_t). Este valor, es la cantidad de ticks a esperar si no se levantan los flags con la tarea bloqueada.
8. Guardar el valor retornado en _flags_.
9. Con un if verificar que se activaron ambos flags utilizando la expresion **(flags & (flag_1 | flag_2)) == (flag_1 | flag_2)**.
10. En caso afirmativo, hacer un log de los flags para hcaer un debug del estado de los bits antes de limpiarlos.
11. En caso negativo, hacer un log de warning indicando que hubo timeout.

NOTA: Siempre cuando se pasa un _TickType_t_ como con las colas, semaforos, y mutex el maximo valor a esperar es **portMAX_DELAY**.
