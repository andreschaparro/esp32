#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const gpio_num_t led = GPIO_NUM_2;
static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static uint32_t led_state;

void app_main(void)
{
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    led_state = 0;
    gpio_set_level(led, led_state);
    while (1)
    {
        if (led_state == 0)
        {
            led_state = 1;
        }
        else
        {
            led_state = 0;
        }
        gpio_set_level(led, led_state);
        vTaskDelay(delay_1000_ms);
    }
}
