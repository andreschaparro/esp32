#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

typedef struct
{
    char ssid[20];
    char pwd[20];
} wifi_credenciales_t;

static const char *tag = "NVS";
static const TickType_t delay_10000_ms = pdMS_TO_TICKS(10 * 1000);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    esp_err_t err = nvs_flash_init();
    if ((err == ESP_ERR_NVS_NO_FREE_PAGES) || (err == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGD(tag, "%s", esp_err_to_name(err));
    }
    else
    {
        uint32_t contador = 0;
        err = nvs_get_u32(my_handle, "contador", &contador);
        switch (err)
        {
        case ESP_OK:
            ESP_LOGD(tag, "Contador = %" PRIu32, contador);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGD(tag, "El objeto contador no esta inicializado");
            break;
        default:
            ESP_LOGD(tag, "%s", esp_err_to_name(err));
            break;
        }
        wifi_credenciales_t mis_credenciales = {
            .ssid = "",
            .pwd = "",
        };
        size_t mis_credenciales_size = sizeof(mis_credenciales);
        esp_err_t result = nvs_get_blob(my_handle, "wifi", (void *)&mis_credenciales, &mis_credenciales_size);
        switch (result)
        {
        case ESP_OK:
            ESP_LOGD(tag, "Wifi: ssid = %s / pwd = %s", mis_credenciales.ssid, mis_credenciales.pwd);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGD(tag, "El objeto wifi credentials no esta inicializado");
            break;
        default:
            ESP_LOGD(tag, "%s", esp_err_to_name(err));
            break;
        }
        contador++;
        ESP_ERROR_CHECK(nvs_set_u32(my_handle, "contador", contador));
        strcpy(mis_credenciales.ssid, "mi_wifi");
        strcpy(mis_credenciales.pwd, "mi_pwd");
        ESP_ERROR_CHECK(nvs_set_blob(my_handle, "wifi", (void *)&mis_credenciales, mis_credenciales_size));
        ESP_ERROR_CHECK(nvs_commit(my_handle));
        nvs_close(my_handle);
    }
    vTaskDelay(delay_10000_ms);
    esp_restart();
}
