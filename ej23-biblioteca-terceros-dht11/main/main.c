#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "dht.h"

typedef struct
{
    float temp;
    float hum;
} dht11_reading_t;

static const char *tag = "DHT11";

static const TickType_t delay_2000_ms = pdMS_TO_TICKS(2000);

static QueueHandle_t cola;

static void tarea_productor(void *pvParameters);
static void tarea_consumidor(void *pvParameters);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    cola = xQueueCreate(10, sizeof(dht11_reading_t));
    configASSERT(cola != NULL);
    BaseType_t ret;
    ret = xTaskCreatePinnedToCore(
        tarea_productor,
        "Tarea Productor",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_consumidor,
        "Tarea consumidor",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_productor(void *pvParameters)
{
    dht11_reading_t reading;
    reading.hum = 0.0;
    reading.temp = 0.0;
    for (;;)
    {
        ESP_ERROR_CHECK(dht_read_float_data(DHT_TYPE_DHT11, GPIO_NUM_17, &reading.hum, &reading.temp));
        xQueueSend(cola, (void *)&reading, (TickType_t)5);
        vTaskDelay(delay_2000_ms);
    }
}

static void tarea_consumidor(void *pvParameters)
{
    dht11_reading_t reading;
    reading.hum = 0.0;
    reading.temp = 0.0;
    for (;;)
    {
        if (xQueueReceive(cola, &reading, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGD(tag, "La humedad ambiente es: %.2f", reading.hum);
            ESP_LOGD(tag, "La temperatura ambiente es: %.2f", reading.temp);
            ESP_LOGD(tag, "---------------------------------------------------------------------");
        }
    }
}