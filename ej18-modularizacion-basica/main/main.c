#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "task_function.h"

enum task_index
{
    TAREA_1,
    TAREA_2,
    TAREA_3,
};

static const task_data_t task_data_array[] = {
    [TAREA_1] =
        {
            .led = GPIO_NUM_2,
            .pulsador = GPIO_NUM_32,
        },
    [TAREA_2] =
        {
            .led = GPIO_NUM_25,
            .pulsador = GPIO_NUM_14,
        },
    [TAREA_3] =
        {
            .led = GPIO_NUM_26,
            .pulsador = GPIO_NUM_12,
        },
};

static TaskHandle_t handle_tarea_1 = NULL;
static TaskHandle_t handle_tarea_2 = NULL;
static TaskHandle_t handle_tarea_3 = NULL;

void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(get_tag(), ESP_LOG_DEBUG);
    ret = xTaskCreatePinnedToCore(
        task_function,
        "Tarea 1",
        4096,
        (void *)(&task_data_array[TAREA_1]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_1,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_function,
        "Tarea 2",
        4096,
        (void *)(&task_data_array[TAREA_2]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_2,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_function,
        "Tarea 3",
        4096,
        (void *)(&task_data_array[TAREA_3]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_3,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}
