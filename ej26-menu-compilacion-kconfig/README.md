# Ejemplo 26: Menu con opciones al momento de compilar utilizando Kconfig en ESP-IDF

## Introduccion

_Kconfig_ nos permite, mediante la terminal, tener un menu con opciones que modifiquen el resultado del proceso de compilacion.

Las opciones, nos permitiran cambiar valores booleanos, enteros, y strings entre otros. Siempre, sin tocar el codigo.

![Menu de opciones de compilacion](/ej26-menu-compilacion-kconfig/menuconfig.png "Menu de opciones de compilacion")

Para poder aprobechar esta caracteristica, necesitaremos crear un archivo llamado _Kconfig.projbuild_.

Luego, hay que completar su contenido siguiendo una serie de reglas.

NOTA: El _Intellisense_ de _VCS_ nos ayudara.

## Creacion del archivo

1. Dentro de la carpeta _main_ del proyecto crear un archivo llamado _Kconfig_ con extension _projbuild_.

## Reglas del archivo Kconfig.projbuild

El menu dentro del archivo, se define por tabulaciones y anidamiento de tags.

- El tag _menu_, nos permite poner un titulo a nuestro menu.
- El archivo debe cerrarse con el tag _endmenu_.
- El tag _config_, nos permite definir una opcion que luego sera reemplazado en los archivos _.c_.
- Por ejemplo, si definimos _config_ _MY_BOOL_, deberemos colocar _CONFIG_MY_BOOL_ en los sitios donde queremos que se sustituya el valor al momento de la compilacion.
- El tag _config_, no tiene tabulaciones respecto de _menu_.
- Los siguientes tags tienen una tabulacion respecto de _config_:

  - _bool_ mas un texto de descripcion.
  - _string_ mas un texto de descripcion.
  - _int_ mas un texto de descripcion.
  - _default_ permite poner un valor defecto.
  - _range_ se aplica a _int_ y nos evita ingresar valores invalidos.
  - _choise_ es para elegir una opcion de varias. Al final, debera cerrarse con un tag _endchoise_.

- Los siguientes tags tienen dos tabulacion respecto de _config_:
  - _help_ que permite agregar un texto de ayuda mas extenso.

Es importante en el _.c_ utilizar las directivas de precompilador como _#ifdef_, _#elif_, _#else_ y _#endif_ para evitar errores.

NOTA: Revisar ejemplos de _Espressif_ para ver mas en detalle su utilizacion. Como por ejemplo _choice_.

## Cambiar las opciones:

1. Abrir una _ESP-IDF Terminal_.
2. Ir al directorio del proyecto.
3. Ejecutar _idf.py menuconfig_.
4. Ingresar a nuestro menu.
5. Completar las opciones.
6. Salir y guardar los cambios.
7. Compilar y descargar el programa.

NOTA: Para ir al directorio de nuestro proyecto, es util copiar la ruta desde la ventana de windows y usar el comando _cd_.

NOTA: Cada vez que se cambie nuestro menu, el proyecto volvera a compilar y tardara bastante tiempo.
