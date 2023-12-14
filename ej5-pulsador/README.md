# Leer un pulsador en ESP-IDF

Para definir e inicializar la entrada donde esta conectado el pulsador:

1. Crear una variable llamada _pulsador_ y del tipo _const gpio_num_t_.
2. Asignale como valor el numero de GPIO con el enum **GPIO_NUM_32**.
3. Llamar a la funcion _gpio_set_direction()_.
4. Pasarle como primer parametro la variable _pulsador_.
5. Pasarle como segundo parametro el enum **GPIO_MODE_INPUT**.
6. Llamar a la funcion _gpio_pullup_en()_ para habilitar el pullup o a _gpio_pullup_dis()_ para deshabilitarlo.
7. Pasarle como parametro la variable _pulsador_.
8. Llamar a la funcion _gpio_pulldown_dis()_ para habilitar el pulldown o a _gpio_pulldown_dis()_ para deshabilitarlo.
9. Pasarle como parametro la variable _pulsador_.

Para leer el estado del pulsador:

1. Llamar a la funcion _gpio_get_level_.
2. Pasarle como primer parametro la variable _pulsador_.
3. Va a devolver un 0 y un 1.

Luego, se invertira el estado del led cada vez que se presione el pulsador.

Como se evidenciara en la realidad, aparece el efecto de los rebotes del pulsador. Basicamente, el led se encendera y apagara mas de una vez por cada vez que pulsemos. Mostrando, un comporamiento diferente cada vez que pulsemos.
