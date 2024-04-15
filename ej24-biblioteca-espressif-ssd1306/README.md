# Ejemplo 24: Utilizar la biblioteca de componentes mantenidos por Espressif para utilizar el display OLED SSD1306 en ESP-IDF

## Introduccion

En este ejemplo, vamos a escribir _Hola Mundo_ en una pantalla _OLED SSD1306_.

La misma, tiene comunicacion _I2C_ y es de _0.91"_ con una resolucion de _128x32_ pixeles.

## Comunicacion I2C

Es un bus de comunicacion serie, pensado para interconectar perifericos externos al microcontrolador, con las siguientes caracteristicas:

- Multi-dispositivo.
- Half duplex. Osea, una linea de datos bidireccional llamada _SDA_ del tipo _open drain_.
- Sincronico. Osea, hay una linea de clock llamada _SCL_.
- Necesita _resistencias de pullup_ en _SDA_ y _SCL_.
- Tiene dos velocidades de clock: _Standard-mode_ de 100Khz y _Fast-mode_ de 400Khz.
- Los dispositivos _I2C_ pueden tener una direccion de 7 o 10 bits.

![Conexion I2C](/ej24-biblioteca-espressif-ssd1306/conexion_i2c.png "Conexion I2C")

![Comunicacion I2C](/ej24-biblioteca-espressif-ssd1306/comunicacion_i2c.png "Comunicacion I2C")

El ESP32 tiene 2 controladores o ports _I2C_: _I2C_NUM_0_ y _I2C_NUM_1_.

NOTA: Las _resistencias de pull-up externas_, deben estar entre los 2 y 5 Kohms. Asi, si estamos en _Fast-mode_ el valor de resistencia sera menor al de _Standard-mode_.

El display _OLED SSD1306_ tiene comunicacion _I2C_.

Bibliotecas a incluir:

- **esp_lcd_panel_io.h**
- **esp_lcd_panel_ops.h**
- **esp_lcd_panel_vendor.h**
- **driver/i2c.h**
- **esp_lvgl_port.h**
- **lvgl.h**

## Configuracion del bus I2C

1. Crear una variable llamada _i2c_conf_ del tipo _i2c_config_t_:

   - El miembro _.mode_ debe valer _I2C_MODE_MASTER_.
   - El miembro _.sda_io_num_ debe valer _GPIO_NUM_21_.
   - El miembro _.scl_io_num_ debe valer _GPIO_NUM_22_.
   - El miembro _.sda_pullup_en_ debe valer _GPIO_PULLUP_ENABLE_.
   - El miembro _.scl_pullup_en_ debe valer _GPIO_PULLUP_ENABLE_.
   - El miembro _.master.clk_speed_ de _conf_ debe valer _100000_.

2. Llamar a la funcion _i2c_param_config_.
3. Pasarle como primer parametro _I2C_NUM_0_.
4. Pasarle como segundo parametro un puntero a _i2c_config_t_.
5. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
6. Llamar a la funcion _i2c_driver_install_.
7. Pasarle como primer parametro _I2C_NUM_0_.
8. Pasarle como segundo parametro _conf.mode_.
9. Pasarle como tercer parametro _0_.
10. Pasarle como cuarto parametro _0_.
11. Pasarle como quinto parametro _0_.
12. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Uso del IDF Component Manager

El _IDF Component Manager_ es una herramienta que descarga _components_ mantenidos por _Espressif_ para nuestros proyectos.

Estos modulos, pueden verse en la pagina *https://components.espressif.com/*.

NOTA: Siempre es preferible utilizar estos modulos, a otros desarrollados por teceros.

1. Buscar en la pagina el que se llama _espressif/esp_lvgl_port_.
2. Elegir la version _1.4.0_.
3. Copiar el comando que se indica en la pagina. En este caso, _idf.py add-dependency "espressif/esp_lvgl_port^1.4.0"_.
4. Pegar el comando copiado en la _ESP-IDF Terminal_ en el paso 3.
5. Presionar _ENTER_.

NOTA: Ver en la pagina que este _component_ tiene como dependencia a otro llamado _lvgl/lvgl_.

6. Buscar en la pagina el que se llama _lvgl/lvgl_.
7. Elegir la version _8.4.0_.
8. Copiar el comando que se indica en la pagina. En este caso, _idf.py add-dependency "lvgl/lvgl^8.4.0"_.
9. Pegar el comando copiado en la _ESP-IDF Terminal_ en el paso 8.
10. Presionar _ENTER_.
11. Verificar, que se crea un archivo llamado _idf_component.yml_ dentro de la carpeta _main_.
12. Verificar, que dentro del archivo _idf_component.yml_ esten ambos modulos como _dependencies_.
13. Hacer un _ESP-IDF: Full Clean_.
14. Volver a compilar el proyecto.
15. Verificar, que se crea una carpeta llamada _managed_components_ dentro del directio del proyecto.
16. Verificar, que se crea una carpeta llamada _espressif\_\_esp_lvgl_port_ dentro de _managed_component_.
17. Verificar, que se crea una carpeta llamada _lvgl\_\_lvgl_ dentro de _managed_component_.

Estos dos _components_, nos permitiran utilizar el controlador para pantallas _LCD_, que viene incluido en el _ESP-IDF_, con el display _OLED SSD1306_.

## Configuracion de la vinculacion del bus I2C con el controlador de LCD

1. Crear una variable llamada _io_handle_ del tipo _esp_lcd_panel_io_handle_t_.
2. Inicializar _io_handle_ en _NULL_.
3. Crear una variable llamada _io_config_ del tipo _esp_lcd_panel_io_i2c_config_t_:

   - El miembro _.dev_addr_ debe valer _0x3C_ segun el datasheet del _OLED SSD1306_ pagina 20.
   - El miembro _.control_phase_bytes_ debe valer _1_ segun el datasheet del _OLED SSD1306_ pagina 20.
   - El miembro _.lcd_cmd_bits_ debe valer _8_ segun el datasheet del _OLED SSD1306_ pagina 20.
   - El miembro _.lcd_param_bits_ debe valer _8_ segun el datasheet del _OLED SSD1306_ pagina 20.
   - El miembro _.dc_bit_offset _ debe valer _6_ segun el datasheet del _OLED SSD1306_ pagina 20.

4. Llamar a la funcion _esp_lcd_new_panel_io_i2c_.
5. Pasarle como primer parametro _I2C_NUM_0_ casteado como _esp_lcd_i2c_bus_handle_t_.
6. Pasarle como segundo parametro un puntero a _io_config_.
7. Pasarle como tercer parametro un puntero a _io_handle_.
8. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Configuracion del controlador de LCD para el OLED SSD1306

1. Crear una variable llamada _panel_handle_ del tipo _esp_lcd_panel_handle_t_.
2. Inicializar _panel_handle_ en _NULL_.
3. Crear una variable llamada _panel_config_ del tipo _esp_lcd_panel_dev_config_t_:

   - El miembro _.bits_per_pixel_ debe valer _1_.
   - El miembro _.reset_gpio_num_ debe valer _-1_.

4. Llamar a la funcion _esp_lcd_new_panel_ssd1306_.
5. Pasarle como primer parametro _io_handle_.
6. Pasarle como segundo parametro un puntero a _panel_config_.
7. Pasarle como tercer parametro un puntero a _panel_handle_.
8. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
9. Llamar a la funcion _esp_lcd_panel_reset_.
10. Pasarle como parametro _panel_handle_.
11. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
12. Llamar a la funcion _esp_lcd_panel_init_.
13. Pasarle como parametro _panel_handle_.
14. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
15. Llamar a la funcion _esp_lcd_panel_disp_on_off_.
16. Pasarle como primer parametro _panel_handle_.
17. Pasarle como segundo parametro _true_.
18. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Configuracion de la biblioteca grafica LVGL para el OLED SSD1306

1. Crear una constante llamada _lvgl_cfg_ del tipo _lvgl_port_cfg_t_ e inicializarla llamando a la macro _ESP_LVGL_PORT_INIT_CONFIG()_.
2. Llamar a la funcion _lvgl_port_init_.
3. Pasarle como parametro un puntero a _lvgl_cfg_.
4. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
5. Crear una constante llamada _disp_cfg_ del tipo _lvgl_port_display_cfg_t_:

   - El miembro _.io_handle_ debe valer _io_handle_.
   - El miembro _.panel_handle_ debe valer _panel_handle_.
   - El miembro _.buffer_size_ debe valer _128 \* 64_, que corresponde al numero de pixeles de ancho por alto.
   - El miembro _.double_buffer_ debe valer _true_
   - El miembro _.hres_ debe valer _128_ que corresponde al numero de pixeles de ancho.
   - El miembro _.vres_ debe valer _64_ que corresponde al numero de pixeles de alto.
   - El miembro _.monochrome_ debe valer _true_.
   - Dentro del miembro _.rotation_:
     - El sub-miembro _swap_xy_ debe valer _false_.
     - El sub-miembro _mirror_x_ debe valer _false_.
     - El sub-miembro _mirror_y_ debe valer _false_.

6. Crear una variable llamada _disp_ del tipo puntero a _lv_disp_t_.
7. Llamar a la funcion _lvgl_port_add_disp_.
8. Pasarle como parametro un puntero a _disp_cfg_.
9. Guardar el valor retornado en _disp_.
10. Llamar a la funcion _lv_disp_set_rotation_.
11. Pasarle como primer parametro _disp_.
12. Pasarle como segundo parametro _LV_DISP_ROT_NONE_.
13. Crear una variable llamada _src_ del tipo puntero a _lv_obj_t_.
14. Llamar a la funcion _lv_disp_get_scr_act_.
15. Pasarle como parametro _disp_.
16. Guardar el valor retornado en _src_.

NOTA: Se utiliza como alto 64 pixels, aunque tenga 32 el display, porque sino queda la memoria del _SSD1306_ con basura y rellena el display con pixeles en blanco de forma aleatoria en la parte baja.

## Crear un label de texto con la biblioteca grafica LVGL en el OLED SSD1306

1. Crear una variable llamada _label_ del tipo puntero a _lv_obj_t_.
2. Llamar a la funcion _lv_label_create_.
3. Pasarle como parametro _src_.
4. Guardar el valor retornado en _label_.
5. Llamara a la funcion _lv_label_set_long_mode_.
6. Pasarle como primer parametro _label_.
7. Pasarle como segundo parametro _LV_LABEL_LONG_SCROLL_CIRCULAR_.
8. Llamar a la funcion _lv_label_set_text_.
9. Pasarle como primer parametro _label_.
10. Pasarle como segundo parametro _"Texto a mostrar..."_.
11. Llamar a la funcnion _lv_obj_set_width_.
12. Pasarle como primer parametro _label_.
13. Pasarle como segundo parametro _disp->driver->hor_res_.
14. Llamar a la funcion _lv_obj_align_.
15. Pasarle como primer parametro _label_.
16. Pasarle como segundo parametro _LV_ALIGN_TOP_MID_.
17. Pasarle como tercer parametro el offset en x. En este caso, vale _0_.
18. Pasarle como cuarto parametro el offset en y. En este caso, vale _0_.

Para mas informacion sobre como utilizar la biblioteca _LVGl_ ir a su sitio web:

https://lvgl.io/

## Conclusiones

Este ejemplo es integrador porque:

- Utilizamos la comunicacion _I2C_.
- Utilizamos el _IDF Component Manager_ y los modulos de _Espressif_ disponiles en la pagina *https://components.espressif.com/*.
- Utilizamos el controlador para pantallas _LCD_ del _ESP-IDF_ con el display _OLED SSD1306_.
- Utilizamos la biblioteca grafica LVGL para escribir texto con el display _OLED SSD1306_.

Este ejemplo puede parecer muy complicado, pero a futuro sera copiar y pegar. Y, se puede modularizar a futuro exclusivamente para el _OLED SSD1306_...

NOTA: las funciones de la biblioteca _LVGL_ no son _thread-safe_ por lo que se proveen dos funciones que internamente implementan un mutex para solucionar el problema. La funcion _lvgl_port_lock(0)_, debe usarse como argumento de un _if_. Si se verifica que se puede tomar el mutex se modifica el contenido del display. Luego, debe liberarse el mutex con la funcion _lvgl_port_unlock()_.
