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

static const char *tag = "CONTROL-ACCESO-PUENTE";

static const TickType_t delay_1000_ms = pdMS_TO_TICKS(1000);

static SemaphoreHandle_t sem_entry_a = NULL;
static SemaphoreHandle_t sem_entry_b = NULL;
static SemaphoreHandle_t sem_exit_a = NULL;
static SemaphoreHandle_t sem_exit_b = NULL;

static TaskHandle_t handle_tarea_a = NULL;
static TaskHandle_t handle_tarea_b = NULL;
static TaskHandle_t handle_tarea_test = NULL;

static SemaphoreHandle_t mux_puente = NULL;

static void tarea_a(void *pvParameters);
static void tarea_b(void *pvParameters);
static void tarea_test(void *pvParameters);

/**
 * @brief Inicializacion
 *
 * Creacion de tareas
 * Creacion de semaforos binarios (recordar que siempre arrancan tomados)
 *
 */
void app_main(void)
{
    BaseType_t ret;
    esp_log_level_set(tag, ESP_LOG_DEBUG);
    sem_entry_a = xSemaphoreCreateBinary();
    configASSERT(sem_entry_a != NULL);
    sem_entry_b = xSemaphoreCreateBinary();
    configASSERT(sem_entry_b != NULL);
    sem_exit_a = xSemaphoreCreateBinary();
    configASSERT(sem_exit_a != NULL);
    sem_exit_b = xSemaphoreCreateBinary();
    configASSERT(sem_exit_b != NULL);
    mux_puente = xSemaphoreCreateMutex();
    configASSERT(mux_puente != NULL);
    ESP_LOGI(tag, "Semaforos en Verde!");
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
        (tskIDLE_PRIORITY + 1U),
        &handle_tarea_test,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);
    vTaskDelete(NULL);
}

/**
 * @brief Tarea A
 *
 * Monitoreo y control del ingreso de vehiculos en A
 * y la salida por B
 *
 * @param pvParameters
 */
static void tarea_a(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(sem_entry_a, (TickType_t)5) == pdTRUE)
        {
            for (;;)
            {
                if (xSemaphoreTake(mux_puente, (TickType_t)5) == pdTRUE)
                {
                    ESP_LOGD(tag, "Vehiculo entro al puente por A");
                    ESP_LOGE(tag, "Semaforos en Rojo!");
                    break;
                }
            }
            for (;;)
            {
                if (xSemaphoreTake(sem_exit_b, (TickType_t)5) == pdTRUE)
                {
                    ESP_LOGD(tag, "Vehiculo salio del puente por B");
                    ESP_LOGI(tag, "Semaforos en Verde!");
                    xSemaphoreGive(mux_puente);
                    break;
                }
            }
        }
    }
}

/**
 * @brief Tarea B
 *
 * Monitoreo y control del ingreso de vehiculos en B
 * y la salida por A
 *
 * @param pvParameters
 */
static void tarea_b(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(sem_entry_b, (TickType_t)5) == pdTRUE)
        {
            for (;;)
            {
                if (xSemaphoreTake(mux_puente, (TickType_t)5) == pdTRUE)
                {
                    ESP_LOGD(tag, "Vehiculo entro al puente por B");
                    ESP_LOGE(tag, "Semaforos en Rojo!");
                    break;
                }
            }
            for (;;)
            {
                if (xSemaphoreTake(sem_exit_a, (TickType_t)5) == pdTRUE)
                {
                    ESP_LOGD(tag, "Vehiculo salio del puente por A");
                    ESP_LOGI(tag, "Semaforos en Verde!");
                    xSemaphoreGive(mux_puente);
                    break;
                }
            }
        }
    }
}

/**
 * @brief Tarea Test
 *
 * Periodicamente envia los estimulos a las tareas a y b
 *
 * @param pvParameters
 */
static void tarea_test(void *pvParameters)
{
    for (;;)
    {
        xSemaphoreGive(sem_entry_a);
        vTaskDelay(delay_1000_ms);
        xSemaphoreGive(sem_entry_b);
        vTaskDelay(delay_1000_ms);
        xSemaphoreGive(sem_exit_a);
        vTaskDelay(delay_1000_ms);
        xSemaphoreGive(sem_exit_b);
        vTaskDelay(delay_1000_ms);
    }
}
