# Ejemplo 17: Control de un estacionamiento

## Introduccion

Hay un estacionamiento, con un punto de ingreso y otro de egreso.

La capacidad es limitada.

Siempre, ingresan al puente por orden de llegada y de a uno.

Se sugiere el uso de 2 tareas para:

- _tarea_a_: para controlar el ingreso.
- _tarea_b_: para controlar el egreso.

Luego, para simular los estimulos de ingreso/egreso en ambos puntos, utilizar una tercer tarea llamada _tarea_test_.

La misma, debera generar la siguiente secuencia periodica de estimulos:

- **ENTRY**
- **EXIT**

Todo debe ejecutarse en el _APP_CPU_NUM_.

## Recomendaciones

Resolverlo con semaforos binarios y mutex.

## Conclusiones

Este ejemplo es integrador porque:

- Utilizamos tareas de _FreeRTOS_.
- Utilizamos los semaforos binarios de _FreeRTOS_ para resolver el problema de serializacion.
- Utilizamos un mutex de _FreeRTOS_ para evitar el acceso simultaneo a un recurso compartido.
- Documentamos con _Doxygen_.
- Repasamos C usando: typedef, enum, switch case, arrays.
