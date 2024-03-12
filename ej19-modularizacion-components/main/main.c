#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "task_functions.h"

enum task_index
{
    TAREA_1,
    TAREA_2,
    TAREA_3,
};

static task_data_t task_data_array[] = {
    [TAREA_1] =
        {
            .gpio_led = GPIO_NUM_2,
            .gpio_pulsador = GPIO_NUM_32,
            .modo_led = NOT_BLINKING,
        },
    [TAREA_2] =
        {
            .gpio_led = GPIO_NUM_25,
            .gpio_pulsador = GPIO_NUM_14,
            .modo_led = NOT_BLINKING,
        },
    [TAREA_3] =
        {
            .gpio_led = GPIO_NUM_26,
            .gpio_pulsador = GPIO_NUM_12,
            .modo_led = NOT_BLINKING,
        },
};

static TaskHandle_t handle_tarea_pulsador_1 = NULL;
static TaskHandle_t handle_tarea_pulsador_2 = NULL;
static TaskHandle_t handle_tarea_pulsador_3 = NULL;
static TaskHandle_t handle_tarea_led_1 = NULL;
static TaskHandle_t handle_tarea_led_2 = NULL;
static TaskHandle_t handle_tarea_led_3 = NULL;

void app_main(void)
{
    BaseType_t ret;
    ret = xTaskCreatePinnedToCore(
        task_pulsador,
        "Tarea Pulsador 1",
        4096,
        (void *)(&task_data_array[TAREA_1]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_pulsador_1,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_led,
        "Tarea led 1",
        4096,
        (void *)(&task_data_array[TAREA_1]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_led_1,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_pulsador,
        "Tarea Pulsador 2",
        4096,
        (void *)(&task_data_array[TAREA_2]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_pulsador_2,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_led,
        "Tarea led 2",
        4096,
        (void *)(&task_data_array[TAREA_2]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_led_2,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_pulsador,
        "Tarea Pulsador 3",
        4096,
        (void *)(&task_data_array[TAREA_3]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_pulsador_3,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_led,
        "Tarea led 3",
        4096,
        (void *)(&task_data_array[TAREA_3]),
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_led_3,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}
