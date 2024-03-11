/**
 * @file main.c
 * @author chaparroandres@gmail.com
 * @brief  Control de un estacionamiento
 * @version 1.0
 * @date 2024-03-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

typedef enum
{
    ENTRY,
    EXIT,
} event_test_t;

static const event_test_t event_test_array[] = {ENTRY, ENTRY, ENTRY, ENTRY, ENTRY, ENTRY, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT};

static const char *tag = "CONTROL-ESTACIONAMIENTO";

static const TickType_t delay_5000_ms = pdMS_TO_TICKS(5000);

static const uint32_t capacidad_max = 3;

static uint32_t capacidad;
static bool lleno_tarea_a;
static bool hay_lugar_tarea_b;

static SemaphoreHandle_t sem_entry = NULL;
static SemaphoreHandle_t sem_exit = NULL;
static SemaphoreHandle_t sem_continue = NULL;

static SemaphoreHandle_t mux_capacidad = NULL;

static TaskHandle_t handle_tarea_a = NULL;
static TaskHandle_t handle_tarea_b = NULL;
static TaskHandle_t handle_tarea_test = NULL;

static void tarea_a(void *pvParameters);
static void tarea_b(void *pvParameters);
static void tarea_test(void *pvParameters);

/**
 * @brief Inicializacion
 *
 * Creacion de semaforos binarios (recordar que siempre arrancan tomados)
 * Creacion de un mutex
 * Creacion de tareas
 *
 */
void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    ESP_LOGD(tag, "Ejemplo 17: control de estacionamiento running");
    sem_entry = xSemaphoreCreateBinary();
    configASSERT(sem_entry != NULL);
    sem_exit = xSemaphoreCreateBinary();
    configASSERT(sem_exit != NULL);
    sem_continue = xSemaphoreCreateBinary();
    configASSERT(sem_continue != NULL);
    mux_capacidad = xSemaphoreCreateMutex();
    configASSERT(mux_capacidad != NULL);
    ret = xTaskCreatePinnedToCore(
        tarea_a,
        "tarea_a",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_a,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_b,
        "tarea_b",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_b,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    ret = xTaskCreatePinnedToCore(
        tarea_test,
        "tarea_test",
        4096,
        NULL,
        (tskIDLE_PRIORITY + 2U),
        &handle_tarea_test,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

/**
 * @brief Tarea A
 *
 * Monitoreo y control del ingreso de vehiculos en A
 *
 * @param pvParameters
 */
static void tarea_a(void *pvParameters)
{
    capacidad = 0;
    lleno_tarea_a = false;
    for (;;)
    {
        ESP_LOGD(tag, "Tarea A: wait entry");
        if (xSemaphoreTake(sem_entry, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGD(tag, "Tarea A: wait mux_capacidad");
            if (xSemaphoreTake(mux_capacidad, portMAX_DELAY) == pdTRUE)
            {
                capacidad++;
                ESP_LOGD(tag, "Tarea A: capacidad = %" PRIu32, capacidad);
                if (capacidad == capacidad_max)
                {
                    lleno_tarea_a = true;
                }
                ESP_LOGD(tag, "Tarea A: signal mux_capacidad");
                xSemaphoreGive(mux_capacidad);
                if (lleno_tarea_a)
                {
                    lleno_tarea_a = false;
                    ESP_LOGD(tag, "Tarea A: wait continue");
                    if (xSemaphoreTake(sem_continue, portMAX_DELAY) == pdTRUE)
                    {
                    }
                }
            }
        }
    }
}

/**
 * @brief Tarea B
 *
 * Monitoreo y control del egreso de vehiculos en B
 *
 * @param pvParameters
 */
static void tarea_b(void *pvParameters)
{
    hay_lugar_tarea_b = false;
    for (;;)
    {
        ESP_LOGD(tag, "Tarea B: wait exit");
        if (xSemaphoreTake(sem_exit, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGD(tag, "Tarea B: wait mux_capacidad");
            if (xSemaphoreTake(mux_capacidad, portMAX_DELAY) == pdTRUE)
            {
                capacidad--;
                ESP_LOGD(tag, "Tarea B: capacidad = %" PRIu32, capacidad);
                if (capacidad == (capacidad_max - 1))
                {
                    hay_lugar_tarea_b = true;
                }
                ESP_LOGD(tag, "Tarea B: signal mux_capacidad");
                xSemaphoreGive(mux_capacidad);
                if (hay_lugar_tarea_b)
                {
                    hay_lugar_tarea_b = false;
                    ESP_LOGD(tag, "Tarea B: signal continue");
                    xSemaphoreGive(sem_continue);
                }
            }
        }
    }
}

/**
 * @brief Tarea Test
 *
 * Periodicamente envia los estimulos a las tareas A y B
 *
 * @param pvParameters
 */
static void tarea_test(void *pvParameters)
{
    for (;;)
    {
        for (uint32_t i = 0; i < (sizeof(event_test_array) / sizeof(event_test_t)); i++)
        {
            ESP_LOGD(tag, "Tarea Test: runing");
            switch (event_test_array[i])
            {
            case ENTRY:
                ESP_LOGD(tag, "Tarea Test: signal sem_entry");
                xSemaphoreGive(sem_entry);
                break;
            case EXIT:
                ESP_LOGD(tag, "Tarea Test: signal sem_exit");
                xSemaphoreGive(sem_exit);
                break;
            default:
                ESP_LOGD(tag, "Tarea Test: signal error");
                break;
            }
            ESP_LOGD(tag, "Tarea Test: wait 5000ms");
            vTaskDelay(delay_5000_ms);
        }
    }
}