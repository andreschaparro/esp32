#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"

static const char *tag = "SPIFFS-UPLOAD";
static esp_err_t leer_archivo_spiffs(const char *file_name, char *read_buffer);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(tag, "Error al montar el file system");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(tag, "Error al buscar la particion del SPIFFS");
        }
        else
        {
            ESP_LOGE(tag, "Error : (%s)", esp_err_to_name(ret));
        }
        vTaskDelete(NULL);
    }
    size_t total = 0;
    size_t used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(tag, "Error al obtener la informacion de la paricion de SPIFFS : %s", esp_err_to_name(ret));
        ESP_LOGE(tag, "Formateando ...");
        ESP_ERROR_CHECK(esp_spiffs_format(conf.partition_label));
        vTaskDelete(NULL);
    }
    else
    {
        ESP_LOGI(tag, "Tamaño de la particion: %i Bytes / Utilizado: %i Bytes", total, used);
    }
    if (used > total)
    {
        ret = esp_spiffs_check(conf.partition_label);
        if (ret != ESP_OK)
        {
            ESP_LOGE(tag, "Fallo la verificacion de la consistencia del SPIFFS : %s", esp_err_to_name(ret));
            vTaskDelete(NULL);
        }
        else
        {
            ESP_LOGD(tag, "La verificacion de la consistencia del SPIFFS fue correcta");
        }
    }
    char buffer[100];
    memset(buffer, '\0', sizeof(buffer));
    ESP_ERROR_CHECK(leer_archivo_spiffs("/spiffs/test.txt", buffer));
    ESP_LOGD(tag, "Leido de test.txt : %s", buffer);
    ESP_ERROR_CHECK(esp_vfs_spiffs_unregister(conf.partition_label));
    vTaskDelete(NULL);
}

static esp_err_t leer_archivo_spiffs(const char *file_name, char *read_buffer)
{
    FILE *fd = fopen(file_name, "r");
    if (fd == NULL)
    {
        ESP_LOGE(tag, "Error al leer el archivo");
        return ESP_FAIL;
    }
    uint32_t i = 0;
    while (!feof(fd))
    {
        fread(&read_buffer[i], 1, 1, fd);
        i++;
    }
    fclose(fd);
    return ESP_OK;
}