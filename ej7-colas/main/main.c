#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"

static const char *tag = "COLAS";

static const TickType_t delay_100_ms = pdMS_TO_TICKS(100);
static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static TaskHandle_t handle_productor = NULL;
static TaskHandle_t handle_consumidor = NULL;
static QueueHandle_t cola;

static void tarea_productor(void *pvParameters);
static void tarea_consumidor(void *pvParameters);

void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    ret = xTaskCreatePinnedToCore(
        tarea_productor,
        "Tarea Productor",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_productor,
        PRO_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_consumidor,
        "Tarea Consumidor",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_consumidor,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_productor(void *pvParameters)
{
    char caracter;
    char tx_buffer[100];
    uint8_t i;
    cola = xQueueCreate(10, sizeof(tx_buffer));
    if (cola == NULL)
    {
        ESP_LOGE(tag, "Error al crear la cola");
        esp_restart();
    }
    ESP_LOGD(tag, "Escriba una cadena de texto y termine con un ENTER");
    for (;;)
    {
        caracter = '\0';
        memset(tx_buffer, '\0', sizeof(tx_buffer));
        i = 0;
        while (caracter != '\n')
        {
            caracter = getchar();
            if ((caracter != 255) && (caracter != '\n'))
            {
                tx_buffer[i] = caracter;
                i++;
            }
            vTaskDelay(delay_100_ms);
        }
        if (xQueueSend(cola, (void *)tx_buffer, (TickType_t)0) != pdTRUE)
        {
            ESP_LOGE(tag, "Error al escribir en la cola");
        }
    }
}

static void tarea_consumidor(void *pvParameters)
{
    char rx_buffer[100];
    for (;;)
    {
        memset(rx_buffer, '\0', sizeof(rx_buffer));
        if (xQueueReceive(cola, (void *)rx_buffer, (TickType_t)5) == pdTRUE)
        {
            ESP_LOGD(tag, "La cadena de texto recibida es: %s", rx_buffer);
        }
        vTaskDelay(delay_1000_ms);
    }
}
