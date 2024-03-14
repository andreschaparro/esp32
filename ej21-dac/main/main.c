#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/dac_oneshot.h"

static const char *tag = "DAC";

static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static SemaphoreHandle_t sem_bin = NULL;

static void tarea_dac(void *pvParameters);
static void tarea_adc(void *pvParameters);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    sem_bin = xSemaphoreCreateBinary();
    configASSERT(sem_bin != NULL);
    BaseType_t ret;
    ret = xTaskCreatePinnedToCore(
        tarea_dac,
        "Tarea DAC",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_adc,
        "Tarea ADC",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_dac(void *pvParameters)
{
    dac_oneshot_handle_t chan0_handle;
    dac_oneshot_config_t chan0_cfg = {
        .chan_id = DAC_CHAN_0,
    };
    ESP_ERROR_CHECK(dac_oneshot_new_channel(&chan0_cfg, &chan0_handle));
    for (;;)
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            ESP_LOGD(tag, "DAC Valor: %" PRIu32, i);
            ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, i));
            xSemaphoreGive(sem_bin);
            vTaskDelay(delay_1000_ms);
        }
    }
}

static void tarea_adc(void *pvParameters)
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
    adc_cali_handle_t cali_handle = NULL;
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle));
    int raw_result;
    int mv_result;
    for (;;)
    {
        if (xSemaphoreTake(sem_bin, portMAX_DELAY) == pdTRUE)
        {
            ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &raw_result));
            ESP_LOGD(tag, "ADC%d Canal[%d] Resutlado: %d", (ADC_UNIT_1 + 1), ADC_CHANNEL_0, (raw_result >> 4));
            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_result, &mv_result));
            ESP_LOGD(tag, "ADC%d Canal[%d] Voltage: %d mV", (ADC_UNIT_1 + 1), ADC_CHANNEL_0, mv_result);
        }
    }
}