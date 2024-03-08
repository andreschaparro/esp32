#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/ledc.h"
#include "driver/pulse_cnt.h"

static const char *tag = "FRECUENCIMETRO";

static const uint32_t freq_hz = 525;

static const gpio_num_t salida_generador = GPIO_NUM_33;
static const gpio_num_t entrada_frecuencimetro = GPIO_NUM_27;

static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static TaskHandle_t handle_tarea_generador = NULL;
static TaskHandle_t handle_tarea_frecuencimetro = NULL;

static void tarea_generador(void *pvParameters);
static void tarea_frecuencimetro(void *pvParameters);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    BaseType_t ret;
    ret = xTaskCreatePinnedToCore(
        tarea_generador,
        "tarea_generador",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_generador,
        PRO_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_frecuencimetro,
        "tarea_frecuencimetro",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_frecuencimetro,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_generador(void *pvParameters)
{
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t adc1_init_cfg = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_init_cfg, &adc1_handle));
    adc_oneshot_chan_cfg_t adc1_chan_cfg = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &adc1_chan_cfg));
    int raw_result;
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = freq_hz,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = salida_generador,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    while (1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &raw_result));
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, raw_result));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
        vTaskDelay(delay_1000_ms);
    }
}

static void tarea_frecuencimetro(void *pvParameters)
{
    pcnt_unit_config_t unit_config = {
        .high_limit = 1000,
        .low_limit = -1000,
        .intr_priority = 0,
    };
    pcnt_unit_handle_t pcnt_unit = NULL;
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));
    pcnt_chan_config_t chan_config = {
        .edge_gpio_num = entrada_frecuencimetro,
        .level_gpio_num = -1,
    };
    pcnt_channel_handle_t pcnt_chan = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_chan));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
    int pulse_count = 0;
    while (1)
    {
        ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
        ESP_LOGD(tag, "Pulsos contados: %d", pulse_count);
        ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
        vTaskDelay(delay_1000_ms);
    }
}