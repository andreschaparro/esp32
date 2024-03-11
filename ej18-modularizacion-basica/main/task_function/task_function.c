#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *tag = "MODULARIZACION-BASICA";

static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

void task_function(void *pvParameters)
{
    uint32_t index = (uint32_t)pvParameters;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    for (;;)
    {
        ESP_LOGD(tag, "Tarea %" PRIu32 " running", index);
        vTaskDelay(delay_1000_ms);
    }
}

const char *read_tag(void)
{
    return tag;
}