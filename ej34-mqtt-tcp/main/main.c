#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"
#include "mqtt_client.h"
#include "api_wifi_station.h"

static const char *tag = "MQTT-TCP";
static const char *ssid = "FV-IOT";
static const char *pwd = "FVVR#iot1980";

static const TickType_t delay_5000_ms = pdMS_TO_TICKS(5000);

static esp_mqtt_client_handle_t client;

static void mqtt_init(void);
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    ESP_ERROR_CHECK(wifi_station_init(ssid, pwd));
    mqtt_init();
    uint32_t value;
    char buffer[10];
    int msg_id;
    while (1)
    {
        value = ((esp_random() % 6) + 1);
        memset(buffer, '\0', sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%" PRIu32, value);
        msg_id = esp_mqtt_client_publish(client, "/dados/dado_1", buffer, 0, 1, 0);
        ESP_LOGI(tag, "sent publish successful, msg_id=%d", msg_id);
        vTaskDelay(delay_5000_ms);
    }
}

static void mqtt_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://test.mosquitto.org:1883",
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(tag, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "/dados/dado_1", 1);
        ESP_LOGI(tag, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(tag, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(tag, "MQTT_EVENT_SUBSCRIBED msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(tag, "MQTT_EVENT_UNSUBSCRIBED");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(tag, "MQTT_EVENT_PUBLISHED");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(tag, "MQTT_EVENT_DATA");
        ESP_LOGI(tag, "TOPIC = %.*s", event->topic_len, event->topic);
        ESP_LOGI(tag, "DATA = %.*s", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(tag, "MQTT_EVENT_ERROR:");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            ESP_LOGE(tag, "%s", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        break;
    }
}