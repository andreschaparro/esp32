#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "api_debounce.h"

typedef enum
{
    OFF,
    ON,
} led_status_t;

static const TickType_t delay_10_ms = pdMS_TO_TICKS(10);

static const gpio_num_t led = GPIO_NUM_2;

static SemaphoreHandle_t sem_led = NULL;

static debounce_data_t pulsador = {
    .input = GPIO_NUM_32,
    .debounce_time_ms = pdMS_TO_TICKS(40),
};

static void task_pulsador(void *pvParameters);
static void task_led(void *pvParameters);

void app_main(void)
{
    sem_led = xSemaphoreCreateBinary();
    configASSERT(sem_led != NULL);
    BaseType_t ret;
    ret = xTaskCreatePinnedToCore(
        task_pulsador,
        "Tarea Pulsador",
        4096,
        (void *)(&pulsador),
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        task_led,
        "Tarea Led",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void task_pulsador(void *pvParameters)
{
    debounce_data_t *data = (debounce_data_t *)pvParameters;
    ESP_ERROR_CHECK(gpio_set_direction(data->input, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_pullup_en(data->input));
    ESP_ERROR_CHECK(gpio_pulldown_dis(data->input));
    debounce_init(data);
    for (;;)
    {
        debounce_update(data);
        if (read_key(data) == PRESSED)
        {
            xSemaphoreGive(sem_led);
        }
        vTaskDelay(delay_10_ms);
    }
}

static void task_led(void *pvParameters)
{
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    led_status_t led_status = OFF;
    gpio_set_level(led, led_status);
    for (;;)
    {
        if (xSemaphoreTake(sem_led, (TickType_t)5) == pdTRUE)
        {
            if (led_status == OFF)
            {
                led_status = ON;
            }
            else
            {
                led_status = OFF;
            }
            gpio_set_level(led, led_status);
        }
    }
}