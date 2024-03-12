#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "task_functions.h"

typedef enum
{
    OFF,
    ON,
} led_status_t;

typedef enum
{
    PRESSED,
    NOT_PRESSED,
} pulsador_status_t;

static const TickType_t delay_50_ms = pdMS_TO_TICKS(50);
static const TickType_t delay_500_ms = pdMS_TO_TICKS(500);

void task_pulsador(void *pvParameters)
{
    task_data_t *data = (task_data_t *)(pvParameters);
    ESP_ERROR_CHECK(gpio_set_direction(data->gpio_pulsador, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_pullup_en(data->gpio_pulsador));
    ESP_ERROR_CHECK(gpio_pulldown_dis(data->gpio_pulsador));
    pulsador_status_t estado_pulsador = NOT_PRESSED;
    for (;;)
    {
        estado_pulsador = gpio_get_level(data->gpio_pulsador);
        if (estado_pulsador == PRESSED)
        {
            if (data->modo_led == BLINKING)
            {
                data->modo_led = NOT_BLINKING;
            }
            else
            {
                data->modo_led = BLINKING;
            }
        }
        vTaskDelay(delay_50_ms);
    }
}

void task_led(void *pvParameters)
{
    task_data_t *data = (task_data_t *)(pvParameters);
    ESP_ERROR_CHECK(gpio_set_direction(data->gpio_led, GPIO_MODE_OUTPUT));
    led_status_t estado_led = OFF;
    ESP_ERROR_CHECK(gpio_set_level(data->gpio_led, estado_led));
    data->modo_led = NOT_BLINKING;
    for (;;)
    {
        if (data->modo_led == BLINKING)
        {
            if (estado_led == OFF)
            {
                estado_led = ON;
            }
            else
            {
                estado_led = OFF;
            }
            ESP_ERROR_CHECK(gpio_set_level(data->gpio_led, estado_led));
        }
        vTaskDelay(delay_500_ms);
    }
}
