# Ejemplo 3: Comunicacion bidireccional por el ESP-IDF Monitor device

Hacer la preparacion ya explicada en el _ej2-parpadeo-led_.

## getchar()

La funcion _getchar()_ no es bloqueante y nos permite leer un caracter, si lo hubiese, de la UART. Si no hay caracteres disponibles, la funcion devuelve un 255.

Luego, y si hay un caracter disponible, podemos interpretar ese caracter como un comando a ejecutar.

## memset()

Esta funcion, es util para inicializar un array que guarde un string:

memset(array, '\0', sizeof(array))

Luego, podemos almacenar caracteres en el array hasta recibir un '\n'.

## Conclusiones

Este ejemplo, es integrador porque:

- Usamos logs de debug.
- Usamos una GPIO como salida para encender y apagar un led.
- Usamos el concepto de demora para ceder el CPU en FreeRTOS.
- Usamos la UART de forma bidireccional. O en otras palabras, recibimos y enviamos caracteres a traves de la misma.
- Repasa el uso de funciones en C para empezar a modularizar el codigo.
- Repasamos el uso de la palabra reservada _static_. Que evita, que nuestras funciones y variables globales sean accedidas por fuera de nuestro archivo _.c_. Esto, es muy importante al modularizar el codigo porque produce un encapsulamiento.
- Repasamos la estructura clasica de programacion en sistemas embebidos, conocida como _bare-metal_. La misma, tiene una inicializacion y un loop infinito donde se ejecutan las tareas.
