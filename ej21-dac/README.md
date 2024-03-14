# Ejemplo 21: Uso de los DAC del ESP32 en ESP-IDF

## Introduccion

El ESP32 tiene 2 modulos DAC de 8 bits.

El primero conectado a la _GPIO25_.

El segundo conectado a la _GPIO26_.

Ambos DAC del ESP32 puede generar tensiones de 0 a 3,3V, que corresponden con los codigos 0 y 255.

En el ejemplo, crearemos una señal tipo diente de sierra con el DAC y la capturaremos con el ADC.

Bibliotecas a incluir:

- _driver/dac_oneshot.h_

## Inicializacion

1. Crear una variable llamada _chan0_handle_ del tipo _dac_oneshot_handle_t_.
2. Crear una variable llamada _chan0_cfg_ del tipo _dac_oneshot_config_t_:
   - El miembro _chan_id_ debe valer _DAC_CHAN_0_.
3. Llamar a la funcion _dac_oneshot_new_channel_.
4. Pasarle como primer parametro un puntero a _chan0_cfg_.
5. Pasarle como segundo parametro un puntero a _chan0_handle_.
6. Verificar el valor retornado con la macro ESP_ERROR_CHECK.

## Escribir un valor de 0 a 255

1. Llamar a la funcion _dac_oneshot_output_voltage_.
2. Pasarle como primer parametro _chan0_handle_.
3. Pasarle como segundo parametro el valor.
4. Verificar el valor retornado con la macro ESP_ERROR_CHECK.

## Conclusiones

Este, es un ejemplo integrador porque:

- Utilizamos tareas y semaforos binarios de _FreeRTOS_.
- Repasamos el uso del _ADC_.
- Incorporamos la utilizacion del _DAC_.
