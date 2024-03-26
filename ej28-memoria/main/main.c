#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *tag = "MEMORIA";
static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static void tarea_a(void *pvParameters);

void app_main(void)
{
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    BaseType_t ret;
    size_t d_ram = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGD(tag, "Tamaño de la DRAM con el Metodo 1: %i Bytes", d_ram);
    d_ram = xPortGetFreeHeapSize();
    ESP_LOGD(tag, "Tamaño de la DRAM con el Metodo 2: %i Bytes", d_ram);
    size_t i_ram = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGD(tag, "Tamaño de la IRAM: %i Bytes", i_ram);
    size_t d_ram_free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    ESP_LOGD(tag, "Bloque de DRAM libre mas grande: %i Bytes", d_ram_free);
    char *p = NULL;
    p = (char *)pvPortMalloc(d_ram_free);
    configASSERT(p);
    vPortFree(p);
    d_ram_free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    ESP_LOGD(tag, "Bloque de DRAM libre mas grande: %i Bytes", d_ram_free);
    size_t stack_mem = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGD(tag, "Espacio libre en el stack = %i Bytes", stack_mem);
    ret = xTaskCreatePinnedToCore(
        tarea_a,
        "Tarea A",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

static void tarea_a(void *pvParameters)
{
    size_t stack_mem = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGD(tag, "Espacio libre en el stack de la tarea a = %i Bytes", stack_mem);
    char buffer[1024];
    memset(&buffer, '\0', sizeof(buffer));
    stack_mem = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGD(tag, "Espacio libre en el stack de Tarea a = %i Bytes", stack_mem);
    size_t d_ram_free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    ESP_LOGD(tag, "Bloque de DRAM libre mas grande: %i Bytes", d_ram_free);
    char *p = NULL;
    p = (char *)pvPortMalloc(d_ram_free);
    if (p == NULL)
    {
        ESP_LOGE(tag, "No se pudo reservar la memoria DRAM");
    }
    else
    {
        ESP_LOGD(tag, "Se pudo reservar la memoria DRAM");
    }
    vPortFree(p);
    d_ram_free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    ESP_LOGD(tag, "Bloque de DRAM libre mas grande: %i Bytes", d_ram_free);
    size_t free_heap = xPortGetFreeHeapSize();
    size_t min_free_heap = xPortGetMinimumEverFreeHeapSize();
    ESP_LOGD(tag, "Heap libre: %i Bytes", free_heap);
    ESP_LOGD(tag, "Minimo heap libre desde que inicio la ejecucion: %i Bytes", min_free_heap);
    size_t spi_ram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    ESP_LOGD(tag, "Tamaño del RAM externa: %i Bytes", spi_ram);
    for (;;)
    {
        vTaskDelay(delay_1000_ms);
    }
}