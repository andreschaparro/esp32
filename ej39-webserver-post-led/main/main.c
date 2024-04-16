#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_http_server.h"
#include "mdns.h"
#include "cJSON.h"
#include "api_wifi_station.h"

static const char *tag = "WEBSERVER-POST-LED";
static const char *ssid = "FV-IOT";
static const char *pwd = "FVVR#iot1980";

static const gpio_num_t led = GPIO_NUM_2;

static httpd_handle_t webserver = NULL;

static httpd_handle_t webserver_init(void);
static esp_err_t root_get_handler(httpd_req_t *req);
static esp_err_t led_post_handler(httpd_req_t *req);
static void mdns_service_init();

static const httpd_uri_t root_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler,
    .user_ctx = NULL,
};

static const httpd_uri_t led_post_uri = {
    .uri = "/api/led",
    .method = HTTP_POST,
    .handler = led_post_handler,
    .user_ctx = NULL,
};

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(led, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(led, 0));
    ESP_ERROR_CHECK(wifi_station_init(ssid, pwd));
    mdns_service_init();
    webserver = webserver_init();
    assert(webserver != NULL);
    vTaskDelete(NULL);
}

static httpd_handle_t webserver_init(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    ESP_LOGI(tag, "Servidor iniciado en el port: %" PRIu16, config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI(tag, "Registrando handlers para las peticiones");
        ESP_ERROR_CHECK(httpd_register_uri_handler(server, &root_uri));
        ESP_ERROR_CHECK(httpd_register_uri_handler(server, &led_post_uri));
    }
    else
    {
        ESP_LOGE(tag, "Error iniciando el servidor!");
    }
    return server;
}

static esp_err_t root_get_handler(httpd_req_t *req)
{
    const char *resp = "Hola Mundo desde el ESP32!";
    ESP_ERROR_CHECK(httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN));
    return ESP_OK;
}

static void mdns_service_init()
{
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set("esp32-id1"));
    ESP_ERROR_CHECK(mdns_instance_name_set("ESP32 con ID = 1"));
}

static esp_err_t led_post_handler(httpd_req_t *req)
{
    char content[100];
    memset(&content, '\0', sizeof(content));
    size_t recv_size = MIN(req->content_len, sizeof(content));
    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0)
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    cJSON *payload = cJSON_Parse(content);
    cJSON *is_on_json = cJSON_GetObjectItem(payload, "is_on");
    bool is_on = cJSON_IsTrue(is_on_json);
    if (is_on)
    {
        ESP_ERROR_CHECK(gpio_set_level(led, 1));
    }
    else
    {
        ESP_ERROR_CHECK(gpio_set_level(led, 0));
    }
    cJSON_Delete(payload);
    const char *resp = "URI POST Response";
    ESP_ERROR_CHECK(httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN));
    return ESP_OK;
}