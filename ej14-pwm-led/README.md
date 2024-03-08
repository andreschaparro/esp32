# Ejemplo 13: Control del brillo de un led utilizando PWM con modulo LEDC

## Introduccion

Haremos cambiar el brillo de un led.

Para ello, se dispone del modulo LEDC, que se utiliza para generar señales PWM. Aunque, se puede usar con otros fines.

Una señal _PWM_, es una señal rectangular que tiene un tiempo en 1(3,3V) y otro tiempo en 0(0V). La suma de ambos tiempos, es constante y se llama periodo. La inversa del periodo, se llama frecuencia y se mide en Hz. Finalmente, la relacion entre el tiempo en 1 y el periodo se llama ciclo de actividad y se lo marca en %.

![Señal PWM](/ej14-pwm-led/pwm.png "Señal PWM")

NOTA: Para aplicaciones de control(motor de DC) o medicion(captura) se recomienda utilizar el modulo _MCPWM_ en lugar del _LEDC_.

Bibliotecas a incluir:

- **driver/ledc.h**

## Configuracion del timer del PWM

Hay timers de baja velocidad y de alta velocidad. Y dentro de cada tipo, hay 4 disponibles.

1. Crear una variable llamada _ledc_timer_ del tipo _ledc_timer_config_t_:
   . El miembro _speed_mode_ debe valer _LEDC_LOW_SPEED_MODE_.
   . El miembro _duty_resolution_ debe valer _LEDC_TIMER_13_BIT_.
   . El miembro _timer_num_ debe valer _LEDC_TIMER_0_.
   . El miembro _freq_hz_ debe valer _1000_.
   . El miembro _clk_cfg_ debe valer _LEDC_AUTO_CLK_. Esto, permite elegir automaticamente la fuente de clock en funcion de la relacion entre _freq_hz_ y _duty_resolution_.
2. Llamar a la funcion _ledc_timer_config_.
3. Pasarle como parametro un puntero a _ledc_timer_.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: en caso de una incompatibilidad entre los valores elegidos de _duty_resolution_ y _freq_hz_ se informara un error en el _ESP-IDF Monitor device_.

## Configuracion del canal del PWM

Hay 16 canales disponibles de PWM, 8 de baja velocidad y 8 de alta velocidad. Ahora, se debe asociar el timer configurado previamente a un canal.

1. Crear una variable llamada _led_ y del tipo _const gpio_num_t_.
2. Asignale como valor el numero de _GPIO_ con el enum _GPIO_NUM_2_.
3. Crear una variable llamada _ledc_channel_ del tipo _ledc_channel_config_t_:
   . El miembro _speed_mode_ debe valer lo mismo que configuramos en el timer, en este caso, _LEDC_LOW_SPEED_MODE_.
   . El miembre _channel_ debe valer _LEDC_CHANNEL_0_.
   . El miembro _timer_sel_ debe valer lo mismo que configuramos en el timer, en este caso, _LEDC_TIMER_0_.
   . El miembro _intr_type_ debe valer _LEDC_INTR_DISABLE_.
   . El miembro _gpio_num_ debe valer _led_.
   . El miembro _duty_ debe valer en 0.
   . El miembro _hpoint_ debe valer 0.
4. Llamar a la funcion _ledc_channel_config_.
5. Pasarle como parametro un puntero a _ledc_channel_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Cambiar el ciclo de actividad del canal PWM

1. Llamar a la funcion _ledc_set_duty_.
2. Pasarle como primer parametro lo mismo que configuramos en el timer, en este caso, _LEDC_LOW_SPEED_MODE_.
3. Pasarle como segundo parametro lo mismo que configuramos en el canal, en este caso, _LEDC_CHANNEL_0_.
4. Pasarle como tercer parametro el duty que puede valer de 0 a 2\*\*_duty_resolution_ - 1. Donde, 0 implica un 0% de ciclo de actividad y 2\*\*_duty_resolution_ - 1 implica un 100% de ciclo de actividad.
5. Llamar a la funcion _ledc_update_duty_.
6. Pasarle como primer parametro lo mismo que configuramos en el timer, en este caso, _LEDC_LOW_SPEED_MODE_.
7. Pasarle como segundo parametro lo mismo que configuramos en el canal, en este caso, _LEDC_CHANNEL_0_.
8. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
