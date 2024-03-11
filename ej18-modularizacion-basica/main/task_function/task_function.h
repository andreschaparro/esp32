#ifndef TASK_FUNCTION
#define TASK_FUNCTION

typedef struct
{
    gpio_num_t led;
    gpio_num_t pulsador;
} task_data_t;

void task_function(void *pvParameters);

const char *get_tag(void);

#endif