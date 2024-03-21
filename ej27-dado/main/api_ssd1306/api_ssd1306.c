#include <stdio.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/i2c.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "api_ssd1306.h"

void display_init(display_t *disp_data)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = 0x3C,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .dc_bit_offset = 6,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)disp_data->i2c_num, &io_config, &io_handle));
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = 128 * 64,
        .double_buffer = true,
        .hres = 128,
        .vres = 64,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }};
    disp_data->disp = lvgl_port_add_disp(&disp_cfg);
    lv_disp_set_rotation(disp_data->disp, LV_DISP_ROT_NONE);
    if (lvgl_port_lock(0))
    {
        disp_data->scr = lv_disp_get_scr_act(disp_data->disp);
        lvgl_port_unlock();
    }
}

void display_string_create(display_t *disp_data, label_t *label_data)
{
    if (lvgl_port_lock(0))
    {
        label_data->label = lv_label_create(disp_data->scr);
        lv_label_set_long_mode(label_data->label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lvgl_port_unlock();
    }
}

void display_string_write(display_t *disp_data, label_t *label_data, const char *str, lv_coord_t pos_x, lv_coord_t pos_y)
{
    if (lvgl_port_lock(0))
    {
        lv_label_set_text(label_data->label, str);
        lv_obj_set_width(label_data->label, disp_data->disp->driver->hor_res);
        lv_obj_align(label_data->label, LV_ALIGN_TOP_MID, pos_x, pos_y);
        lvgl_port_unlock();
    }
}
