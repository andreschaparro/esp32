#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali_scheme.h"

static const char *tag = "ADC";

static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

void app_main(void)
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
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    while (1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &raw_result));
        ESP_LOGD(tag, "ADC%d Canal[%d] Resutlado: %d", (ADC_UNIT_1 + 1), ADC_CHANNEL_0, raw_result);
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_result, &mv_result));
        ESP_LOGD(tag, "ADC%d Canal[%d] Voltage: %d mV", (ADC_UNIT_1 + 1), ADC_CHANNEL_0, mv_result);
        vTaskDelay(delay_1000_ms);
    }
}
