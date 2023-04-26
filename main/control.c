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
esp_err_t readQueue (QueueHandle_t queue, SemaphoreHandle_t semaphore, int16_t *value) {
    /* Check if there is any value in the queue. */
    if ((uxQueueMessagesWaiting(queue) > 0) && (semaphore != NULL)) {
        if (xSemaphoreTake(semaphore, (100/portTICK_PERIOD_MS)) == pdTRUE) {
            xQueueReceive(queue, value, (100/portTICK_PERIOD_MS));
            xSemaphoreGive(semaphore);
            // printf("RECEIVE: %d\n", *value);
            return ESP_OK;
        } else {
            return ESP_FAIL;
        }
    } else {
        // printf("Queue empty.\n");
        return ESP_FAIL;
    }
}

esp_err_t writeQueue (QueueHandle_t queue, SemaphoreHandle_t semaphore, int16_t *value) {
    int16_t null_value;
    if (xSemaphoreTake(semaphore, (100/portTICK_PERIOD_MS)) == pdTRUE) {
        if (uxQueueSpacesAvailable(queue) != 0) {
            xQueueReceive(queue, &null_value, (100/portTICK_PERIOD_MS));
            xQueueSendToFront(queue, value, (100/portTICK_PERIOD_MS));
            xSemaphoreGive(semaphore);
            // printf("SEND: %d\n", *value);
        }
        return ESP_OK;
    } else {
        return ESP_FAIL;
    }
}


/**
 * @brief The control task to gathering data from soil humidity,
 *      temperature water and water level. Also a task control the irrigation
 *      system.
 */
void control_task (void *pvParameters) {
    int16_t adcHumidity = 0;             /*ADC value from queue*/
    int16_t adcWaterTemperature = 0;     /*ADC value from queue*/
    int16_t adcDayTime = 0;              /*ADC value from queue*/
    int8_t user_temp;
    int8_t user_lowhumidity;

    user_lowhumidity = get_i8_variable("storDev", "hum_low");
    user_temp = get_i8_variable("storDev", "temp");

    while (1) {
        readQueue(QueueWaterTemperature, SemWaterTemperatureQueue, &adcWaterTemperature);
        readQueue(QueueSoilHumidity, SemHumidityQueue, &adcHumidity);
        readQueue(QueueDayTime, SemDayTimeQueue, &adcDayTime);
        if ((adcWaterTemperature != -1) && (adcHumidity != -1) && (adcDayTime != -1)) {
            printf("Water %d | Humidity %d | Day %d\n", adcWaterTemperature, adcHumidity, adcDayTime);
            if ((adcWaterTemperature >= user_temp) && (adcHumidity <= user_lowhumidity) && adcDayTime) {
                xTaskNotify(task_Irrigation, 0, eNoAction);
            }
        }
        vTaskDelay(1 * 1000 / portTICK_PERIOD_MS);
    }
}
