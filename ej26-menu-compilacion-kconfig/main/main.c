#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *tag = "KCONFIG";

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    ESP_LOGD(tag, "Mi entero es %d", CONFIG_MY_INT);
    ESP_LOGD(tag, "Mi string es %s", CONFIG_MY_STRING);
    bool my_bool;
#ifdef CONFIG_MY_BOOL
    my_bool = true;
#else
    my_bool = false;
#endif
    ESP_LOGD(tag, "Mi booleano %s", my_bool ? "true" : "false");
    int option;
#ifdef CONFIG_OPTION_1
    option = 1;
#elif CONFIG_OPTION_2
    option = 2;
#else
    option = 3;
#endif
    ESP_LOGD(tag, "Mi opcion es %d", option);
    vTaskDelete(NULL);
}
