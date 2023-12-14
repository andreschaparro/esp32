#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const gpio_num_t pulsador = GPIO_NUM_32;
static const gpio_num_t led = GPIO_NUM_2;

static uint32_t estado_led;

void app_main(void)
{
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    estado_led = 0;
    gpio_set_level(led, estado_led);
    gpio_set_direction(pulsador, GPIO_MODE_INPUT);
    gpio_pullup_en(pulsador);
    gpio_pulldown_dis(pulsador);
    while (1)
    {
        if (gpio_get_level(pulsador) == 0)
        {
            if (estado_led == 0)
            {
                estado_led = 1;
            }
            else
            {
                estado_led = 0;
            }
        }
        gpio_set_level(led, estado_led);
        vTaskDelay(1);
    }
}
