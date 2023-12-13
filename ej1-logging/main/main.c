#include <stdio.h>
#include "esp_log.h"

static const char *tag1 = "LOG_TEST_1";
static const char *tag2 = "LOG_TEST_2";

void app_main(void)
{
    esp_log_level_set(tag2, ESP_LOG_DEBUG);
    ESP_LOGE(tag1, "Este es un error 1");
    ESP_LOGW(tag1, "Este es un warning 1");
    ESP_LOGI(tag1, "Esto es info 1");
    ESP_LOGD(tag1, "Esto es debug 1");
    ESP_LOGV(tag1, "Esto es no es util 1...");
    ESP_LOGE(tag2, "Este es un error 2");
    ESP_LOGW(tag2, "Este es un warning 2");
    ESP_LOGI(tag2, "Esto es info 2");
    ESP_LOGD(tag2, "Esto es debug 2");
    ESP_LOGV(tag2, "Esto es no es util 2...");
}
