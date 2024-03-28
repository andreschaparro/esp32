#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *tag = "TAREAS";
static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static TaskHandle_t handle_tarea_1 = NULL;
static TaskHandle_t handle_tarea_2 = NULL;

static void tarea_1(void *pvParameters);
static void tarea_2(void *pvParameters);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    BaseType_t ret;
    ret = xTaskCreate(
        tarea_1,
        "tarea_1",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_1);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_2,
        "tarea 2",
        (2 * configMINIMAL_STACK_SIZE),
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_2,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
}

static void tarea_1(void *pvParameters)
{
    for (uint8_t i = 0;;)
    {
        ESP_LOGD(tag, "Ejecucion de la tarea 1 %" PRIu32, xTaskGetTickCount());
        vTaskDelay(delay_1000_ms);
        if (i == 5)
        {
            ESP_LOGD(tag, "Suspencion de la tarea 2 %" PRIu32, xTaskGetTickCount());
            vTaskSuspend(handle_tarea_2);
        }
        else if (i == 8)
        {
            ESP_LOGD(tag, "Reanudacion de la tarea 2 %" PRIu32, xTaskGetTickCount());
            vTaskResume(handle_tarea_2);
        }
        else if (i == 10)
        {
            ESP_LOGD(tag, "Eliminacion de la tarea 2 %" PRIu32, xTaskGetTickCount());
            vTaskDelete(handle_tarea_2);
        }
        if (i < 100)
        {
            i++;
        }
    }
}

static void tarea_2(void *pvParameters)
{
    TickType_t previousWakeTime;
    for (;;)
    {
        previousWakeTime = xTaskGetTickCount();
        ESP_LOGD(tag, "Ejecucion de la tarea 2 %" PRIu32, xTaskGetTickCount());
        xTaskDelayUntil(&previousWakeTime, delay_1000_ms);
    }
}