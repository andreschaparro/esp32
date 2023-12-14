#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const gpio_num_t led = GPIO_NUM_2;
static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static uint32_t estado_led;

void app_main(void)
{
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    estado_led = 0;
    gpio_set_level(led, estado_led);
    while (1)
    {
        if (estado_led == 0)
        {
            estado_led = 1;
        }
        else
        {
            estado_led = 0;
        }
        gpio_set_level(led, estado_led);
        vTaskDelay(delay_1000_ms);
    }
}
