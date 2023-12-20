# Ejemplo 9: Mutex en FreeRTOS

Como ya vimos un mutex es similar a un semaforo binario.

Pero, su uso practico es evitar que diferentes tareas accedan simultaneamente a un recurso compartido, por ejemplo una variable. Es un problema distinto a la serializacion.

Esto, es para evitar lo que se conoce como condicion de carrera (race condition). Lo que implica, que el resultado final depende del orden en el que acceden las tareas al recurso compartido.

Otro ejemplo, de recurso compartido muy tipico, es un area de codigo donde las tareas deban operar de forma serializada. A esta, se la suele llamar seccion critica.

Si usaramos un semaforo binario, vamos a tener un problema que se llama inverion de prioridades. El mismo se da, cuando una tarea de menor prioridad accede al recurso compartido, no da el semaforo, y produce que una de mayor prioridad quede bloqueada al intentar tomarlo. Entonces, es como si la de menor prioridad tuviese momentaneamente mayor prioridad que la otra.

Asi, nace el mutex que es un semaforo binario pero que incluye el algoritmo de herencia de prioridad (priority inheritance)

El mismo, hace que en el caso anterior, la tarea de menor prioridad eleve temporalmente su nivel para que entre primera en ejecucion y de el semaforo lo mas rapido posible. Luego, recupera su prioridad original. Esto no soluciona el problema al 100%.

En el ejemplo, utilizaremos dos tareas para modificar el contenido de una variable global llamada contador. Una, incrementara en 1 su valor. Y la otra, en 2. Pero, con el mutex nos aseguraremos que una tarea termine de modificarla antes de que la otra lo intente.

## Crear, dar, y tomar un mutex

El procedimiento es similar al binario, salvo que se utiliza la macro _xSemaphoreCreateMutex()_

Las funciones para tomar y dar son las mismas que las del semaforo binario.

## Otros problemas que pueden aparecer

Dos tareas de mayor prioridad que una tercera, se alternan y nunca dejen que esta ultima tome el recurso. Esto, se conoce inanicion (starvation).

Ya se lo menciono en el ejemplo de semaforos y se llamaba punto muerto o interbloqueo (deadlock). En este caso, tambien puede producirse, cuando dos o mas tareas tratan de tomar recursos utilizados por la otra tarea, bloqueandose indefinidamente.
