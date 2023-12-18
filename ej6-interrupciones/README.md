# Ejemplo 6: Interrupciones con FreeRTOS

Vamos a utilizar el pulsador para generar una interrupcion e imprimir el tick siguiente a cuando se produce.

Como se evidenciara en la realidad, aparece el efecto de los rebotes del pulsador.

## Configuracion

Primero, debemos configurar la entrada para que genere una interrupcion:

1. Llamar a la funcion _gpio_set_intr_type()_.
2. Pasarle como primer parametro la variable _pulsador_.
3. Pasarle como segundo **GPIO_INTR_NEGEDGE** que corresponde a cuando queremos que se produzca la interrupcion:

   - Para el flanco ascendente _GPIO_INTR_POSEDGE_.
   - Para el flanco descendente _GPIO_INTR_NEGEDGE_,
   - Para ambos flancos _GPIO_INTR_ANYEDGE_.

4. Llamar a la funcion _gpio_install_isr_service()_ para habilitar el servicio que permite tener un una funcion atencion de interrupcion o ISR diferente para cada entrada.
5. Pasarle como parametro, un _define_ llamado _ESP_INTR_FLAG_DEFAULT_ que valga **0**.
6. Llamar a la funcion _gpio_isr_handler_add()_.
7. Pasarle como primer parametro la variable _pulsador_.
8. Pasarle como segundo parametro el nombre del ISR. En este caso, _pulsador_isr_handler_.
9. Parasle como tercer parametro, y de forma opcional, un puntero a una variable la cual necesitemos dentro de la ISR. O como en este caso, pasarle **NULL**.

NOTA 1: La ISR debera tener como argumento _void \*args_.
NOTA 2: La ISR debera tener como atributo _IRAM_ATTR_ en su declaracion.

## Funciones de FreeRTOS dentro de una ISR

Las funciones de FreeRTOS, como las que vimos en el ejemplo 4 de tareas, tienen su variante cuando se utilizan dentro de una ISR. Por ejemplo, _xTaskResumeFromISR()_.

Por el momento, no verificaremos el valor que retorna _xTaskResumeFromISR()_. Mas adelante, veremos sincronizacion de taras y los problemas del uso de esta funcion.

## Manejo de errores en ESP-IDF

La mayoria de las funciones de ESP-IDF devuelven una variable del tipo _esp_err_t_.

Su valor, debe ser chequeado con la macro _ESP_ERROR_CHECK()_. Esta macro, funciona de forma similar a _assert_, pero no verifica condiciones booleanas, sino los valores de _esp_err_t_.

Otra opcion, es preguntar con un if si es el valor retornado es igual a **ESP_OK**, y en caso contrario, ejectuar alguna rutina.
