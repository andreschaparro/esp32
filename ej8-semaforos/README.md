# Ejemplo 8: Semaforos en FreeRTOS

Resuelve procesos de sincronizacion, asi como las colas resuelven el problema de intercambio de datos entre tareas.

Un semaforo es un numero entero que se inicializa en un valor mayor a 0. Luego, se lo puede incrementar o decrementar en 1.

Cuando una tarea decrementa el semaforo, si el resultado es negativo, la tarea se bloquea hasta que otra lo incremente.

El primer uso practico de un semaforo, es la señalizacion. Es decir, que una tarea le avise a otra que algo paso. Lo que nos permite, que algo se ejecute antes que otra cosa. Esto resuelve lo que se conoce como problema de serializacion.

NOTA: La prioridad del que avisa debe ser menor que la del que espera el desbloqueo si corren en el mismo CORE.

NOTA: La señalizacion puede hacer en ambos sentidos, si consideramos que cada tarea tiene dos partes. Asi, la primera parte de la tarea 2 depende del fin de la primera parte de la tarea 1. Luego, el fin de la segunda parte de la tarea 1 depende del fin de la segunda parte de la tarea 2. En estos casos, se debe tener cuidad con los puntos muerto o interbloqueos (deadlocks).

En FreeRTOS hay 3 tipos de semaforos:

- Binarios que pueden valer 0 o 1.
- Contador que puede valer 0, 1, 2, 3, ...
- Mutex que puede valer 0 o 1.

El ejemplo, nos permitira reescribir el codigo del ejemplo 6 utilizando dos tareas y un semaforo binario. La primera, lee el pulsador por polling. La segunda, invierte el estado del led. Ambas tareas, correran en el CORE_1 con diferente prioridad.

Como primero es necesario leer el pulsador para luego invertir el estado del led. Este es un ejemplo de señalizacion.

# Crear un semaforo binario

1. Crear una instancia de la estructura _SemaphoreHandle_t_ llamada **sem_bin**.
2. Inicializarla en **NULL**.
3. Llamar a la macro _xSemaphoreCreateBinary()_.
4. El valor retornado por la macro debe alamacenarse en **sem_bin**.
5. Verificar con la macro _configASSERT()_ que _sem_bin_ sea distinto de NULL.

NOTA: Un semaforo binario siempre se crea tomado.

# Dar un semaforo binario

1. Llamar a la macro _xSemaphoreGive()_.
2. Pasarle como parametro **sem_bin**.

# Tomar un semaforo binario

1. Llamar a la macro _xSemaphoreTake()_.
2. Pasarle como primer parametro **sem_bin**.
3. Pasarlo como segundo parametro 5 casteado como _(TickType_t)_. Este valor, es la cantidad de ticks a esperar si el semafoto esta tomado con la tarea bloqueada. En caso de poner 0, la macro se vuelve no bloqueante.
4. Si el valor devuelto por _xSemaphoreTake()_ es igual a _pdTRUE_ se pudo tomar el semaforo.

# Crear, dar, y tomar de un semaforo contador

El procedimiento es similar al binario, salvo que se utiliza la macro _xSemaphoreCreateCounting_. Como primer parametro, se le pasa el valor maximo que puede tener. Y como segundo parametro, se le pasa el valor inicial.

Las funciones para tomar y dar son las mismas que las del semaforo binario.
