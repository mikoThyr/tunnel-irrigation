/**
 * @file control.c
 * @brief The file with task to control the irrigation system. Few functions
 *      chceks the values of the soil humidity, temperature and water level and
 *      in the proper moment start the pump.
 */
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

#include "control.h"

/**< The queue stored a voltage value from soil humidity ADC.*/
QueueHandle_t QueueSoilHumidity;
QueueHandle_t QueueAirTemperature;
QueueHandle_t QueueWaterTemperature;

SemaphoreHandle_t SemHumidityQueue = NULL;
SemaphoreHandle_t SemAirTemperatureQueue = NULL;
SemaphoreHandle_t SemWaterTemperatureQueue = NULL;

/**
 * @brief Take the voltage value from soil humidity queue.
 * @return int adcValue: -1: if queue is empty the value of variable is equel -1.
 *                       >0: if there is value in the queue, the adcValue
 *                          variable is equel of this value.
 */
int readHumidityQueue (void) {
    int value = 0;
        /* Check if there is any value in the queue. */
    if (uxQueueMessagesWaiting(QueueSoilHumidity) != 0) {
        xSemaphoreTake(SemHumidityQueue, portMAX_DELAY);
        xQueueReceive(QueueSoilHumidity, &value, portMAX_DELAY);
        xSemaphoreGive(SemHumidityQueue);
        return value;
    } else {
        return -1;
    }
}

int readAirTemperatureQueue (void) {
    int value = 0;
        /* Check if there is any value in the queue. */
    if (uxQueueMessagesWaiting(QueueAirTemperature) != 0) {
        xSemaphoreTake(SemAirTemperatureQueue, portMAX_DELAY);
        xQueueReceive(QueueAirTemperature, &value, portMAX_DELAY);
        xSemaphoreGive(SemAirTemperatureQueue);
        return value;
    } else {
        return -1;
    }
}

int readWaterTemperatureQueue (void) {
    int value = 0;
        /* Check if there is any value in the queue. */
    if (uxQueueMessagesWaiting(QueueWaterTemperature) != 0) {
        xSemaphoreTake(SemWaterTemperatureQueue, portMAX_DELAY);
        xQueueReceive(QueueWaterTemperature, &value, portMAX_DELAY);
        xSemaphoreGive(SemWaterTemperatureQueue);
        return value;
    } else {
        return -1;
    }
}

/**
 * @brief The control task to gathering data from soil humidity,
 *      temperature water and water level. Also a task control the irrigation
 *      system.
 */
void control_task (void *pvParameters) {
    int adcVoltage; /**< ADC voltage value from soil humidity measurement. */
    int adcAirTemperature;
    int adcWaterTemperature;

    /* Initialize the soil humidity queue to transfer datas between the tasks. */
    QueueSoilHumidity = xQueueCreate( 3, sizeof( int ) );
    QueueAirTemperature = xQueueCreate( 3, sizeof( int ) );
    QueueWaterTemperature = xQueueCreate( 3, sizeof( int ) );

    SemHumidityQueue = xSemaphoreCreateMutex();
    SemAirTemperatureQueue = xSemaphoreCreateMutex();
    SemWaterTemperatureQueue = xSemaphoreCreateMutex();

    while (1) {
        adcVoltage = readHumidityQueue();
        if (adcVoltage >= 0) {
            printf("Humidity of the soil (ADC voltage): %d\n", adcVoltage);
        }
        adcAirTemperature = readAirTemperatureQueue();
        if (adcAirTemperature >= 0) {
            printf("Temperature of the air (ADC voltage): %d\n", adcAirTemperature);
        }
        adcWaterTemperature = readWaterTemperatureQueue();
        if (adcWaterTemperature >= 0) {
            printf("Temperature of the water (ADC voltage): %d\n", adcWaterTemperature);
        }


        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
