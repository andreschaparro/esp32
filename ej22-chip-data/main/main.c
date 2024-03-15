#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_mac.h"
#include "esp_flash.h"

static const char *tag = "CHIP-DATA";

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    switch (chip_info.model)
    {
    case CHIP_ESP32:
        ESP_LOGD(tag, "Modelo ESP32");
        break;
    case CHIP_ESP32C2:
        ESP_LOGD(tag, "Modelo ESP32C2");
        break;
    case CHIP_ESP32C3:
        ESP_LOGD(tag, "Modelo ESP32C3");
        break;
    case CHIP_ESP32C6:
        ESP_LOGD(tag, "Modelo ESP32C6");
        break;
    case CHIP_ESP32H2:
        ESP_LOGD(tag, "Modelo ESP32H2");
        break;
    case CHIP_ESP32S2:
        ESP_LOGD(tag, "Modelo ESP32S2");
        break;
    case CHIP_ESP32S3:
        ESP_LOGD(tag, "Modelo ESP32S3");
        break;
    default:
        ESP_LOGD(tag, "Modelo desconocido o corriendo en un simulador");
        break;
    }
    ESP_LOGD(tag, "Tiene memoria flash embebida : %s", chip_info.features & CHIP_FEATURE_EMB_FLASH ? "Si" : "No");
    ESP_LOGD(tag, "Tiene Wifi: %s", chip_info.features & CHIP_FEATURE_WIFI_BGN ? "Si" : "No");
    ESP_LOGD(tag, "Tiene Bluetooth Low Energy : %s", chip_info.features & CHIP_FEATURE_BLE ? "Si" : "No");
    ESP_LOGD(tag, "Tiene Bluetooth tradicional : %s", chip_info.features & CHIP_FEATURE_BT ? "Si" : "No");
    ESP_LOGD(tag, "Tiene IEEE 802.15.4 : %s", chip_info.features & CHIP_FEATURE_IEEE802154 ? "Si" : "No");
    ESP_LOGD(tag, "Tiene psram embebida : %s", chip_info.features & CHIP_FEATURE_EMB_PSRAM ? "Si" : "No");
    ESP_LOGD(tag, "Revision del SOC (formato MXX): %" PRIu16, chip_info.revision);
    ESP_LOGD(tag, "Numero de COREs : %" PRIu8, chip_info.cores);
    uint8_t factory_mac_addr[6] = {0};
    ESP_ERROR_CHECK(esp_read_mac(factory_mac_addr, ESP_MAC_EFUSE_FACTORY));
    ESP_LOGD(tag, "La mac address de fabrica de Espressif es: \"0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\"",
             factory_mac_addr[0],
             factory_mac_addr[1],
             factory_mac_addr[2],
             factory_mac_addr[3],
             factory_mac_addr[4],
             factory_mac_addr[5]);
    uint8_t wifi_sta_mac_addr[6] = {0};
    ESP_ERROR_CHECK(esp_read_mac(wifi_sta_mac_addr, ESP_MAC_WIFI_STA));
    ESP_LOGD(tag, "La mac address de la interfaz WIFI cuando se conecta a un access point es: \"0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\"",
             wifi_sta_mac_addr[0],
             wifi_sta_mac_addr[1],
             wifi_sta_mac_addr[2],
             wifi_sta_mac_addr[3],
             wifi_sta_mac_addr[4],
             wifi_sta_mac_addr[5]);
    uint32_t flash_size = 0;
    ESP_ERROR_CHECK(esp_flash_get_physical_size(esp_flash_default_chip, &flash_size));
    ESP_LOGD(tag, "Tiene %" PRIu32 "MB de memoria flash", flash_size / (1024 * 1024));
    vTaskDelete(NULL);
}
