# Ejemplo 8: Semaforos en FreeRTOS

Resuelven procesos de sincronizacion, asi como las colas resuelven la comunicacion entre tareas.

Un semaforo es un numero entero que se inicializa en un valor mayor a 0. Luego, se lo puede incrementar o decrementar en 1.

Cuando una tarea decrementa el semaforo, si el resultado es negativo, la tarea se bloquea hasta que otra lo incremente.

Un semaforo se utiliza para resolver un problema de serializacion mediante la señalizacion. Es decir, que algo se ejecute antes que otra cosa si o si. Por ejemplo, que una tarea o una isr le avise a otra tarea que algo paso para que actue.

NOTA: La señalizacion se puede hacer en ambos sentidos. Por ejemplo, si consideramos que 2 tareas estan compuestas por 2 segmentos de codigo. Si, nos queremos asegurar que el primer segmento se la tarea 1 se ejecute antes del segundo segmento de la tarea 2. Y al mismo tiempo, nos queremos asegurar que el primer segmento se la tarea 2 se ejecute antes del segundo segmento de la tarea 1. Esto se conce como cita ( rendezvous ). En estos casos, se puede producir un punto muerto o interbloqueo (deadlock).

En FreeRTOS hay 3 tipos de semaforos:

- Binarios que pueden valer 0 o 1.
- Contador que puede valer 0, 1, 2, 3, ...
- Mutex que puede valer 0 o 1 con un algoritmo que se vera en el ejemplo 9.

Incluir la biblioteca:

- "freertos/semphr.h"

El ejemplo, nos permitira reescribir el codigo del ejemplo 6 utilizando dos tareas, una isr, y un semaforo binario. La primera tarea, inicializa el pulsador. Luego, la ISR, libera un semaforo binario cuando se presiona el pulsador. Y, finalmente La segunda tarea invierte el estado del led cuando pueda tomar el semaforo.

Se vuelve un requisito que primero se presione el pulsador para luego modificar el estado del led. Por lo que, es un ejemplo de serializacion.

## Crear un semaforo binario

1. Crear una instancia de la estructura _SemaphoreHandle_t_ llamada **sem_bin**.
2. Inicializarla en **NULL**.
3. Llamar a la macro _xSemaphoreCreateBinary()_.
4. El valor retornado por la macro debe alamacenarse en **sem_bin**.
5. Verificar con la macro _configASSERT()_ que _sem_bin_ sea distinto de NULL.

NOTA: Un semaforo binario siempre se crea tomado.

## Dar un semaforo binario desde una ISR

1. Crear una variable del tipo _BaseType_t_ llamada **despertar_tarea**.
2. Inicializarla en _pdFALSE_.
3. Llamar a la macro _xSemaphoreGiveFromISR()_.
4. Pasarle como primer parametro **sem_bin**.
5. Pasarle como segundo parametro la direccion de **despertar_tarea**.
6. Preguntar con if si **despertar_tarea** es igual a _pdTRUE_.
7. En caso afirmativo, llamar a la macro _portYIELD_FROM_ISR()_.

NOTA: usar este metodo y no la funcion _xTaskResumeFromISR()_ como en el ejemplo 6.

Nota: en caso de dar el semaforo desde una tarea usar _xSemaphoreGive()_.
s

## Tomar un semaforo binario

1. Llamar a la macro _xSemaphoreTake()_.
2. Pasarle como primer parametro **sem_bin**.
3. Pasarle como segundo parametro 5 casteado como _(TickType_t)_. Este valor, es la cantidad de ticks a esperar si el semafoto esta tomado con la tarea bloqueada. En caso de poner 0, la macro se vuelve no bloqueante. _portMAX_DELAY_ es lo maximo que se puede esperar.
4. Si el valor devuelto por _xSemaphoreTake()_ es igual a _pdTRUE_ se pudo tomar el semaforo.

## Crear, dar, y tomar de un semaforo contador

El procedimiento es similar al binario, salvo que se utiliza la macro _xSemaphoreCreateCounting_. Como primer parametro, se le pasa el valor maximo que puede tener. Y como segundo parametro, se le pasa el valor inicial.

Las funciones para tomar y dar son las mismas que las del semaforo binario.
