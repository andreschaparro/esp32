# Ejemplo 4: Crear, eliminar, suspender, y reanudar Tareas en FreeRTOS

Incluir las bibliotecas:

- "freertos/FreeRTOS.h"
- "freertos/task.h"

# Crear una tarea

En la \*app_main\*\* se crean las tareas. Luego, si no se le pone un loop infinito termina.

Llamar a la funcion _xTaskCreate_. A la que hay que pasarle los siguientes parametros:

- Nombre de la funcion que ejecutara. La estructura de la funcion deberia ser como la de un _main_ en _bare-metal_.
- Un String que contendra el nombre de la tarea a modo de informacion.
- El tamaño del stack, en bytes.
- Se puede pasar un parametro a traves de un puntero. Pero, si no se desea pasar nada, completar NULL.
- Prioridad, que puede valer de _tskIDLE_PRIORITY_ a _configMAX_PRIORITIES_ – 1. O sea, de 0 a 24. Por ello, usar siempre un valor de _tskIDLE_PRIORITY_ + 1 como minimo.
- Un puntero a una instancia de la estructura _TaskHandle_t_. Dicha instancia, hara referencia a la tarea creada y nos permitira tomar acciones sobre ella como veremos mas adelante.

La instancia de la estructura _TaskHandle_t_ debe inicializarse con NULL.

Cuando, una tarea creada con _xTaskCreate_ pasa a modo de ejecucion podra correr tanto en el CORE_0 como en el CORE_1. Esto, lo determinara el scheduler de FreeRTOS.

En cambio, si deseamos que corra siempre en el mismo CORE, debemos utilizar la funcion _xTaskPinnedToCore_. A la que, le pasaremos los mismos parametros que a _xTaskCreate_ junto al siguiente:

- _PRO_CPU_NUM_ o _APP_CPU_NUM_.

En FreeRTOS, las funciones que tienen el prefijo _x_ en su nombre pueden devuelven un valor del tipo _BaseType_t_. En cambio, el prefijo _v_ como aparece en _vTaskDelay_ indica que devuelve void.

Si devuelve un _BaseType_t_, se debe verificar el valor con la macro _configASSERT_.

La macro _configASSERT_ verifica condiciones del tipo booleanas, del mismo modod que _assert_. Asi, debemos pasarle como parametro que la condicion de que valor retornado sea igual a _pdPASS_.

# Eliminar una tarea

Llamar a la funcion _vTaskDelete_ y pasarle como parametro la instancia de la estructura _TaskHandle_t_ . Si en cambio, le pasamos un NULL, se entiende que nos estamos refiriendo a la misma tarea que invoco la funcion.

# Suspender una tarea

Implica que una tarea salga del scheduler, pero sin destruir su informacion. En otras palabras, el scheduler no podra pasar a ejecutarla hasta que se la reanude explicitamente.

Llamar a la funcion _vTaskSuspend_ y pasarle como parametro la instancia de la estructura _TaskHandle_t_ . Si en cambio, le pasamos un NULL, pasa lo mismo que con _vTaskDelete_.

# Reanudar una tarea

Llamar a la funcion _vTaskResume_ y pasarle como parametro la instancia de la estructura _TaskHandle_t_ . En este caso, no puede pasarse NULL porque una tarea suspendida no puede reanudarse a si misma, debe hacerlo otra.

# Obtener el numero de tick de FreeRTOS correspondiente a cuando entra en ejecucion una tarea

Llamar a la funcion _xTaskGetTickCount_ y considerar que devuelve un uint32_t.

Los ticks se cuentan desde el arranque y se debe tener cuidado porque este valor puede desbordar.
