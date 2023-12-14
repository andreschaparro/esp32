#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const char *tag = "SERIAL_COM";
static const gpio_num_t led = GPIO_NUM_2;
static const TickType_t delay_100_ms = pdMS_TO_TICKS(100);

static char buffer[100];
static uint32_t estado_led;

static void inicializar(void);
static void comandos_disponibles(void);
static void procesar_comandos(void);
static void encender_led(void);
static void apagar_led(void);
static void recibir_string(void);

void app_main(void)
{
    inicializar();
    while (1)
    {
        procesar_comandos();
        vTaskDelay(delay_100_ms);
    }
}

static void inicializar(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    estado_led = 0;
    gpio_set_level(led, estado_led);
    comandos_disponibles();
}

static void comandos_disponibles(void)
{
    ESP_LOGD(tag, "Comandos disponibles :");
    ESP_LOGD(tag, "Press 'e' or 'E' para encender el led");
    ESP_LOGD(tag, "Press 'a' or 'A' para apagar el led");
    ESP_LOGD(tag, "Press 's' or 'S' para escribir e imprimir una cadena de texto");
}

static void procesar_comandos(void)
{
    char caracter = getchar();
    if (caracter != 255)
    {
        switch (caracter)
        {
        case 'e':
        case 'E':
            encender_led();
            break;
        case 'a':
        case 'A':
            apagar_led();
            break;
        case 's':
        case 'S':
            recibir_string();
            break;
        default:
            comandos_disponibles();
            break;
        }
    }
}

static void encender_led(void)
{
    if (estado_led == 0)
    {
        estado_led = 1;
        gpio_set_level(led, estado_led);
        ESP_LOGD(tag, "Led encendido!");
    }
    else
    {
        ESP_LOGD(tag, "El led ya estaba encendido!");
    }
}

static void apagar_led(void)
{
    if (estado_led == 1)
    {
        estado_led = 0;
        gpio_set_level(led, estado_led);
        ESP_LOGD(tag, "Led apagado!");
    }
    else
    {
        ESP_LOGD(tag, "El led ya estaba apagado!");
    }
}

static void recibir_string(void)
{
    char caracter = '\0';
    memset(buffer, '\0', sizeof(buffer));
    uint8_t i = 0;
    ESP_LOGD(tag, "Escriba una cadena de texto y termine con un ENTER");
    while (caracter != '\n')
    {
        caracter = getchar();
        if ((caracter != 255) && (caracter != '\n'))
        {
            buffer[i] = caracter;
            i++;
        }
        vTaskDelay(delay_100_ms);
    }
    ESP_LOGD(tag, "La cadena de texto recibida es: %s", buffer);
}
