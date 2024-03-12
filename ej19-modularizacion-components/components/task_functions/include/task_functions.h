#ifndef TASK_FUNCTIONS_H
#define TASK_FUNCTIONS_H

#include "driver/gpio.h"

typedef enum
{
    BLINKING,
    NOT_BLINKING,
} led_mode_t;

typedef struct
{
    gpio_num_t gpio_led;
    gpio_num_t gpio_pulsador;
    led_mode_t modo_led;
} task_data_t;

void task_pulsador(void *pvParameters);

void task_led(void *pvParameters);

#endif