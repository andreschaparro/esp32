# Ejemplo 20: Antirebotes para un pulsador utilizando maquinas de estado

## Introduccion

Como se vio en el ejemplo 5 y en otros posteriores, al presionar un pulsador aparecer un efecto de rebotes.

Una forma de eliminarlos, es por hardware utilizando redes R-C. La otra, es por software.

En este caso, veremos como hacerlo utilizando una maquina de estados finitos o _MEF_. No obstante, hay otros algotirmos mas sencillos.

Una maquina de estados, permiten modelar un sistema graficamente mediante un diagrama o una tabla.

Basicamente, se va cambiando de un estado a otro mediante transiciones.

En principio, exiten dos tipos de _MEF_: _Moore_ y _Mealy_.

- En las _Moore_, las salidas, solo depende del estado actual. Y, el estado proximo del estado actual y de las entradas.

- En las _Mealy_, las salidas, dependen del estado actual y de las entradas. Y, el estado proximo del estado actual y de las entradas.

Para el ejemplo, se utlizara una maquina de estados de tipo _Mealy_ y la integraremos dentro de un _component_ llamado _api_debounce_.

Y finalmente, cambiaremos el estado de un led cada vez que se presione el pulsador evidenciando que desaparecen los rebotes.

NOTA: Se recomienda repasar la teoria de maquinas de estado.

NOTA: Existe un framework online para desarrollar graficamente maquinas de estado. El mismo, se llama _YAKINDU_:

https://www.itemis.com/en/products/create/

## Conclusiones

Este, es un ejemplo integrador porque:

- Utilizamos tareas y semaforos binarios de _FreeRTOS_.
- Utilizamos una _MEF_.
- Creamos un _component_ reutilizable.
- Este ejemplo, es una version mucho mas funcional del ejemplo 8.
