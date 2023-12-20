#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char *tag = "SEMAFOROS";

static const TickType_t delay_10_ms = pdMS_TO_TICKS(10);
static const gpio_num_t pulsador = GPIO_NUM_32;
static const gpio_num_t led = GPIO_NUM_2;

static SemaphoreHandle_t sem_bin = NULL;
static TaskHandle_t handle_tarea_pulsador = NULL;
static TaskHandle_t handle_tarea_led = NULL;

static void pulsador_isr_handler(void *args);
static void tarea_pulsador(void *pvParameters);
static void tarea_led(void *pvParameters);

void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    sem_bin = xSemaphoreCreateBinary();
    configASSERT(sem_bin != NULL);
    ret = xTaskCreatePinnedToCore(
        tarea_pulsador,
        "Tarea Pulsador",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_pulsador,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_led,
        "Tarea Led",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_led,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void IRAM_ATTR pulsador_isr_handler(void *args)
{
    BaseType_t despertar_tarea = pdFALSE;
    xSemaphoreGiveFromISR(sem_bin, &despertar_tarea);
    if (despertar_tarea == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

static void tarea_pulsador(void *pvParameters)
{
    ESP_ERROR_CHECK(gpio_set_direction(pulsador, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_pullup_en(pulsador));
    ESP_ERROR_CHECK(gpio_pulldown_dis(pulsador));
    ESP_ERROR_CHECK(gpio_set_intr_type(pulsador, GPIO_INTR_NEGEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(pulsador, pulsador_isr_handler, NULL));
    for (;;)
    {
        vTaskDelay(delay_10_ms);
    }
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
            ESP_LOGD(tag, "Se apreto el pulsador a los %" PRIu32 " ticks", xTaskGetTickCount());
        }
    }
}
