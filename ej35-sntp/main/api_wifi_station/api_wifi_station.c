#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "api_wifi_station.h"

static const uint32_t max_retry_num = 5;

static const char *tag = "API-WIFI-STATION";

static const EventBits_t wifi_connected_bit = BIT0;
static const EventBits_t wifi_fail_bit = BIT1;

static uint32_t retry_num = 0;

static EventGroupHandle_t bits_wifi_event_group;

static void nvs_init(void);
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void print_station_disconnected_reason(wifi_err_reason_t wifi_err_reaso);

esp_err_t wifi_station_init(const char *ssid, const char *pwd)
{
    nvs_init();
    bits_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &event_handler,
        NULL,
        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &event_handler,
        NULL,
        NULL));
    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.sta.ssid, ssid, (sizeof(wifi_config.sta.ssid) - 1));
    strncpy((char *)wifi_config.sta.password, pwd, (sizeof(wifi_config.sta.password) - 1));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    EventBits_t bits = xEventGroupWaitBits(
        bits_wifi_event_group,
        wifi_connected_bit | wifi_fail_bit,
        pdTRUE,
        pdFALSE,
        portMAX_DELAY);
    if (bits & wifi_connected_bit)
    {
        ESP_LOGI(tag, "Conectado a ssid: %s password: %s", ssid, pwd);
        return ESP_OK;
    }
    else if (bits & wifi_fail_bit)
    {
        ESP_LOGI(tag, "No se pudo conectar a ssid: %s password: %s", ssid, pwd);
    }
    return ESP_FAIL;
}

static void nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(tag, "Conectando por primera vez...");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        wifi_event_sta_disconnected_t *wifi_event_sta_disconnected = event_data;
        ESP_LOGW(tag, "Fallo la conexion al access point causa:");
        print_station_disconnected_reason((wifi_err_reason_t)wifi_event_sta_disconnected->reason);
        if (retry_num < max_retry_num)
        {
            retry_num++;
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI(tag, "Reconectando al access point...");
        }
        else
        {
            ESP_LOGW(tag, "Superado el numero de intentos de conexion");
            xEventGroupSetBits(bits_wifi_event_group, wifi_fail_bit);
        }
        break;
    case IP_EVENT_STA_GOT_IP:
        retry_num = 0;
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(tag, "IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(bits_wifi_event_group, wifi_connected_bit);
        break;
    default:
        break;
    }
}

static void print_station_disconnected_reason(wifi_err_reason_t wifi_err_reaso)
{
    switch (wifi_err_reaso)
    {
    case WIFI_REASON_UNSPECIFIED:
        ESP_LOGW(tag, "WIFI_REASON_UNSPECIFIED");
        break;
    case WIFI_REASON_AUTH_EXPIRE:
        ESP_LOGW(tag, "WIFI_REASON_AUTH_EXPIRE");
        break;
    case WIFI_REASON_AUTH_LEAVE:
        ESP_LOGW(tag, "WIFI_REASON_AUTH_LEAVE");
        break;
    case WIFI_REASON_ASSOC_EXPIRE:
        ESP_LOGW(tag, "WIFI_REASON_ASSOC_EXPIRE");
        break;
    case WIFI_REASON_ASSOC_TOOMANY:
        ESP_LOGW(tag, "WIFI_REASON_ASSOC_TOOMANY");
        break;
    case WIFI_REASON_NOT_AUTHED:
        ESP_LOGW(tag, "WIFI_REASON_NOT_AUTHED");
        break;
    case WIFI_REASON_NOT_ASSOCED:
        ESP_LOGW(tag, "WIFI_REASON_NOT_ASSOCED");
        break;
    case WIFI_REASON_ASSOC_LEAVE:
        ESP_LOGW(tag, "WIFI_REASON_ASSOC_LEAVE");
        break;
    case WIFI_REASON_ASSOC_NOT_AUTHED:
        ESP_LOGW(tag, "WIFI_REASON_ASSOC_NOT_AUTHED");
        break;
    case WIFI_REASON_DISASSOC_PWRCAP_BAD:
        ESP_LOGW(tag, "WIFI_REASON_DISASSOC_PWRCAP_BAD");
        break;
    case WIFI_REASON_DISASSOC_SUPCHAN_BAD:
        ESP_LOGW(tag, "WIFI_REASON_DISASSOC_SUPCHAN_BAD");
        break;
    case WIFI_REASON_BSS_TRANSITION_DISASSOC:
        ESP_LOGW(tag, "WIFI_REASON_BSS_TRANSITION_DISASSOC");
        break;
    case WIFI_REASON_IE_INVALID:
        ESP_LOGW(tag, "WIFI_REASON_IE_INVALID");
        break;
    case WIFI_REASON_MIC_FAILURE:
        ESP_LOGW(tag, "WIFI_REASON_MIC_FAILURE");
        break;
    case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        ESP_LOGW(tag, "WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT");
        break;
    case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT:
        ESP_LOGW(tag, "WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT");
        break;
    case WIFI_REASON_IE_IN_4WAY_DIFFERS:
        ESP_LOGW(tag, "WIFI_REASON_IE_IN_4WAY_DIFFERS");
        break;
    case WIFI_REASON_GROUP_CIPHER_INVALID:
        ESP_LOGW(tag, "WIFI_REASON_GROUP_CIPHER_INVALID");
        break;
    case WIFI_REASON_PAIRWISE_CIPHER_INVALID:
        ESP_LOGW(tag, "WIFI_REASON_PAIRWISE_CIPHER_INVALID");
        break;
    case WIFI_REASON_AKMP_INVALID:
        ESP_LOGW(tag, "WIFI_REASON_AKMP_INVALID");
        break;
    case WIFI_REASON_UNSUPP_RSN_IE_VERSION:
        ESP_LOGW(tag, "WIFI_REASON_UNSUPP_RSN_IE_VERSION");
        break;
    case WIFI_REASON_INVALID_RSN_IE_CAP:
        ESP_LOGW(tag, "WIFI_REASON_INVALID_RSN_IE_CAP");
        break;
    case WIFI_REASON_802_1X_AUTH_FAILED:
        ESP_LOGW(tag, "WIFI_REASON_802_1X_AUTH_FAILED");
        break;
    case WIFI_REASON_CIPHER_SUITE_REJECTED:
        ESP_LOGW(tag, "WIFI_REASON_CIPHER_SUITE_REJECTED");
        break;
    case WIFI_REASON_TDLS_PEER_UNREACHABLE:
        ESP_LOGW(tag, "WIFI_REASON_TDLS_PEER_UNREACHABLE");
        break;
    case WIFI_REASON_TDLS_UNSPECIFIED:
        ESP_LOGW(tag, "WIFI_REASON_TDLS_UNSPECIFIED");
        break;
    case WIFI_REASON_SSP_REQUESTED_DISASSOC:
        ESP_LOGW(tag, "WIFI_REASON_SSP_REQUESTED_DISASSOC");
        break;
    case WIFI_REASON_NO_SSP_ROAMING_AGREEMENT:
        ESP_LOGW(tag, "WIFI_REASON_NO_SSP_ROAMING_AGREEMENT");
        break;
    case WIFI_REASON_BAD_CIPHER_OR_AKM:
        ESP_LOGW(tag, "WIFI_REASON_BAD_CIPHER_OR_AKM");
        break;
    case WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION:
        ESP_LOGW(tag, "WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION");
        break;
    case WIFI_REASON_SERVICE_CHANGE_PERCLUDES_TS:
        ESP_LOGW(tag, "WIFI_REASON_SERVICE_CHANGE_PERCLUDES_TS");
        break;
    case WIFI_REASON_UNSPECIFIED_QOS:
        ESP_LOGW(tag, "WIFI_REASON_UNSPECIFIED_QOS");
        break;
    case WIFI_REASON_NOT_ENOUGH_BANDWIDTH:
        ESP_LOGW(tag, "WIFI_REASON_NOT_ENOUGH_BANDWIDTH");
        break;
    case WIFI_REASON_MISSING_ACKS:
        ESP_LOGW(tag, "WIFI_REASON_MISSING_ACKS");
        break;
    case WIFI_REASON_EXCEEDED_TXOP:
        ESP_LOGW(tag, "WIFI_REASON_EXCEEDED_TXOP");
        break;
    case WIFI_REASON_STA_LEAVING:
        ESP_LOGW(tag, "WIFI_REASON_STA_LEAVING");
        break;
    case WIFI_REASON_END_BA:
        ESP_LOGW(tag, "WIFI_REASON_END_BA");
        break;
    case WIFI_REASON_UNKNOWN_BA:
        ESP_LOGW(tag, "WIFI_REASON_UNKNOWN_BA");
        break;
    case WIFI_REASON_TIMEOUT:
        ESP_LOGW(tag, "WIFI_REASON_TIMEOUT");
        break;
    case WIFI_REASON_PEER_INITIATED:
        ESP_LOGW(tag, "WIFI_REASON_PEER_INITIATED");
        break;
    case WIFI_REASON_AP_INITIATED:
        ESP_LOGW(tag, "WIFI_REASON_AP_INITIATED");
        break;
    case WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT:
        ESP_LOGW(tag, "WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT");
        break;
    case WIFI_REASON_INVALID_PMKID:
        ESP_LOGW(tag, "WIFI_REASON_INVALID_PMKID");
        break;
    case WIFI_REASON_INVALID_MDE:
        ESP_LOGW(tag, "WIFI_REASON_INVALID_MDE");
        break;
    case WIFI_REASON_INVALID_FTE:
        ESP_LOGW(tag, "WIFI_REASON_INVALID_FTE");
        break;
    case WIFI_REASON_TRANSMISSION_LINK_ESTABLISH_FAILED:
        ESP_LOGW(tag, "WIFI_REASON_TRANSMISSION_LINK_ESTABLISH_FAILED");
        break;
    case WIFI_REASON_ALTERATIVE_CHANNEL_OCCUPIED:
        ESP_LOGW(tag, "WIFI_REASON_ALTERATIVE_CHANNEL_OCCUPIED");
        break;
    case WIFI_REASON_BEACON_TIMEOUT:
        ESP_LOGW(tag, "WIFI_REASON_BEACON_TIMEOUT");
        break;
    case WIFI_REASON_NO_AP_FOUND:
        ESP_LOGW(tag, "WIFI_REASON_NO_AP_FOUND");
        break;
    case WIFI_REASON_AUTH_FAIL:
        ESP_LOGW(tag, "WIFI_REASON_AUTH_FAIL");
        break;
    case WIFI_REASON_ASSOC_FAIL:
        ESP_LOGW(tag, "WIFI_REASON_ASSOC_FAIL");
        break;
    case WIFI_REASON_HANDSHAKE_TIMEOUT:
        ESP_LOGW(tag, "WIFI_REASON_HANDSHAKE_TIMEOUT");
        break;
    case WIFI_REASON_CONNECTION_FAIL:
        ESP_LOGW(tag, "WIFI_REASON_CONNECTION_FAIL");
        break;
    case WIFI_REASON_AP_TSF_RESET:
        ESP_LOGW(tag, "WIFI_REASON_AP_TSF_RESET");
        break;
    case WIFI_REASON_ROAMING:
        ESP_LOGW(tag, "WIFI_REASON_ROAMING");
        break;
    case WIFI_REASON_ASSOC_COMEBACK_TIME_TOO_LONG:
        ESP_LOGW(tag, "WIFI_REASON_ASSOC_COMEBACK_TIME_TOO_LONG");
        break;
    case WIFI_REASON_SA_QUERY_TIMEOUT:
        ESP_LOGW(tag, "WIFI_REASON_SA_QUERY_TIMEOUT");
        break;
    default:
        ESP_LOGW(tag, "UNKNOWN");
        break;
    }
}