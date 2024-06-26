# Ejemplo 7: Colas en FreeRTOS

Una cola es una FIFO que permiten alamacenar una determinada cantidad de elementos, todos del mismo tamaño. El fin, es cumplir con el patron productor/consumidor y proporcionar un metodo de comunicacion entre tareas.

Se incluyen mecanismos de sincronizacion para cuando:

- Se quiere leer un dato y la cola esta vacia.
- Se quiere escribir un dato y la cola esta llena.

Leer o escribir un elemento en una cola es una operacion por copia! Tener en cuento esto.

Los elementos pueden ser: estructuras, variables, etc.

Incluir la biblioteca:

- "freertos/queue.h"

En el ejemplo crearemos dos tareas: _tarea_productor_ y _tarea_consumidor_. La primera, capturara caracteres por la UART y la segunda hara un log. El funcionamiento, es similar a una parte del ejemplo 2 pero utilizando colas y tareas.

## Crear una cola

1. Crear una instancia de la estructura _QueueHandle_t_ llamada **cola**.
2. Llamar a la macro _xQueueCreate()_.
3. Pasarle como primer parametro el numero de elementos que tendra, **10**.
4. Pasarle como segundo parametro el tamaño de cada elemento. Siempre hacer uso de la funcion _sizeof_. En este ejemplo, **sizeof(tx_buffer)**.
5. El valor retornado por la macro debe alamacenarse en **cola**.
6. Verificar con la macro _configASSERT()_ que _cola_ sea distinto de NULL.

## Escribir un dato en la cola

1. Llamar a la macro _xQueueSend()_.
2. Pasarle como primer parametro **cola**.
3. Pasarle como segundo parametro un puntero al elemento a enviar, casteado como _(void \*)_, en este caso **tx_buffer**.
4. Pasarle como segundo parametro 0 casteado como _(TickType_t)_. Este valor, es la cantidad de ticks a esperar si la cola esta llena con la tarea bloqueada. En caso de poner 0, la macro se vuelve no bloqueante.
5. Si el valor devuelto por _xQueueSend()_ es distinto de _pdTRUE_ hubo un error al escribir.

## Leer un dato de la cola

1. Llamar a la macro _xQueueReceive()_.
2. Pasarle como primer parametro **cola**.
3. Pasarle como segundo parametro un puntero al elemento donde se recibira, en este caso **rx_buffer**.
4. Pasarle como segundo parametro 5 casteado como _(TickType_t)_. Este valor, es la cantidad de ticks a esperar si la cola esta vacia con la tarea bloqueada. En caso de poner 0, la macro se vuelve no bloqueante. _portMAX_DELAY_ es lo maximo que se puede esperar.
5. Si el valor devuelto por _xQueueReceive()_ es igual a _pdTRUE_ se recibio el valor correctamente.

## Otras macros utiles

- _xQueuePeek()_ es similar a _xQueueReceive()_ pero no remueve el elemento.
- _uxQueueMessagesWaiting()_ devuelve la cantidad de mensajes esperando por ser leidos.
- _uxQueueSpacesAvailable()_ devuelve la cantidad de espacios para elementos disponibles en la cola.
- _vQueueDelete()_ destruye la cola.
- _xQueueReset()_ vuelve la cola a su estado original.

## Buena practica

Es una buena practica en _app_main_ poner al final _vTaskDelete(NULL)_.
