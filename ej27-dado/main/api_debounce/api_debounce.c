#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "api_debounce.h"

void debounce_init(debounce_data_t *data)
{
    data->key = NOT_PRESSED;
    if (gpio_get_level(data->input) == 1)
    {
        data->debounce_state = BUTTON_UP;
    }
    else
    {
        data->debounce_state = BUTTON_DOWN;
    }
}

void debounce_update(debounce_data_t *data)
{
    switch (data->debounce_state)
    {
    case BUTTON_UP:
        if (gpio_get_level(data->input) == 0)
        {
            data->debounce_state = BUTTON_FALLING;
            data->debounce_accumulated = 0;
        }
        break;
    case BUTTON_FALLING:
        if (data->debounce_accumulated >= data->debounce_time_ms)
        {
            if (gpio_get_level(data->input) == 0)
            {
                data->debounce_state = BUTTON_DOWN;
            }
            else
            {
                data->debounce_state = BUTTON_UP;
            }
        }
        data->debounce_accumulated += data->delay_time_ms;
        break;
    case BUTTON_DOWN:
        if (gpio_get_level(data->input) == 1)
        {
            data->debounce_state = BUTTON_RISING;
            data->debounce_accumulated = 0;
        }
        break;
    case BUTTON_RISING:
        if (data->debounce_accumulated >= data->debounce_time_ms)
        {
            if (gpio_get_level(data->input) == 1)
            {
                data->key = PRESSED;
                data->debounce_state = BUTTON_UP;
            }
            else
            {
                data->debounce_state = BUTTON_DOWN;
            }
        }
        data->debounce_accumulated += data->delay_time_ms;
        break;
    default:
        debounce_init(data);
        break;
    }
}

key_status_t read_key(debounce_data_t *data)
{
    key_status_t aux = data->key;
    data->key = NOT_PRESSED;
    return aux;
}