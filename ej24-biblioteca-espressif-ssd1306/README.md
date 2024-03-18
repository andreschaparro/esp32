# Ejemplo 24: Utilizar la biblioteca de componentes mantenidos por Espressif para utilizar el display OLED SSD1306 en ESP-IDF

## Introduccion

En este ejemplo, vamos a escribir _Hola Mundo_ en una pantalla _OLED SSD1306_ que tiene comunicacion _I2C_ y es de _0.91"_ con una resolucion de _128x32_ pixeles.

## Comunicacion I2C

Es un bus de comunicacion, pensado para interconectar diferentes perifericos externos, con las siguientes caracteristicas:

- Multi-dispositivo.
- Half duplex. Osea, un hilo de datos bidireccional llamado _SDA_.
- Sincronico. Osea, hay un hilo de clock llamado _SCL_.
- Se necesitan _resistencias de pullup_ en ambos hilos. Esto, puede configurarse dentro del microcontrolador.
- Hay dos velocidades de reloj: _Standard-mode_ de 100Khz y _Fast-mode_ de 400Khz.
- Los dispositivos _I2C_ pueden tener una direccion de 7 o 10 bits.

El ESP32 tiene 2 modulos _I2C_.

NOTA: En el _Fast-mode_ se necesitaran _resistencias de pull-up externas_ cuyos valores deberan estar entre los 1 y 10 Kohms. Consultar las hojas de datos.

## Uso del IDF Component Manager

El _IDF Component Manager_ es una herramienta que descarga componentes mantenidos por _Espressif_ para nuestros proyectos.

Estos modulos, pueden verse en la pagina *https://components.espressif.com/*.

NOTA: Siempre es preferible utilizar estos modulos, a otros desarrollados por teceros.

1. Buscar en la pagina el modulo _ssd1306_.
2. Clic en el que se llama _espressif/ssd1306_.
3. _Abrir el ESP-IDF Terminal_.
4. Copiar el comando que se indica en la pagina. En este caso, _idf.py add-dependency "espressif/ssd1306^1.0.5~1"_ y presionar _ENTER_.
5. Verificar, que se crea un archivo llamado _idf_component.yml_ dentro de la carpeta _main_.
6. Verificar, que dentro del archivo _idf_component.yml_ figura el modulo _ssd1306_ como una dependencia.
7. Verificar, que se crea una carpeta llamada _managed_component_ dentro del directio del proyecto.
8. Verificar, que se crea una carpeta llamada _espessif_ssd1306_ dentro de _managed_component_.
9. Verificar, que dentro de _espessif_ssd1306_ estan los archivos fuente del _component_.
10. Clic en el link de _Homepage_ en la pagina para encontrar un ejemplo de como utilizarlo.
11. Volver a compilar el proyecto.

## Configuracion del bus I2C API vieja

1. Crear una variable llamada _conf_ del tipo _i2c_config_t_.
2. El miembro _.mode_ de _conf_ debe valer _I2C_MODE_MASTER_.
3. El miembro _.sda_io_num_ de _conf_ debe valer _GPIO_NUM_21_.
4. El miembro _.sda_pullup_en_ de _conf_ debe valer _GPIO_PULLUP_ENABLE_.
5. El miembro _.scl_io_num_ de _conf_ debe valer _GPIO_NUM_22_.
6. El miembro _.scl_pullup_en_ de _conf_ debe valer _GPIO_PULLUP_ENABLE_.
7. El miembro _.master.clk_speed_ de _conf_ debe valer _100000_.
8. El miembro _.clk_flags_ de _conf_ debe valer _I2C_SCLK_SRC_FLAG_FOR_NOMAL_.
9. Llamar a la funcion _i2c_param_config_.
10. Pasarle como primer parametro _I2C_NUM_0_.
11. Pasarle como segundo parametro un puntero a _conf_.
12. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
13. Llamar a la funcion _i2c_driver_install_.
14. Pasarle como primer parametro _I2C_NUM_0_.
15. Pasarle como segundo parametro _conf.mode_.
16. Pasarle como tercer parametro _0_.
17. Pasarle como cuarto parametro _0_.
18. Pasarle como quinto parametro _0_.
19. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

NOTA: Este modulo, utiliza el **i2c.h** que no esta recomendado para el desarrollo de nuevos drivers. Mas adelante, veremos como utilizar la nueva _API_.

NOTA: Las 2 _APIs_ de _i2c_ no pueden usarse en simultaneo en el mismo proyecto.

## Configuracion del SSD1306

1. Crear una variable llamada _ssd1306_dev_ del tipo _ssd1306_handle_t_.
2. Iniciarlizar _ssd1306_dev_ en _NULL_.
3. Llamar a la funcion _ssd1306_create_.
4. Pasarle como primer parametro _I2C_NUM_0_.
5. Pasarle como segundo parametro _SSD1306_I2C_ADDRESS_.
6. Asigar el valor retornado por _ssd1306_create_ en _ssd1306_dev_.
7. Llamar a la funcion _ssd1306_refresh_gram_.
8. Pasarle como primer parametro _ssd1306_dev_.
9. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Limpiar el contenido del SSD1306

1. Llamar a la funcion _ssd1306_clear_screen_.
2. Pasarle como primer parametro _ssd1306_dev_.
3. Pasarle como segundo parametro _0x00_.
4. Llamar a la funcion _ssd1306_refresh_gram_.
5. Pasarle como primer parametro _ssd1306_dev_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Escribir el contenido del SSD1306

1. Llamar a la funcion _ssd1306_draw_string_.
2. Pasarle como primer parametro _ssd1306_dev_.
3. Pasarle como segundo parametro la posicion del cursor en _X_. En este caso _0_.
4. Pasarle como tercer parametro la posicion del cursor en _Y_. En este caso _16_.
5. Pasarle como cuarto parametro el nombre de un _array_ de _chars_ casteado como _(const uint8_t \*)_.
6. Pasarle como quinto parametro la altura del caracter. En este caso _16_.
7. Pasarle como sexto parametro _0_.
8. Llamar a la funcion _ssd1306_refresh_gram_.
9. Pasarle como primer parametro _ssd1306_dev_.
10. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
