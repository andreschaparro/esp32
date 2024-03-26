# Ejemplo 25: Uso de un hardware timer en ESP-IDF

## Introduccion

FreeRTOS provee software timers como se vio en el ejemplo 10.

Pero estos timers, tienen limitaciones:

- Duracion del tick.
- Baja prioridad de las funciones de callback, que en un entorno apropiativo, pueden producir que no se cumplan las temporizaciones con la presicion necesaria.

Los timers por hardware, o hardware timers, o high resolution timer, o _Esp Timer_ no tienen estas limitaciones. Por ejemplo, permiten definir tiempos en microsegundos.

Se pueden crear dos tipos de timers:

- Periodico.
- Oneshot.

En el ejemplo, crearemos un timer periodico que se dispare cada 0.5s. Luego, con otro del tipo one-shot cambiaremos el periodo a 1s, pasados 5 segundos del arranque del _ESP32_.

Bibliotecas a incluir:

- **esp_timer.h**

NOTA: las funciones de callback devuelven _void_ y reciben un _void\* args_.

## Creacion del timer periodico

1. Crear una variable llamada _periodic_timer_args_ del tipo _esp_timer_create_args_t_.
2. Inicializar su campo _.callback_ en _periodic_timer_callback_. Siendo esta, la funcion de callback.
3. Inicializar su campo _.name_ en _periodic_.
4. Crear una variable llamada _periodic_timer_ del tipo _esp_timer_handle_t_.
5. Llamar a la funcion _esp_timer_create_.
6. Pasarle como primer parametro un puntero a _periodic_timer_args_.
7. Pasarle como segundo parametro un puntero a _periodic_timer_.
8. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: En este ejemplo, el campo _.arg_ de _periodic_timer_args_ no se utiliza.

## Creacion del timer onshot

Es similar al timer periodico.

Pero cambia el nombre de la variable _periodic_timer_args_ por _oneshot_timer_args_ y _periodic_timer_ por _oneshot_timer_.

Ademas, el campo _.arg_ de _oneshot_timer_args_ debe valer _(void \*)periodic_timer_ para poder modificar el timer periodico desde el oneshot.

## Arrancar el timer periodico

1. Llamar a la funcion _esp_timer_start_periodic_.
2. Pasarle como primer parametro _periodic_timer_.
3. Pasarle como segundo parametro _500000_. Que es, el periodod en us.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Arrancar el timer oneshot

1. Llamar a la funcion _esp_timer_start_once_.
2. Pasarle como primer parametro _oneshot_timer_.
3. Pasarle como segundo parametro _5000000_. Que es, el tiempo que debe transcurrir para ejecutar la callback.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Detener un timer

1. Llamar a la funcion _esp_timer_stop_.
2. Pasarle el nombre de la variable del tipo _esp_timer_handle_t_ correspondiente a ese timer.
3. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
