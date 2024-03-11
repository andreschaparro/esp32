# Ejemplo 18: Modularizacion basica

## Introduccion

Veremos como separar nuestro programa en varios archivos _.h_ y _.c_ para:

- Favorecer la lectura del programa. Si nos fijamos, los ultimos 2 ejemplos llegan a 200 lineas de codigo o mas.
- Portabilidad.
- Reutilizacion.

Los _.c_ tendrán un contenido de caracter privado.

Los _.h_ nos permitiran acceder a las funciones en los _.c_. Es decir, representan lo publico.

Asi, el conjunto de archivos estara dentro de una carpeta, conformando lo que es un modulo.

En el ejemplo, crearemos 3 tareas utilizando la misma funcion que estará definida en un modulo llamado _task_function_.

Adicionalmente, veremos como pasar un parametro para distinguir a cada una de las 3 tareas.

NOTA: Repasar el uso de la palabra reservada _static_ tanto en variables como en funciones.

NOTA: Repasar las directivas _#ifndef_, _#define_, y _#endif_ y su uso como guarda en los archivos _.h_.

## Creacion de directorios y archivos

1. Crear una carpeta llamada _task_function_ dentro de la carpeta _main_.
2. Dentro de la carpeta, crear un archivo llamado _task_function.c_.
3. Dentro de la carpeta, crear un archivo llamado _task_function.h_.

## Modificacion de CMakeLists.txt dentro de la carpeta main

1. Dentro de _SRCS_, agregar _"task_function/task_function.c"_.
2. Dentro de _INCLUDE_DIRS_ agregar _"task_function"_.

## Inclusion del modulo al main.c

1. Agregar la directiva _#include "task_function.h"_.

## Conclusiones

- Evitar el uso de _extern_ porque rompe el concepto de encapsulamiento.
- Este metodo, es el mas generico y ofrece compatibilidad con otras plataformas.
- Una forma mas util, en caso de querer distribuir codigo, como por ejemplo el de un driver, es utilizar _components_.
