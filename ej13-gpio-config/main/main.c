#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char *tag = "GPIO_CONFIG";

static const TickType_t delay_50_ms = pdMS_TO_TICKS(50);

static const gpio_num_t pulsador = GPIO_NUM_32;
static const gpio_num_t led = GPIO_NUM_2;

static QueueHandle_t cola;

static void pulsador_isr_handler(void *args);
static void tarea_pulsador(void *pvParameters);

void app_main(void)
{

    gpio_config_t io_cfg = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pin_bit_mask = (1ULL << pulsador),
    };
    ESP_ERROR_CHECK(gpio_config(&io_cfg));
    io_cfg.intr_type = GPIO_INTR_DISABLE;
    io_cfg.mode = GPIO_MODE_OUTPUT;
    io_cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    io_cfg.pin_bit_mask = (1ULL << led);
    ESP_ERROR_CHECK(gpio_config(&io_cfg));
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    BaseType_t ret;
    ret = xTaskCreatePinnedToCore(
        tarea_pulsador,
        "Tarea Pulsador",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    cola = xQueueCreate(10, sizeof(uint32_t));
    configASSERT(cola != NULL);
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(pulsador, pulsador_isr_handler, (void *)pulsador));
    vTaskDelete(NULL);
}

static void IRAM_ATTR pulsador_isr_handler(void *args)
{
    uint32_t num_gpio = (uint32_t)args;
    xQueueSendFromISR(cola, (void *)&num_gpio, (TickType_t)0);
}

static void tarea_pulsador(void *pvParameters)
{
    uint32_t num_gpio;
    uint32_t estado_led = 0;
    for (;;)
    {
        if (xQueueReceive(cola, &num_gpio, (TickType_t)5) == pdTRUE)
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
            ESP_LOGD(tag, "Se apreto el pulsador %" PRIu32 " a los %" PRIu32 " ticks", num_gpio, xTaskGetTickCount());
        }
        vTaskDelay(delay_50_ms);
    }
}