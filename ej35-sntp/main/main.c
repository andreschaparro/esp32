#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "api_wifi_station.h"

static const uint8_t retry_count = 15;

static const char *tag = "SNTP";
static const char *ssid = "FV-IOT";
static const char *pwd = "FVVR#iot1980";

static char strftime_buf[64];
static struct tm timeinfo = {0};

static void localtime_init(void);
static void print_localtime(void);

void app_main(void)
{
    ESP_ERROR_CHECK(wifi_station_init(ssid, pwd));
    localtime_init();
    print_localtime();
    vTaskDelay(pdMS_TO_TICKS(15 * 1000));
    print_localtime();
    esp_netif_sntp_deinit();
    vTaskDelete(NULL);
}

static void localtime_init(void)
{
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(5, ESP_SNTP_SERVER_LIST("time.windows.com", "0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org", "3.pool.ntp.org"));
    ESP_ERROR_CHECK(esp_netif_sntp_init(&config));
    for (uint8_t i = 1; i <= retry_count; i++)
    {
        if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10 * 1000)) == ESP_OK)
        {
            ESP_LOGI(tag, "Se puede configurar la fecha y hora del sistema");
            break;
        }
        else
        {
            ESP_LOGW(tag, "Esperando para configurar la fecha y hora del sistema... intento %" PRIu8 " de %" PRIu8, i, retry_count);
        }
    }
    setenv("TZ", "ART3", 1);
    tzset();
}

static void print_localtime(void)
{
    time_t now = 0;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(tag, "La fecha y hora en Argentina es: %s", strftime_buf);
}