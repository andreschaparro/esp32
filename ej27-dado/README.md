# Ejemplo 27: Dado electronico utilizando un pulsador y el display SSD1306

## Introduccion

Se utilizara un pulsador en conjunto con un antirebotes por software.

Cada vez que se presione el pulsador se generara un numero aleatorio.

Finalmente, se mostrara el numero en el display _OLED SSD1306_ utilizando los modulos de _Espressif_.

Todo el proyecto estara modularizado y las tareas deben correr en el _APP_CPU_NUM_.

## Recomendaciones

El bus _I2C_ debe inicializarse de forma global en _app_main_. Esto, es util para que se haga una sola vez en caso de que hubiese mas dispositivos en el bus.

Crear 3 modulos dentro de la carpeta _main_:

- **api_debounce** debera tener un _.c_ y un _.h_ con las mismas funciones que las hechas en el ejemplo 20.
- **api_dado** debera tener un _.c_ y un _.h_ con las siguientes funciones:
  - _tirar_dado_: devuelve un numero aleatorio entre 1 y 6.
- **api_ssd1306** debera tener un _.c_ y un _.h_ con las siguientes funciones:
  - _display_init_: inicializa el display _OLED SSD1306_.
  - _display_string_create_ : crea un label en el display.
  - _display_string_position_write_: escribe un texto en label y lo situa en una posicion del display.

La _api_ssd1306_ debe utilizar internamente con los modulos de _Espressif_.

Hacer 3 tareas:

- **tarea_pulsador**:
  - Debe atender el pulsador utilizando la _api_debounce_.
  - Entregar un semaforo binario.
- **tarea_dado**:
  - Debe permanecer bloqueada hasta que se pueda tomar el semaforo binario.
  - Debe generar el numero de 1 a 6 utilizando la _api_dado_.
  - Debe enviar el numero a una cola.
- **tarea_display**:
  - Debe permanecer bloqueada hasta que haya un numero en la cola.
  - Debe mostrar el numero en el display _OLED SSD1306_ utilizando la _api_ssd1306_.

## Generacion de un numero aleatorio en ESP-IDF

1. Incluir la biblioteca **esp_random.h**.
2. Llamar a la funcion _esp_random()_.

NOTA: El numero generado es positivo.

## Conclusiones

Este es un ejemplo integrador porque:

- Generamos numeros aleatorios.
- Utilizamos modularizacion.
- Utilizamos un antirebotes por software para un pulsador.
- Utilizamos la comunicacion _I2C_.
- Utilizamos el _IDF Component Manager_ y los modulos de _Espressif_ disponiles en la pagina *https://components.espressif.com/*.
- Utilizamos el controlador para pantallas _LCD_ del _ESP-IDF_ con el display _OLED SSD1306_.
- Utilizamos la biblioteca grafica LVGL para escribir texto con el display _OLED SSD1306_.
- Utilizamos tareas, semaforos binarios, y colas de _FreeRTOS_.
- Utilizamos un mutex, de forma indirecta, para que al usar las funciones de la biblioteca _LVGL_ sean _thread-safe_.
- Repasamos C utilizando: directivas del precompilador, _typedef_, _enum_, y _struct_.
