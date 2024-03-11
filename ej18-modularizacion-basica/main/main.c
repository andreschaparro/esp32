#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "task_function.h"

static TaskHandle_t handle_tarea_1 = NULL;
static TaskHandle_t handle_tarea_2 = NULL;
static TaskHandle_t handle_tarea_3 = NULL;

void app_main(void)
{
    BaseType_t ret;
    uint32_t index;
    esp_log_level_set(read_tag(), ESP_LOG_DEBUG);
    index = 1;
    ret = xTaskCreatePinnedToCore(
        task_function,
        "Tarea 1",
        4096,
        (void *)index,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_1,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    index = 2;
    ret = xTaskCreatePinnedToCore(
        task_function,
        "Tarea 2",
        4096,
        (void *)index,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_2,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    index = 3;
    ret = xTaskCreatePinnedToCore(
        task_function,
        "Tarea 3",
        4096,
        (void *)index,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_3,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
}
