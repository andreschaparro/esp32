# Ejemplo 26: Menu con opciones al momento de compilar utilizando Kconfig en ESP-IDF

## Introduccion

_Kconfig_ nos permite, mediante la terminal, tener opciones que modifiquen el resultado del proceso de compilacion.

Las opciones, nos permitiran cambiar valores booleanos, enteros, etc.

Para poder aprobechar esta caracteristica, necesitaremos crear un archivo llamado _Kconfig.projbuild_. Y luego, completar su contenido siguiente una serie de reglas.

La idea, es crear un menu de opciones de compilacion.

NOTA: El _Intellisense_ de _VCS_ nos ayudara.

## Creacion del archivo

1. Dentro de la carpeta _main_ del proyecto crear un archivo llamado _Kconfig_ con extension _projbuild_.

## Reglas del archivo Kconfig

El menu dentro del archivo se define por tabulaciones.

- El tag _menu_ nos permite poner un titulo al menu.
- El archivo debe cerrarse con el tag _endmenu_.
- El tag _config_ nos permite definir un valor que luego sera reemplazado en el archivo _.c_. Por ejemplo, si completamos _config_ _MY_BOOL_ en el codigo, deberemos colocar _CONFIG_MY_BOOL_ en los sitios donde queremos que se sustituya el valor al momento de la compilacion. Este tag, va al mismo nivel que _menu_.
- Dentro de _config_ mas un tabulador, colocamos el tag con el tipo de dato u opciones que deseamos:

  - _bool_ mas un texto de ayuda.
  - _string_ mas un texto de ayuda.
  - _int_ mas un texto de ayuda.
  - _default_ permite poner un valor defecto en caso de solo presionar _ENTER_ y saltar la opcion.
  - _range_ se aplica a _int_ y nos evita ingresar valores invalidos.
  - _choise_ es para elegir una opcion de varias. Al final, debera cerrarse con un tag _endchoise_.
  - _help_ permite ingresar un texto de ayuda adicional. Pero, va con dos tabuladores mas que _config_. Y luego el texto, con tres tabuladores y sin comillas.

Es importante en el _.c_ utilizar las directivas de precompilador como _#ifdef_, _#elif_, _#else_ y _#endif_ para evitar errores.

Para mas informacion revisar la documentacion oficial de _Kconfig_:

https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt

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
