#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

static const char *tag = "WIFI-SCANNER";
static const uint16_t scan_list_size = 10;

static void print_auth_mode(wifi_auth_mode_t auth_mode);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    esp_err_t ret = nvs_flash_init();
    if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    uint16_t ap_info_count = scan_list_size;
    wifi_ap_record_t ap_info[scan_list_size];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_info_count, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGD(tag, "------------------------------------------------------------------");
    ESP_LOGD(tag, "Total de redes wifi encontradas: %" PRIu16, ap_count);
    ESP_LOGD(tag, "Total de redes wifi de las que se obtuvo informacion: %" PRIu16, ap_info_count);
    ESP_LOGD(tag, "------------------------------------------------------------------");
    for (int i = 0; i < ap_info_count; i++)
    {
        ESP_LOGD(tag, "ssid: %s", ap_info[i].ssid);
        ESP_LOGD(tag, "rssi: %" PRIi8, ap_info[i].rssi);
        print_auth_mode(ap_info[i].authmode);
        ESP_LOGD(tag, "canal: %" PRIu8, ap_info[i].primary);
        ESP_LOGD(tag, "------------------------------------------------------------------");
    }
    vTaskDelete(NULL);
}

static void print_auth_mode(wifi_auth_mode_t auth_mode)
{
    switch (auth_mode)
    {
    case WIFI_AUTH_OPEN:
        ESP_LOGD(tag, "authenticate mode : open");
        break;
    case WIFI_AUTH_OWE:
        ESP_LOGD(tag, "authenticate mode : OWE");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGD(tag, "authenticate mode : WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGD(tag, "authenticate mode : WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGD(tag, "authenticate mode : WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGD(tag, "authenticate mode : WPA_WPA2_PSK");
        break;
    case WIFI_AUTH_ENTERPRISE:
        ESP_LOGD(tag, "authenticate mode : WiFi EAP security");
        break;
    case WIFI_AUTH_WPA3_PSK:
        ESP_LOGD(tag, "authenticate mode : WPA3_PSK");
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        ESP_LOGD(tag, "authenticate mode : WPA2_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA3_ENT_192:
        ESP_LOGD(tag, "Authmode \tWIFI_AUTH_WPA3_ENT_192");
        break;
    case WIFI_AUTH_WAPI_PSK:
        ESP_LOGD(tag, "authenticate mode : WAPI_PSK");
        break;
    default:
        ESP_LOGD(tag, "authenticate mode : UNKNOWN");
        break;
    }
}