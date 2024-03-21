#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "api_debounce.h"
#include "api_dado.h"
#include "api_ssd1306.h"

static const TickType_t debounce_time_50_ms = pdMS_TO_TICKS(50);
static const TickType_t delay_10_ms = pdMS_TO_TICKS(10);

static SemaphoreHandle_t sem_dado = NULL;
static QueueHandle_t cola_dado;

static i2c_config_t i2c_conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = GPIO_NUM_21,
    .scl_io_num = GPIO_NUM_22,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000,
};

static void tarea_pulsador(void *pvParameters);
static void tarea_dado(void *pvParameters);
static void tarea_display(void *pvParameters);

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
    sem_dado = xSemaphoreCreateBinary();
    configASSERT(sem_dado != NULL);
    cola_dado = xQueueCreate(10, sizeof(uint32_t));
    configASSERT(cola_dado != NULL);
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
    ret = xTaskCreatePinnedToCore(
        tarea_dado,
        "Tarea Dado",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_display,
        "Tarea Display",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_pulsador(void *pvParameters)
{
    debounce_data_t pulsador = {
        .input = GPIO_NUM_32,
        .debounce_time_ms = debounce_time_50_ms,
        .delay_time_ms = delay_10_ms,
    };
    ESP_ERROR_CHECK(gpio_set_direction(pulsador.input, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_pullup_en(pulsador.input));
    ESP_ERROR_CHECK(gpio_pulldown_dis(pulsador.input));
    debounce_init(&pulsador);
    for (;;)
    {
        debounce_update(&pulsador);
        if (read_key(&pulsador) == PRESSED)
        {
            xSemaphoreGive(sem_dado);
        }
        vTaskDelay(delay_10_ms);
    }
}

static void tarea_dado(void *pvParameters)
{
    uint32_t resultado;
    for (;;)
    {
        if (xSemaphoreTake(sem_dado, portMAX_DELAY))
        {
            resultado = tirar_dado();
            xQueueSend(cola_dado, (void *)&resultado, portMAX_DELAY);
        }
    }
}

static void tarea_display(void *pvParameters)
{
    display_t display = {
        .i2c_num = I2C_NUM_0,
    };
    display_init(&display);
    label_t label_titulo;
    display_string_create(&display, &label_titulo);
    display_string_write(&display, &label_titulo, "Presione el Boton", 0, 0);
    label_t label_valor;
    display_string_create(&display, &label_valor);
    display_string_write(&display, &label_valor, "-", 0, 16);
    uint32_t resultado;
    char texto[10];
    for (;;)
    {
        if (xQueueReceive(cola_dado, &resultado, portMAX_DELAY) == pdTRUE)
        {
            sprintf(texto, "%" PRIu32, resultado);
            display_string_write(&display, &label_valor, texto, 0, 16);
        }
    }
}