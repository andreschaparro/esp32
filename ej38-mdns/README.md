# Ejemplo 38: Encontrar a nuestro ESP32 conectado a una red wifi sin saber su IP en ESP-IDF

## Introduccion

El servicio _mDNS_ es un servicio _multicast UDP_ que se utiliza para encontrar _SOCs ESP32_ conectados a la red _wifi_.

Por lo que, podriamos acceder a su _webserver_ sin necesidad de conocer su direccion _IP_. Esto es un problema en las redes que no administramos, porque un servidor _DHCP_ le asignara una direccion _IP_ que puede cambiar a lo largo del tiempo.

Actualmente el componente _mDNS_ esta fuera del _ESP-IDF_ y debe ser agregado utilizando el _IDF Component Manager_.

Bibliotecas a incluir:

- **mdns.h**

## Agregar el componente espressif_mdns utilizando el IDF Component Manager

El _IDF Component Manager_ es una herramienta que descarga components mantenidos por _Espressif_ para nuestros proyectos.

Estos modulos, pueden verse en la pagina https://components.espressif.com/.

NOTA: Siempre es preferible utilizar estos modulos, a otros desarrollados por teceros.

1. Buscar en la pagina el que se llama _espressif/mdns_.
2. Elegir la version _1.3.0_.
3. Copiar el comando que se indica en la pagina. En este caso, _idf.py add-dependency "espressif/mdns^1.3.0"_.
4. Pegar el comando copiado en la _ESP-IDF Terminal_ en el paso 3.
5. Presionar _ENTER_.
6. Hacer un _ESP-IDF: Full Clean_.
7. Volver a compilar el proyecto.
8. Verificar, que dentro del archivo idf_component.yml este el modulo como dependencia.
9. Verificar, que se crea una carpeta llamada _managed_components_ dentro del directio del proyecto.
10. Verificar, que se crea una carpeta llamada _espressif\_\_mdns_ dentro de _managed_components_.

## Configuracion del mDNS en ESP-IDF

1. Llamar a la funcion _mdns_init_.
2. Verificar el valor de err con la macro _ESP_ERROR_CHECK_.
3. Llamar a la funcion _mdns_hostname_set_.
4. Pasarle como parametro un _const char\*_ que identifique al _ESP32_. En este caso _"esp32-id1_.
5. Verificar el valor de err con la macro _ESP_ERROR_CHECK_.
6. Llamar a la funcion _mdns_instance_name_set_.
7. Pasarle como parametro un _const char\*_ que identifique a nuestro dispositivo de forma mas amable. En este caso _"ESP32 con ID = 1"_.
8. Verificar el valor de err con la macro _ESP_ERROR_CHECK_.

Esto debe hacerse antes de conectarse a una red _wifi_ o de iniciar en modo _access point_.

Ahora, para hacer la misma peticion del ejemplo anterior desde el navegador _web_, hay que completar _esp32-id1.local"_ en lugar de la direccion _IP_ del _ESP32_.
