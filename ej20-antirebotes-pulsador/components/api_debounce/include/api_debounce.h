#ifndef API_DEBOUNCE_H
#define API_DEBOUNCE_H

typedef enum
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_FALLING,
    BUTTON_RISING,
} debounce_state_t;

typedef enum
{
    PRESSED,
    NOT_PRESSED,
} key_status_t;

typedef struct
{
    gpio_num_t input;
    debounce_state_t debounce_state;
    TickType_t debounce_time_ms;
    key_status_t key;
} debounce_data_t;

void debounce_init(debounce_data_t *data);

void debounce_update(debounce_data_t *data);

key_status_t read_key(debounce_data_t *data);

#endif
