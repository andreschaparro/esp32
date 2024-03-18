# Ejemplo 23: Utilizar una biblioteca de terceros para leer el sensor DHT11 en ESP-IDF

## Introduccion

En el ejemplo, vamos a leer temperatura y humedad del ambiente utilizando el sensor DHT11.

Luego, mostraremos los resultados en el _ESP-IDF Monitor device_.

Este, es un caso tipico de uso del patron productor-consumidor, que se resuelve con colas en _FreeRTOS_.

NOTA: El DHT11 tiene un protocolo de comunicacion especial de un hilo.

NOTA: El DHT11 debe ser leido como minimo cada 2 segundos. Ver el siguiente link para mas informacion: http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html.

## Repositorio de UncleRus

Este repositorio, contiene una gran cantidad de modulos desarrollados para el ESP32 con el ESP-IDF:

https://github.com/UncleRus/esp-idf-lib/tree/master

Entre ellos, hay uno para leer el sensor DHT11.

El mismo, es el _component_ llamado _dht_.

Lo utilizaremos, como si fuese una caja negra. Es decir, sin ponernos a estudiar como esta implementado internamente.

La finalidad, es aprobechar el desarrollo ya hecho por otra persona. Tal cual, se realiza en equipos de desarrollo de software.

NOTA: Revisar siempre la licencia de los modulos que nos descarguemos, para asegurarnos de no incrumplir con los aspectos legales de su uso.

NOTA: Usarlo como una caja negra, no implica que no debamos hacerle un _testing_ para detectar bugs y limitaciones.

## Clonar el repositorio con la biblioteca de modulos

1. Abrir una terminal de GIT dentro del directorio del proyecto.
2. Ejecutar: git clone https://github.com/UncleRus/esp-idf-lib.git
3. Verificar que aparezca dentro del proyecto la siguiente ruta: _esp-idf-lib/components/dht_

## Incluir la biblioteca al proceso de compilacion

1. Abrir el _CMakeLists.txt_ que esta dentro del proyecto. No el de la carpeta _main_.
2. Antes de la linea _include_, agregar _set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/esp-idf-lib/components)_
3. Volver a compilar el proyecto, va a tardar como la primera vez.

Ahora, podemos utilizar el componente _dht_, presente dentro de la biblioteca, de forma similar a como utilizamos **driver/gpio.h**.

NOTA: Se recomienda, revisar los ejemplos dentro del repositorio para ver como se utilizan los distintos _components_.
