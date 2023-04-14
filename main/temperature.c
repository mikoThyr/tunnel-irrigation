/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include "temperature.h"

QueueHandle_t QueueAirTemperature;
QueueHandle_t QueueWaterTemperature;
SemaphoreHandle_t SemAirTemperatureQueue = NULL;
SemaphoreHandle_t SemWaterTemperatureQueue = NULL;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_airtemperature (void *pvParameters) {
    QueueAirTemperature = xQueueCreate(3, sizeof( int ));
    SemAirTemperatureQueue = xSemaphoreCreateMutex();
    while (1) {
        int voltage = adc_read(ADC_CHANNEL_3);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemAirTemperatureQueue, portMAX_DELAY);
        xQueueSend(QueueAirTemperature, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemAirTemperatureQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}

void check_watertemperature (void *pvParameters) {
    QueueWaterTemperature = xQueueCreate(3, sizeof( int ));
    SemWaterTemperatureQueue = xSemaphoreCreateMutex();
    while (1) {
        int voltage = adc_read(ADC_CHANNEL_6);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemWaterTemperatureQueue, portMAX_DELAY);
        xQueueSend(QueueWaterTemperature, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemWaterTemperatureQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}
