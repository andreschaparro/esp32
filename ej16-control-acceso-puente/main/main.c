/**
 * @file main.c
 * @author chaparroandres87@gmail.com
 * @brief Control de acceso a un puente de un solo sentido
 * @version 1.0
 * @date 2024-03-08
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
    ENTRY_A,
    ENTRY_B,
    EXIT_A,
    EXIT_B,
} event_test_t;

static const event_test_t event_test_array[] = {ENTRY_B, ENTRY_A, EXIT_B, EXIT_A};

static const char *tag = "CONTROL-ACCESO-PUENTE";

static const TickType_t delay_5000_ms = pdMS_TO_TICKS(5000);

static SemaphoreHandle_t sem_entry_a = NULL;
static SemaphoreHandle_t sem_entry_b = NULL;
static SemaphoreHandle_t sem_exit_a = NULL;
static SemaphoreHandle_t sem_exit_b = NULL;

static SemaphoreHandle_t mux_puente = NULL;

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
    ESP_LOGD(tag, "Ejemplo 16: control de acceso para puente running");
    sem_entry_a = xSemaphoreCreateBinary();
    configASSERT(sem_entry_a != NULL);
    sem_exit_a = xSemaphoreCreateBinary();
    configASSERT(sem_exit_a != NULL);
    sem_entry_b = xSemaphoreCreateBinary();
    configASSERT(sem_entry_b != NULL);
    sem_exit_b = xSemaphoreCreateBinary();
    configASSERT(sem_exit_b != NULL);
    mux_puente = xSemaphoreCreateMutex();
    configASSERT(mux_puente != NULL);
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
 * y la salida por A
 *
 * @param pvParameters
 */
static void tarea_a(void *pvParameters)
{
    ESP_LOGD(tag, "Tarea A: running");
    for (;;)
    {
        ESP_LOGD(tag, "Tarea A: wait sem_entry_a");
        if (xSemaphoreTake(sem_entry_a, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGD(tag, "Tarea A: wait mux_puente");
            if (xSemaphoreTake(mux_puente, portMAX_DELAY) == pdTRUE)
            {
                ESP_LOGD(tag, "Tarea A: wait sem_exit_a");
                if (xSemaphoreTake(sem_exit_a, portMAX_DELAY) == pdTRUE)
                {
                    ESP_LOGD(tag, "Tarea A: signal mux_puente");
                    xSemaphoreGive(mux_puente);
                }
            }
        }
    }
}

/**
 * @brief Tarea B
 *
 * Monitoreo y control del ingreso de vehiculos en B
 * y la salida por B
 *
 * @param pvParameters
 */
static void tarea_b(void *pvParameters)
{
    ESP_LOGD(tag, "Tarea B: running");
    for (;;)
    {
        ESP_LOGD(tag, "Tarea B: wait sem_entry_b");
        if (xSemaphoreTake(sem_entry_b, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGD(tag, "Tarea B: wait mux_puente");
            if (xSemaphoreTake(mux_puente, portMAX_DELAY) == pdTRUE)
            {
                ESP_LOGD(tag, "Tarea B: wait sem_exit_b");
                if (xSemaphoreTake(sem_exit_b, portMAX_DELAY) == pdTRUE)
                {
                    ESP_LOGD(tag, "Tarea B: signal mux_puente");
                    xSemaphoreGive(mux_puente);
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
            case ENTRY_A:
                ESP_LOGD(tag, "Tarea Test: signal sem_entry_a");
                xSemaphoreGive(sem_entry_a);
                break;
            case ENTRY_B:
                ESP_LOGD(tag, "Tarea Test: signal sem_entry_b");
                xSemaphoreGive(sem_entry_b);
                break;
            case EXIT_A:
                ESP_LOGD(tag, "Tarea Test: signal sem_exit_a");
                xSemaphoreGive(sem_exit_a);
                break;
            case EXIT_B:
                ESP_LOGD(tag, "Tarea Test: signal sem_exit_b");
                xSemaphoreGive(sem_exit_b);
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