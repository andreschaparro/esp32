#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "task_function.h"

typedef enum
{
    BLINKING,
    NOT_BLINKING,
} led_mode_t;

typedef enum
{
    ON,
    OFF,
} led_status_t;

static const char *tag = "MODULARIZACION-BASICA";

static const TickType_t delay_500_ms = pdMS_TO_TICKS(500);

void task_function(void *pvParameters)
{
    task_data_t const *data = (task_data_t *)(pvParameters);
    gpio_set_direction(data->led, GPIO_MODE_OUTPUT);
    led_mode_t modo_led = NOT_BLINKING;
    led_status_t estado_led = OFF;
    gpio_set_level(data->led, estado_led);
    gpio_set_direction(data->pulsador, GPIO_MODE_INPUT);
    gpio_pullup_en(data->pulsador);
    gpio_pulldown_dis(data->pulsador);
    char *nombre_tarea = (char *)pcTaskGetName(NULL);
    for (;;)
    {
        ESP_LOGD(tag, "%s Running", nombre_tarea);
        if (gpio_get_level(data->pulsador) == 0)
        {
            if (modo_led == BLINKING)
            {
                modo_led = NOT_BLINKING;
            }
            else
            {
                modo_led = BLINKING;
            }
        }
        vTaskDelay(delay_500_ms);
        if (modo_led == BLINKING)
        {
            if (estado_led == OFF)
            {
                estado_led = ON;
            }
            else
            {
                estado_led = OFF;
            }
            gpio_set_level(data->led, estado_led);
        }
        vTaskDelay(delay_500_ms);
    }
}

const char *get_tag(void)
{
    return tag;
}