#ifndef API_SSD1306_H
#define API_SSD1306_H

#include "driver/i2c.h"
#include "lvgl.h"

typedef struct
{
    i2c_port_t i2c_num;
    lv_disp_t *disp;
    lv_obj_t *scr;
} display_t;

typedef struct
{
    lv_obj_t *label;
} label_t;

void display_init(display_t *disp_data);

void display_string_create(display_t *disp_data, label_t *label_data);

void display_string_write(display_t *disp_data, label_t *label_data, const char *str, lv_coord_t pos_x, lv_coord_t pos_y);

#endif