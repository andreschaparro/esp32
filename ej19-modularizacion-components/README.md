# Ejemplo 19: Modularizacion con Components en ESP-IDF

## Introduccion

Los _components_ permiten: reutilizar, distribuir, y organizar los modulos de forma mas eficiente. Esto, es una caracteristica del _ESP-IDF_.

El ejemplo, es similar al 18.

Pero, haremos un modulo en forma de _component_ con dos funciones que permitan crear 6 tareas: 3 para los pulsadores, y 3 para los leds.

Por lo que, tendremos 6 tareas funcionando simultaneamente.

NOTA: repasar uso de la palabra reservada _const_.

## Cracion de un componente

1. _CTRL+SHIFT+P_.
2. Clic en _ESP-IDF: Create New ESP-IDF Component_.
3. Colocarle un nombre al _component_.

Ahora, aparecera un directorio _components_ y dentro otro directorio con nuestro _componente_.

Tendra por defecto: un _.c_ y un _.h_.

## Modificacion del CMakeLists.txt para resolver dependencias del modulo

1. Abrir el archivo _CMakeLists.txt_ que esta dentro del directorio del componente.
2. Agregar la seccion _REQUIRES_.
3. Dentro de _REQUIRES_ agregar _driver_.

NOTA: en caso de faltar una dependencia, el error se informara al momento de compilar el proyecto.

NOTA: seguir los lineamientos de desarrollo _topdown_ a la hora de hacer modulos.

## Modificacion del CMakeLists.txt para resolver dependencias del proyecto

1. Abrir el archivo _CMakeLists.txt_ que esta dentro de la carpeta _main_.
2. Agregar la seccion _REQUIRES_.
3. Dentro de _REQUIRES_ agregar el nombre del modulo. En este caso, _task_functions_.
