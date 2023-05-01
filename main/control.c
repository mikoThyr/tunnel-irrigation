/**
 * @file control.c
 * @brief The file with task to control the irrigation system. Few functions
 *      chceks the values of the soil humidity, temperature and water level and
 *      in the proper moment start the pump.
 */
#include "control.h"

/**
 * @brief   Take the voltage value from queue.
 * @param   queue       Handle to a queue
 *          semaphore   Handle to a semaphore
 *          value       Pointer to a variable where will be sotre a value from
 *                      queue.
 * @return  ESP_OK      If queue isn't empty, semapthore was created and
 *                      semaphore is ready to take.
 *          ESP_FAIL
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

/**
 * @brief   Write the voltage value to the queue. Function checks the free
 *          space in the queue and removes the oldest value if queue is full.
 *          New value is added to the front of the queue.
 * @param   queue       Handle to a queue
 *          semaphore   Handle to a semaphore
 *          value       Pointer to a variable which will be written to the
 *                      queue.
 * @return  ESP_OK      If semapthore was created and it is ready to use.
 *          ESP_FAIL
 */
esp_err_t writeQueue (QueueHandle_t queue, SemaphoreHandle_t semaphore, int16_t *value) {
    int16_t null_value;
    if (semaphore != NULL) {
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
    int16_t adcAirTemperature = 0;
    int8_t user_temp;
    int8_t user_lowhumidity;

    // Take data from the user configuration
    user_lowhumidity = get_i8_variable("storDev", "hum_low");
    user_temp = get_i8_variable("storDev", "temp");

    while (1) {
        xTaskNotify(task_DayTime, 0, eNoAction);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // Take from the queue the actual measurements.
        readQueue(QueueWaterTemperature, SemWaterTemperatureQueue, &adcWaterTemperature);
        readQueue(QueueSoilHumidity, SemHumidityQueue, &adcHumidity);
        readQueue(QueueAirTemperature, SemAirTemperatureQueue, &adcAirTemperature);

        // Check if taken values are free of the bugs.
        if ((adcWaterTemperature != -1) && (adcHumidity != -1)) {
            // printf("Air %d | Water %d | Humidity %d\n", adcAirTemperature, adcWaterTemperature, adcHumidity);
            // Check actual measurements and run pump.
            if ((adcWaterTemperature >= user_temp) && (adcHumidity <= user_lowhumidity)) {
                xTaskNotify(task_Irrigation, 0, eNoAction);
            }
        }
        vTaskDelay(5 * 1000 / portTICK_PERIOD_MS);
    }
}
