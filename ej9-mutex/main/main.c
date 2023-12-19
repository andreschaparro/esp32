#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *tag = "MUTEX";

static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);
static const TickType_t delay_3000_ms = pdMS_TO_TICKS(5000);

static uint32_t contador;

static SemaphoreHandle_t mux = NULL;

static void tarea_sumar_1(void *pvParameters);
static void tarea_sumar_2(void *pvParameters);

void app_main(void)
{
    BaseType_t ret;
    contador = 0;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    mux = xSemaphoreCreateMutex();
    configASSERT(mux != NULL);
    ret = xTaskCreatePinnedToCore(
        tarea_sumar_1,
        "Tarea Sumar 1",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        (TaskHandle_t *)NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_sumar_2,
        "Tarea Sumar 2",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 2U),
        (TaskHandle_t *)NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_sumar_1(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(mux, (TickType_t)5) == pdTRUE)
        {
            ESP_LOGD(tag, "Antes de Sumar 1 : %" PRIu32, contador);
            contador += 1;
            ESP_LOGD(tag, "Despues de Sumar 1 : %" PRIu32, contador);
            xSemaphoreGive(mux);
        }
        vTaskDelay(delay_1000_ms);
    }
}

static void tarea_sumar_2(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(mux, (TickType_t)5) == pdTRUE)
        {
            ESP_LOGD(tag, "Antes de Sumar 2 : %" PRIu32, contador);
            contador += 2;
            ESP_LOGD(tag, "Despues de Sumar 2 : %" PRIu32, contador);
            xSemaphoreGive(mux);
        }
        vTaskDelay(delay_3000_ms);
    }
}
