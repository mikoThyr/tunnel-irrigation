/**
 * @file control.c
 * @brief The file with task to control the irrigation system. Few functions
 *      chceks the values of the soil humidity, temperature and water level and
 *      in the proper moment start the pump.
 */
#include "control.h"

/**
 * @brief Take the voltage value from soil humidity queue.
 * @return int adcValue: -1: if no queue the value will be equel -1.
 *                       >=0: if there is value in the queue, the adcValue
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

int readDayTimeQueue (void) {
    int value = 0;
        /* Check if there is any value in the queue. */
    if (uxQueueMessagesWaiting(QueueDayTime) != 0) {
        xSemaphoreTake(SemDayTimeQueue, portMAX_DELAY);
        xQueueReceive(QueueDayTime, &value, portMAX_DELAY);
        xSemaphoreGive(SemDayTimeQueue);
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
    int adcHumidity;
    int adcAirTemperature;
    int adcWaterTemperature;
    int adcDayTime;
    adc_init();
    vTaskDelay(1 * 1000 / portTICK_PERIOD_MS);
    while (1) {
        int8_t time_status = get_i8_variable("storage", "time_day");
        if (time_status) {
            adcDayTime = readDayTimeQueue();
            if (adcDayTime != -1) {
                printf("D    %d | ", adcDayTime);
            }
        }

        adcHumidity = readHumidityQueue();
        if (adcHumidity != -1) {
            printf("H(s) %d | ", adcHumidity);
        }
        adcAirTemperature = readAirTemperatureQueue();
        if (adcAirTemperature != -1) {
            printf("T(a) %d | ", adcAirTemperature);
        }
        adcWaterTemperature = readWaterTemperatureQueue();
        if (adcWaterTemperature != -1) {
            printf("T(w) %d\n", adcWaterTemperature);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
