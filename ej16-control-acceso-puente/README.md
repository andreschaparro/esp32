# Ejemplo 16: Control de acceso a un puente de un solo sentido con FreeRTOS

## Introduccion

Hay un puente, donde cada extremo se llama _A_ y _B_ respectivamente.

En el mismo, se permite el paso de un solo vehiculo a la vez.

Siempre, ingresan al puente por orden de llegada.

Se sugiere el uso de 2 tareas para:

- _tarea_a_: para controlar el ingreso/egreso en el extremo _A_.
- _tarea_b_: para controlar el ingreso/egreso en el extremo _B_.

Luego, para simular los estimulos de ingreso/egreso en ambos puntos, utilizar una tercer tarea llamada _tarea_test_.

La misma, debera generar la siguiente secuencia periodica de estimulos:

- **ENTRY_A**
- **ENTRY_B**
- **EXIT_A**
- **EXIT_B**

Todo debe ejecutarse en el _APP_CPU_NUM_.

## Recomendaciones

Resolverlo con semaforos binarios y mutex.

## Conclusiones

Este ejemplo es integrador porque:

- Utilizamos tareas de _FreeRTOS_.
- Utilizamos los semaforos binarios de _FreeRTOS_ para resolver el problema de serializacion.
- Utilizamos un mutex de _FreeRTOS_ para evitar el acceso simultaneo a un recurso compartido.
- Documentamos con _Doxygen_.
- Repasamos _C_ usando: _typedef_, _enum_, _switch case_, y _arrays_.
