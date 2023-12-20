#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "driver/gpio.h"

static const char *tag = "SOFTWARE_TIMER";

static const TickType_t tim_period = pdMS_TO_TICKS(1000);
static const gpio_num_t led = GPIO_NUM_2;

static SemaphoreHandle_t sem_bin = NULL;
static TimerHandle_t tim = NULL;

static void tim_callback(TimerHandle_t xTimer);
static void tarea_led(void *pvParameters);

void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    sem_bin = xSemaphoreCreateBinary();
    configASSERT(sem_bin != NULL);
    ret = xTaskCreatePinnedToCore(
        tarea_led,
        "Tarea Led",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        (TaskHandle_t *)NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    tim = xTimerCreate(
        "Timer de Ejemplo",
        tim_period,
        pdTRUE,
        (void *)NULL,
        tim_callback);
    configASSERT(tim != NULL);
    if (xTimerStart(tim, (TickType_t)0) != pdTRUE)
    {
        ESP_LOGE(tag, "No se pudo arrancar el timer");
    }
    vTaskDelete(NULL);
}

static void tim_callback(TimerHandle_t xTimer)
{
    xSemaphoreGive(sem_bin);
}

static void tarea_led(void *pvParameters)
{
    ESP_ERROR_CHECK(gpio_set_direction(led, GPIO_MODE_OUTPUT));
    uint32_t estado_led = 0;
    ESP_ERROR_CHECK(gpio_set_level(led, estado_led));
    for (;;)
    {
        if (xSemaphoreTake(sem_bin, (TickType_t)5) == pdTRUE)
        {
            if (estado_led == 0)
            {
                estado_led = 1;
            }
            else
            {
                estado_led = 0;
            }
            ESP_ERROR_CHECK(gpio_set_level(led, estado_led));
            ESP_LOGD(tag, "Se disparo el timer a los %" PRIu32 " ticks", xTaskGetTickCount());
        }
    }
}