# Ejemplo 14: Medicion de frecuencia utilizando el modulo PCNT en ESP-IDF

## Introduccion

Hay dos metodos basicos para medir frecuencia:

- Conectar la señal a medir a una _GPIO_, y por interrupciones, contar el numero de pulsos durante un cierto tiempo.
- Contar los pulsos por hardware utilizando el modulo _PCNT_.

En este caso, utilizaremos la segunda opcion.

El modulo _PCNT_, permite contar flacos de subida y/o bajada de una señal de entrada.

La idea, es que en una deteccion se pueda: incrementar, decrementar, o no hacer nada con un contador.

En este ejemplo, la señal a medir, sera del tipo _PWM_ y se generara mediante el modulo _LEDC_.

El ancho del pulso de la misma, se podra variar con un potenciometro conectado a un canal de un _ADC_.

Esto, nos permitira ver como se afecta el resultado de la medición al hacer variaciones del ciclo de actividad manteniendo fija la frecuencia de salida.

Para ello, se crearan 2 tareas:

- La primera cumplira la funcion de generador utilizando el _ADC_ y el _PWM_. Y, correra en el _PRO_CPU_NUM_.
- La segunda cumplira la funcion de frecuencimetro utilizando el _PCNT_. Y, correra en el _APP_CPU_NUM_.

Bibliotecas a incluir:

- **driver/pulse_cnt.h**

NOTA: Otro posible uso del modulo _PCNT_, es leer un encoder rotativo. Para lo cual, se necesita una segunda _GPIO_ que detecte cambios de nivel.

## Configuracion de una unidad PCNT

1. Crear una variable llamada _unit_config_ del tipo _pcnt_unit_config_t_:
   . El miembro _high_limit_ debe valer 1000 (mayor al numero de pulsos a contar).
   . El miembro _low_limit_ debe valer -1000 (debe ser negativo).
   . El miembro _intr_priority_ debe valer 0 para que la interrupcion de la unidad _PCNT_ se asigne automaticamente.
2. Crear una variable llamada _pcnt_unit_ del tipo _pcnt_unit_handle_t_ e inicializarla en _NULL_.
3. Llamar a la funcion _pcnt_new_unit_.
4. Pasarle como primer parametro un puntero a _unit_config_.
5. Pasarle como segundo parametro un puntero a _pcnt_unit_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Configuracion de un canal de la unidad PCNT

1. Crear una variable llamada _entrada_frecuencimetro_ y del tipo const gpio_num_t.
2. Asignale como valor el numero de _GPIO_ con el enum _GPIO_NUM_27_.
3. Crear una variable llamada _chan_config_ del _tipo pcnt_chan_config_t_:
   . El miembro _edge_gpio_num_ debe valer _entrada_frecuencimetro_.
   . El miembro _level_gpio_num_ debe valer -1 porque no lo utilizamos.
4. Crear una variable llamada _pcnt_chan_ del tipo _pcnt_channel_handle_t_ e inicializarla en _NULL_.
5. Llamar a la funcion _pcnt_new_channel_.
6. Pasarle como primer parametro _pcnt_unit_.
7. Pasarle como segundo parametro un puntero a _chan_config_.
8. Pasarle como tercer parametro un puntero a _pcnt_chan_.
9. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: _level_gpio_num_ se usa para indicar en que GPIO se realizara la deteccion de cambios de nivel.

## Configuracion de la acciones del canal de la unidad PCNT

1. Llamar a la funcion _pcnt_channel_set_edge_action_.
2. Pasarle como primer parametro _pcnt_chan_.
3. Pasarle como segundo parametro _PCNT_CHANNEL_EDGE_ACTION_INCREASE_. Esta, es la accion correspondiente al flanco ascendente.
4. Pasarle como tercer parametro _PCNT_CHANNEL_EDGE_ACTION_HOLD_. Esta, es la accion correspondiente al flanco descendente.
5. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: Se debe llamar a _pcnt_channel_set_level_action_ de forma similar a _*pcnt_channel_set_edge_action*_ si detectamos de forma adicional cambios por nivel. Sino, no es necesario.

NOTA: Se pueden definir watchpoints, para generar una callback cuando se alcanza un determinado valor de conteo. En este caso, no se utiliza.

## Configuracion del filtro de glitch

Ignora un flanco, si la duracion del pulso es menor al tiempo configurado en el filtro considerandolo ruido.

1. Crear una variable llamada _filter_config_ del tipo _pcnt_glitch_filter_config_t_:
   . El miembro _max_glitch_ns_ debe valer 1000.
2. Llamar a la funcion _pcnt_unit_set_glitch_filter_.
3. Pasarle como primer parametro _pcnt_unit_.
4. Pasarle como segundo parametro un puntero a _filter_config_.
5. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Habilitar la unidad PCNT

1. Llamar a la funcion _pcnt_unit_enable_.
2. Pasarle como parametro _pcnt_unit_.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Inicializar la unidad PCNT

1. Llamar a la funcion _pcnt_unit_start_.
2. Pasarle como parametro _pcnt_unit_.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Reiniciar la unidad PCNT a 0

1. Llamar a la funcion _pcnt_unit_clear_count_.
2. Pasarle como parametro _pcnt_unit_.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Leer el valor de la unidad PCNT

1. Crear una variable llamada _pulse_count_ del tipo _int_ e inicializarla en 0.
2. Llamar a la funcion _pcnt_unit_get_count_.
3. Pasarle como primer parametro _pcnt_unit_.
4. Pasarle como segundo parametro un puntero a _pulse_count_.

## Conclusiones

Este ejemplo es integrador porque:

- Generamos una señal del tipo _PWM_.
- Usamos el _ADC_.
- Medimos frecuencia con el modulo _PCNT_.
- Repasamos el uso de tareas de _FreeRTOS_.
