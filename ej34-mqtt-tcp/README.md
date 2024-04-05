# Ejemplo 34: Uso del protocolo MQTT sobre TCP en ESP-IDF

## Introduccion

El protocolo MQTT se utiliza para enviar y recibir mensajes a traves de un _broker_. Es uno de los protocolos para telemetria mas utilizados en _IOT_:

- Cada mensaje tendra un _topic_ y un _payload_. El _payload_ debe ser un _char\*_.
- Habra algunos _clients_ que publicaran mensajes al _broker_.
- Habra algunos _clients_ que se subscribiran al _broker_ para recibir mensajes.
- Cuando uno se subscribe, debe informarle al _broker_ a que _topics_ lo hace.

Asi, cuando un _client_ publique un mensaje al _broker_, el mismo tendra que buscar que _clients_ estan subscriptos al _topic_ del mensaje que llego. Y luego, el _broker_ le entregara a cada uno una copia del mensaje.

Esta arquitectura de comunicacion se llama _Pubish/Subscriber_.

![MQTT](/ej34-mqtt-tcp/mqtt.png "MQTT")

Como _broker_ de prueba utilizaremos uno publico alojado en la web: https://test.mosquitto.org/

Para poder acceder al mismo, necesitaremos conectar el _ESP32_ a un red _wifi_ con salida a internet.

Podriamos tener una _raspberry Pi_, conectada a la misma red _wifi_ donde tambien se conectara el _ESP32_, e instalarle el _mosquitto broker_. En este caso, tendriamos nuestro propio _broker_ de forma local y no seria necesaria la salida a internet. El unico cuidado que hay que tener, es que la direccion _IP_ de la _raspberry Pi_ sea estatica.

En el ejemplo:

- Inicializaremos el _client mqtt_ del _ESP32_
- Publicaremos un numero entero aleatorio que puede valer de 1 al 6, simulando ser un dado, con el _topic_ **/dados/dado1** al _broker_.
- Nos subscribiremos al _broker_ utilizando el el _topic_ **/dados/dado1**.
- Todo se hara en el mismo _ESP32_. De modo, que deberiamos recibir el mismo numero que publicamos.

Bibliotecas a incluir:

- **mqtt_client.h**

## Inicializacion del client MQTT por TCP en ESP-IDF

El _client_ funciona con _events_, de forma similar a como lo hace el _driver wifi_ con los _wifi events_. Por ello, necesitaremos un _handler_ que actue como _event callback_. El mismo, se llamara _mqtt_event_handler_ y se definira mas adelante.

1. Crear una variable llamada _client_ del tipo _esp_mqtt_client_handle_t_.
2. Crear una variable llamada _mqtt_cfg_ del tipo _esp_mqtt_client_config_t_:
   - El miembro _.broker.address.uri_ debe valer _"mqtt://test.mosquitto.org:1883"_.
3. Llamar a la funcion _esp_mqtt_client_init_.
4. Pasarle como parametro un puntero a _mqtt_cfg_.
5. Guardar el valor retornado en _client_.
6. Llamar al funcion _esp_mqtt_client_register_event_.
7. Pasarle como primer parametro _client_.
8. Pasarle como segundo parametro _ESP_EVENT_ANY_ID_.
9. Pasarle como tercer parametro _mqtt_event_handler_.
10. Pasarle como cuarto parametro _NULL_.
11. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
12. Llamar a la funcion _esp_mqtt_client_start_.
13. Pasarle como paramtro _client_.
14. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Publicar un mensaje al broker MQTT en ESP-IDF

1. Crear una variable llamada _msg_id_ del tipo _int8_t_.
2. Llamar a la funcion _esp_mqtt_client_publish_.
3. Pasarle como primer parametro _client_.
4. Pasarle como segundo parametro el _topic_. En este caso, debe valer _"/dados/dado_1"_.
5. Pasarle como tercer parametro el _payload_. En este caso, el numero aleatorio convertido a _string_.
6. Pasarle como cuarto parametro _0_.
7. Pasarle como quinto parametro _1_. Este valor, corresponde al _qos_ que significa _quality of service_ y puede valer _0_, _1_, o _2_.
8. Pasarle como sexto parametro _0_.
9. Guardar el valor retornado en _msg_id_.

Con un _qos_ de _1_ , si el valor retornado en _msg_id_ es postivo, indica que se publico con exito el mensaje en el _broker_.

En cambio, con un _qos_ de _0_ , si el valor retornado en _msg_id_ siempre sera _0_ y no habra forma de saber si se publico con exito el mensaje en el _broker_.

## Definicion del handler para los events del client MQTT en ESP-IDF

1. Crear una funcion llamada mqtt_event_handler que:
   - Devuelve _void_.
   - El primer argumento es _void \*handler_args_.
   - El segundo argumento es _esp_event_base_t base_.
   - El tercer argumento es _int32_t event_id_.
   - El cuarto argumento es _void \*event_data_.
2. Crear una variable llamada _event_ del tipo _esp_mqtt_event_handle_t_.
3. Guardar el valor de _event_data_ en en _event_.
4. Crear un swith utilizando _(esp_mqtt_event_id_t)event_id_.

El evento _MQTT_EVENT_CONNECTED_ se produce despues de que el _broker_ confirma que nuestro paso de inicialiacion se completo correctamente. Se utiliza comunmente, para llamar a la funcion _esp_mqtt_client_subscribe_ y que el _ESP32_ se subscriba al _broker_.

El evento _MQTT_EVENT_DISCONNECTED_ se produce cuando hay una desconexion con el _broker_.

El evento _MQTT_EVENT_SUBSCRIBED_ se produce despues que nos subscribimos al _broker_, con un _qos_ de _1_ o _2_, y el mismo nos envia un mensaje de confirmacion de la subscripcion.

El evento _MQTT_EVENT_PUBLISHED_ se produce despues de publicar un mensaje en el _broker_, con un _qos_ de _1_ o _2_, y el mismo nos envia un mensaje de confirmacion de la publicacion.

El evento _MQTT_EVENT_DATA_ se produce cuando el _broker_ nos hace llegar un mensaje, cuyo _topic_ era uno al que estabamos subscriptos. Se utiliza comunmente, para acceder al _payload_ del mensaje mediante los miembros de la variable _event_.

El evento _MQTT_EVENT_ERROR_ se produce cuando hay algun tipo de error. Se utiliza comunmente, para mostrar el error por el _ESP-IDF: Monitor Device_.

## Subscribirse al broker MQTT por topic en ESP-IDF

1. Crear una variable llamada _msg_id_ del tipo _int8_t_.
2. Llamar a la funcion _esp_mqtt_client_subscribe_.
3. Pasarle como primer parametro _client_.
4. Pasarle como segundo parametro el _topic_. En este caso, debe valer _"/dados/dado_1"_.
5. Pasarle como tercer parametro _1_. Este valor, corresponde al _qos_.
6. Guardar el valor retornado en _msg_id_.

Con un _qos_ de _1_ , si el valor retornado en _msg_id_ es postivo, indica que se recibio con exito un mensaje del _broker_.

En cambio, con un _qos_ de _0_ , si el valor retornado en _msg_id_ siempre sera _0_.
