#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

static const char *tag = "HARDWARE-TIMER";

static void periodic_timer_callback(void *arg);
static void oneshot_timer_callback(void *arg);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        .name = "periodic",
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    esp_timer_create_args_t oneshot_timer_args = {
        .callback = &oneshot_timer_callback,
        .arg = (void *)periodic_timer,
        .name = "oneshot",
    };
    esp_timer_handle_t oneshot_timer;
    ESP_ERROR_CHECK(esp_timer_create(&oneshot_timer_args, &oneshot_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500000));
    ESP_ERROR_CHECK(esp_timer_start_once(oneshot_timer, 5000000));
    ESP_LOGD(tag, "Tiempo de inicio: %lld us", esp_timer_get_time());
    vTaskDelete(NULL);
}

static void periodic_timer_callback(void *arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGD(tag, "Llamada al timer periodico: %lld us", time_since_boot);
}

static void oneshot_timer_callback(void *arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGD(tag, "Llamada al timer oneshot: %lld us", time_since_boot);
    esp_timer_handle_t periodic_timer_handle = (esp_timer_handle_t)arg;
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer_handle));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer_handle, 1000000));
    time_since_boot = esp_timer_get_time();
    ESP_LOGD(tag, "Reinicio del timer periodico luego de cambiarle el periodo a 1 s: %lld us",
             time_since_boot);
}
