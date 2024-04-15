#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_server.h"
#include "api_wifi_ap.h"

static const char *tag = "WEB-SERVER";
static const char *ap_ssid = "wlan-esp32";
static const char *ap_pwd = "FVSC#iot2018";

static httpd_handle_t webserver = NULL;

static httpd_handle_t webserver_init(void);
static esp_err_t root_get_handler(httpd_req_t *req);

static const httpd_uri_t root_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler,
    .user_ctx = NULL,
};

void app_main(void)
{
    wifi_ap_init(ap_ssid, ap_pwd);
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
