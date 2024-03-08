# Ejemplo 13: Uso de gpio_config_t para inicializar varias GPIO en simultaneo

En el ejemplo, el pulsador y el led se inicializan utilizando el tipo gpio_config_t (es una estructura). Este metodo, permite inicialziar varias entradas y varias salidas al mismo tiempo.

Luego, se muestra como utilizar una misma ISR para varias GPIO de entrada. Esto es, mediante el pasaje de parametros a una ISR.

El ejemplo no suma ningun concepto nuevo, solo es una forma de hacer lo mismo que ya haciamos con menos lineas de codigo (en algunos casos...).

Tambien, se repasan los temas: tareas y colas.

## Configurar varias entradas en simultaneo

1. Crear una variable llamada _io_cfg_ del tipo _gpio_config_t_:
   - El miembro _intr_type_ debe valer _GPIO_INTR_NEGEDGE_ si deseamos que todas las GPIO generen una interrupcion. En caso contrario, poner _GPIO_INTR_DISABLE_ para que ninguna genere una interrupcion.
   - El miembro _mode_ debe vale _GPIO_MODE_INPUT_.
   - El miembro _pull_down_en_ debe valer _GPIO_PULLDOWN_DISABLE_.
   - El miembro _pull_up_en_ debe valer _GPIO_PULLUP_ENABLE_.
   - El miembro _pin_bit_mask_ deve valer _(1ULL << pulsador)_. La misma, es una expresiones que mediante el operador _OR_ permite afectar mas de una entrada al mismo tiempo.
2. Llamar a la funcion _gpio_config_.
3. Pasarle como primer parametro un puntero a _io_cfg_.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Configurar varias salidas en simultaneo

No hace falta crear otra variable, puede reutilizarse \_io_cfg.

1. El miembro _intr_type_ de _io_cfg_ deve valer _GPIO_INTR_DISABLE_.
2. El miembro _mode_ de _io_cfg_ deve valer _GPIO_MODE_OUTPUT_.
3. El miembro _pull_up_en_ de _io_cfg_ deve valer _GPIO_PULLUP_DISABLE_.
4. El miembro _pin_bit_mask_ de _io_cfg_ deve valer _(1ULL << led)_. La misma, es una expresiones que mediante el operador _OR_ permite afectar mas de una salida al mismo tiempo.
5. Llamar a la funcion _gpio_config_.
6. Pasarle como primer parametro un puntero a _io_cfg_.
7. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Pasaje de parametros a una ISR

1. Al llamar a la funcion _gpio_isr_handler_add_ pasarle como tercer parametro _(void \*)pulsador_.
2. Dentro de la ISR, crear una variable llamada _num_gpio_ del tipo _uint32_t_.
3. Guardar el valor de _args_ casteado como _uint32_t_ en _num_gpio_.

Asi, cuando una gpio sea presionada tendremos su numero disponible eb la ISR. De modo, que solo basta con escribir una sola ISR.
