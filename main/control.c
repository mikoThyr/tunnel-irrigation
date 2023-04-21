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
int8_t readQueue (QueueHandle_t queue, SemaphoreHandle_t semaphore) {
    int8_t value = 0;
        /* Check if there is any value in the queue. */
    if (uxQueueMessagesWaiting(queue) != 0) {
        xSemaphoreTake(semaphore, portMAX_DELAY);
        xQueueReceive(queue, &value, portMAX_DELAY);
        xSemaphoreGive(semaphore);
        return value;
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
    int8_t adcHumidity;             /*ADC value from queue*/
    int8_t adcAirTemperature;       /*ADC value from queue*/
    int8_t adcWaterTemperature;     /*ADC value from queue*/
    int8_t adcDayTime;              /*ADC value from queue*/
    int8_t timeConfigFlag;          /*Value to gint8_t time_statuset NVM data set in the device config*/

    // bool irrigation_status;

    adc_init();
    vTaskDelay(1 * 1000 / portTICK_PERIOD_MS);
    while (1) {
        timeConfigFlag = get_i8_variable("storage", "time_day");
        if (timeConfigFlag) {
            adcDayTime = readQueue(QueueDayTime, SemDayTimeQueue);
            if (adcDayTime != ESP_FAIL) {
                printf("D    %d | ", adcDayTime);
            }
        }

        adcHumidity = readQueue(QueueSoilHumidity, SemHumidityQueue);
        if (adcHumidity != ESP_FAIL) {
            printf("H(s) %d | ", adcHumidity);
            if (adcHumidity > 80) {
                xTaskNotify(task_Irrigation, 0, eNoAction);
            }
        }
        adcAirTemperature = readQueue(QueueAirTemperature, SemAirTemperatureQueue);
        if (adcAirTemperature != ESP_FAIL) {
            printf("T(a) %d | ", adcAirTemperature);
        }
        adcWaterTemperature = readQueue(QueueWaterTemperature, SemWaterTemperatureQueue);
        if (adcWaterTemperature != ESP_FAIL) {
            printf("T(w) %d\n", adcWaterTemperature);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
