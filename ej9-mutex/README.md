# Ejemplo 9: Mutex en FreeRTOS

Como ya vimos un mutex es similar a un semaforo binario.

Pero, su uso practico es evitar que diferentes tareas accedan simultaneamente a un recurso compartido, por ejemplo una variable global.

Cuando dos o mas tareas acceden a un recurso compartido sin ningun control, puede ocurrir una condicion de carrea (race condition). Lo que implica, que el resultado final depende del orden en el que acceden las tareas al recurso compartido.

Otro ejemplo, de recurso compartido, es un area de codigo llamada seccion critica. La que debe ejecutarse en su totalidad sin que se produzca un cambio de contexto.

Si usaramos un semaforo binario tradicional en vez de un mutex, vamos a tener un problema que se llama inverion de prioridades.

Esto de produce cuando una tarea de menor prioridad accede al recurso compartido y no da el semaforo antes de que se produzca el cambio de contexto y entre en ejecucion una de mayor prioridad. Lo que como resultado, produce que la tarea de mayor prioridad quede bloqueada al intentar tomar el recurso compartido. Y es a efectos practicos, como si la de menor prioridad tuviese momentaneamente mayor prioridad que la otra.

Para minimizar este efecto existe el mutex. Que es un semaforo binario pero que incluye el algoritmo de herencia de prioridad (priority inheritance)

El mismo, hace que en el caso anterior, la tarea de menor prioridad eleve temporalmente su nivel para que entre primera en ejecucion y pueda dar el semaforo lo mas rapido posible. Luego, recupera su prioridad original. Esto no soluciona el problema al 100%.

En el ejemplo, utilizaremos dos tareas para modificar el contenido de una variable global llamada contador. Una, incrementara en 1 su valor. Y la otra, en 2. Pero, con el mutex nos aseguraremos que una tarea termine de modificarla antes de que la otra lo intente.

## Crear, dar, y tomar un mutex

El procedimiento es similar al binario, salvo que se utiliza la macro _xSemaphoreCreateMutex()_

Las funciones para tomar y dar son las mismas que las del semaforo binario.

## Otros problemas que pueden aparecer

Dos tareas de mayor prioridad que una tercera, se alternan y nunca dejen que esta ultima tome el CPU o le niegan el acceso a un recurso. Tambien, podria ser que solo una tarea de mayor prioridad se lo niege permanentemente. Esto, se conoce inanicion (starvation).

Ya se lo menciono en el ejemplo de semaforos y se llamaba punto muerto o interbloqueo (deadlock). En este caso, tambien puede producirse, cuando dos o mas tareas tratan de tomar recursos utilizados por la otra tarea, bloqueandose indefinidamente. Por ejemplo: la tarea 2 espera el recurso 1 que es tomado por la tarea 1, y la tarea 1 espera el recurso 2 que es tomado por la tarea 2.
