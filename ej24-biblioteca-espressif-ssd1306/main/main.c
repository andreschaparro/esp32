#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"

static const char *msg = "Hola Mundo!";

void app_main(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = GPIO_NUM_21;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = GPIO_NUM_22;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));
    ssd1306_handle_t ssd1306_dev = NULL;
    ssd1306_dev = ssd1306_create(I2C_NUM_0, SSD1306_I2C_ADDRESS);
    ESP_ERROR_CHECK(ssd1306_refresh_gram(ssd1306_dev));
    ssd1306_clear_screen(ssd1306_dev, 0x00);
    ssd1306_draw_string(ssd1306_dev, 0, 16, (const uint8_t *)msg, 16, 1);
    ESP_ERROR_CHECK(ssd1306_refresh_gram(ssd1306_dev));
    vTaskDelete(NULL);
}
