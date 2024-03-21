# Ejemplo 18: Modularizacion basica en ESP-IDF

## Introduccion

Veremos como separar nuestro programa en varios archivos _.h_ y _.c_ para:

- Favorecer la lectura del programa. Si nos fijamos, los ultimos 2 ejemplos llegan a 200 lineas de codigo o mas.
- Tener portabilidad.
- Poder reutilizar codigo.

Los _.c_ tendrán un contenido de caracter privado.

Los _.h_ nos permitiran acceder a las funciones en los _.c_. Es decir, representan lo publico.

Asi, el conjunto de archivos estara dentro de una carpeta, conformando lo que es un modulo. Permitiendonos, acceder a ciertas funciones declaradas en el _.h_

En el ejemplo, crearemos 3 tareas utilizando la misma funcion. Cuya definicion, estara en el modulo llamado _task_function_ y tendra su mismo nombre.

La _task_function_, recibira un puntero a una estructura. Y, a traves del puntero, obtendremos los datos que nos permitiran reutilizar el codigo para:

- Hacer parpadear un led o dejarlo fijo.
- Leer un pulsador y cambiar el comportamiento del led.

NOTA: Repasar las directivas _#ifndef_, _#define_, y _#endif_ y su uso como guarda en los archivos _.h_.

NOTA: Repasar el uso de la palabra reservada _static_ tanto en variables como en funciones.

NOTA: Repasar estructuras en C.

NOTA: No usar _extern_ porque rompe el concepto de encapsulamiento de los modulos.

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

Este es un ejemplo integrador porque:

- Utilizamos tareas de FreeRTOS y les pasamos parametros a las mismas.
- Particionamos nuestro codigo y reducimos su tamaño mediante la modularizacion.
- Repasamos el uso de las GPIO.
- Repasamos _C_ usando: directivas del precompilador, _typedef_, _enum_, _arrays_, y _struct_.
- Repasamos funciones de _get_ (muy similares a las de _set_) para mantener el encapsulamiento.

NOTA: Otra forma mas util de generar modulos, por ejemplo, en el caso de querer distribuir codigo es utilizar _components_. El caso tipico, es el desarrollo de un driver para un periferico externo.
