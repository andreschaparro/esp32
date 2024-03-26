# Ejemplo 28: Analisis de la de memoria en el ESP32 con ESP-IDF y FreeRTOS

## Introduccion a los tipos de memoria en el ESP32

Toda la memoria, es compartida entre el _APP_CPU_NUM_ y el _PRO_CPU_NUM_.

Hay un bus de memoria para instrucciones y otro para datos.

Se puede leer/escribir a traves del bus de instrucciones con una alineacion de 4bytes.

Se puede leer/escribir a traves del bus de datos con una alineacion de a 1byte, 2bytes, o 4bytes.

El ESP32 tiene _520KB_ de _SRAM_ embebida en total.

NOTA: _SRAM_ es _station random access memory_.

La _SRAM_ se divide en 3 segmentos: _SRAM0_, _SRAM1_, y _SRAM2_.

NOTA: El modulo de acceso directo a memoria o _DMA_ utiliza parte de _SRAM1_ y de _SRAM2_.

El ESP32 tiene _448KB_ de _ROM_ embebida en total.

La _ROM_ se divide en 2 segmentos: _ROM0_ y _ROM1_.

Puede colocarse hasta _16MB_ de memoria _Flash externa_ utilizando el bus de comunicacion _SPI_.

Puede colocarse hasta _8MB_ de memoria _SRAM externa_ utilizando el bus de comunicacion _SPI_.

## Introduccion a los tipos de memoria en ESP-IDF

La memoria _SRAM_ leida o escrita por el bus de datos se llamara _DRAM_.

La memoria _SRAM_ leida o escrita por el bus de instrucciones se llamara _IRAM_.

NOTA: no confundir la sigla _DRAM_ con _Dynamic RAM_.

Una parte de la _DRAM_, se puede utilizar para alojamiento estatico.

Otra parte de la _DRAM_, se utiliza como _heap_ de _FreeRTOS_.

Bibliotecas a incluir:

- **esp_heap_caps**

## Atributos en ESP-IDF

El atributo _IRAM_ATTR_ de las _ISR_, fuerza a que se cargue el codigo en la _IRAM_. Sino, por defecto estaria en la _IROM_.

La _IROM_ es la parte de la _ROM_ que se acceder por el bus de instrucciones.

Si hay constantes definidas dentro de las _ISR_, colocarles el atributo _DRAM_ATTR_ para que se almacenen en la _DRAM_. Sino, por defecto estarian en la _DROM_.

La _DROM_ es la parte de la _ROM_ que se utliza para almacer las _const_.

NOTA: solo las _ISR_ deben alojarse en la _IRAM_.

## Obtener el tamaño de la DRAM en ESP-IDF

La primera forma es:

1. Crear una variable llamada _d_ram_ del tipo _size_t_.
2. Llamar a la funcion _heap_caps_get_free_size_.
3. Pasarle como parametro _MALLOC_CAP_8BIT_.
4. Guargar el valor retornado en _d_ram_.

La segunda forma es:

1. Crear una variable llamada _d_ram_ del tipo _size_t_.
2. Llamar a la funcion _xPortGetFreeHeapSize_.
3. Guargar el valor retornado en _d_ram_.

## Obtener el tamaño de la IRAM en ESP-IDF

1. Crear una variable llamada _i_ram_ del tipo _size_t_.
2. Llamar a la funcion _heap_caps_get_free_size_.
3. Pasarle como parametro _MALLOC_CAP_32BIT_.
4. Llamar a la funcion _heap_caps_get_free_size_.
5. Pasarle como parametro _MALLOC_CAP_8BIT_.
6. Guargar la resta del primer valor retornado con el segundo en _i_ram_.

## Memoria estatica en FreeRTOS

La memoria estatica, es asignada por el compilador y reservada antes de ejecutar el programa

Y la misma, utiliza el _stack_.

Las variables locales, son un ejemplo de asignacion estatica.

La asignacion, es permanente mientras se ejecuta la funcion, o tarea. Por lo que, no se puede reutilizar el espacio.

Por ejemplo, si se define un array cuya longitud esta sobredimensionada, no se podran utilizar los bytes sobrantes para guardar un float.

El compilador es responsable de manejar del _stack_.

Tiene como ventaja, una ejecucion es mas rapida.

## Memoria dinamica en FreeRTOS

Es asignada por la funcion de _malloc_ y desasignada con _free_ mientras se ejecuta el programa.

Para la misma, se utliza el _heap_.

El programador es responsable de manejar el _heap_, por lo que pueden producirse los siguientes problemas:

- _Fragmentacion_: quedan espacios vacios en memoria, luego de un uso descontrolado de _malloc_ y _free_.
- _Memory Leaks_: quedan espacios en memoria, que no pueden reutilizar, si se hizo un _malloc_ y se olvido de hacer el _free_ correspondiente.
- _Liberacion antes de tiempo_: si se hace un _free_ antes de utilizar el espacio pedido por _malloc_.

Es posible reutilizar memoria con _realloc_.

Tiene como desventaja, una ejecucion es mas lenta.

Los objetos de _FreeRTOS_ vistos hasta ahora (tareas, semaforos, colas, mutex, etc), se almacenan dinamicamente en el _heap_.

Solo es posible crear tareas de forma estatica con las funciones:

- **xTaskCreateStatic**
- **xTaskCreateStaticPinnedToCore**

![Memoria](/ej28-memoria/memoria.png "Memoria")

NOTA: Utilizar _vPortMalloc_ y _vPortFree_ en lugar de _malloc_ y _free_.

## Obtener el bloque de mayor tamaño disponible de DRAM en ESP-IDF

1. Crear una variable llamada _d_ram_free_ del tipo _size_t_.
2. Llamar a la funcion _heap_caps_get_largest_free_block_.
3. Pasarle como parametro _MALLOC_CAP_8BIT_.
4. Guargar el valor retornado en _d_ram_free_.

## Obtener del espacio libre en el stack de una tarea en FreeRTOS

1. Crear una variable llamada _stack_mem_ del tipo _size_t_.
2. Llamar a la funcion _uxTaskGetStackHighWaterMark_.
3. Pasarle como parametro _NULL_.
4. Guargar el valor retornado en _stack_mem_.

## Obtener el tamaño del heap en FreeRTOS

1. Crear una variable llamada _free_heap_ del tipo _size_t_.
2. Llamar a la funcion _xPortGetFreeHeapSize_.
3. Guargar el valor retornado en _free_heap_.

## Obtener el minimo tamaño historico del heap en FreeRTOS

1. Crear una variable llamada _min_free_heap_ del tipo _size_t_.
2. Llamar a la funcion _xPortGetMinimumEverFreeHeapSize_.
3. Guargar el valor retornado en _min_free_heap_.

## Obtener el tamaño de la SRAM externa en ESP-IDF

1. Crear una variable llamada _spi_ram_ del tipo _size_t_.
2. Llamar a la funcion _heap_caps_get_free_size_.
3. Pasarle como parametro _MALLOC_CAP_SPIRAM_.
4. Guargar el valor retornado en _spi_ram_.

NOTA: Usar la funcion _heap_caps_malloc_ en lugar de _vPortMalloc_ para reservar memoria externa dinamicamente.
