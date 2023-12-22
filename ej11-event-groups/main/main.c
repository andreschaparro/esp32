#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

static const char *tag = "EVENT_GROUPS";

static const EventBits_t flag_1 = BIT0;
static const EventBits_t flag_2 = BIT1;

static const TickType_t delay_2000_ms = pdMS_TO_TICKS(2000);
static const TickType_t delay_5000_ms = pdMS_TO_TICKS(5000);

static EventGroupHandle_t grupo;

static void tarea_1(void *pvParameters);
static void tarea_2(void *pvParameters);
static void tarea_3(void *pvParameters);

void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    grupo = xEventGroupCreate();
    configASSERT(grupo != NULL);
    ret = xTaskCreatePinnedToCore(
        tarea_1,
        "Tarea 1",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        (TaskHandle_t *)NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_2,
        "Tarea 2",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        (TaskHandle_t *)NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_3,
        "Tarea 3",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        (TaskHandle_t *)NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_1(void *pvParameters)
{
    EventBits_t flags;
    for (;;)
    {
        flags = xEventGroupSetBits(grupo, flag_1);
        ESP_LOGD(tag, "Mensaje desde la tarea 1");
        ESP_LOGD(tag, "Flags %" PRIu32, flags);
        vTaskDelay(delay_2000_ms);
    }
}

static void tarea_2(void *pvParameters)
{
    EventBits_t flags;
    for (;;)
    {
        flags = xEventGroupSetBits(grupo, flag_2);
        ESP_LOGD(tag, "Mensaje desde la tarea 2");
        ESP_LOGD(tag, "Flags %" PRIu32, flags);
        vTaskDelay(delay_5000_ms);
    }
}

static void tarea_3(void *pvParameters)
{
    EventBits_t flags;
    for (;;)
    {
        flags = xEventGroupWaitBits(grupo, flag_1 | flag_2, pdTRUE, pdTRUE, (TickType_t)100);
        if ((flags & (flag_1 | flag_2)) == (flag_1 | flag_2))
        {
            ESP_LOGD(tag, "Mensaje desde la tarea 3");
            ESP_LOGD(tag, "Flags %" PRIu32, flags);
        }
        else
        {
            ESP_LOGW(tag, "Timeout desde la tarea 3");
            ESP_LOGW(tag, "Flags %" PRIu32, flags);
        }
    }
}