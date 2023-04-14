/**
 * @file soil_humidity.c
 * @brief The task and function to check humidity of soil by ADC measurement.
 */
#include "soil_humidity.h"

QueueHandle_t QueueSoilHumidity;
SemaphoreHandle_t SemHumidityQueue = NULL;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_humidity (void *pvParameters) {
    QueueSoilHumidity = xQueueCreate( 3, sizeof( int ) );
    SemHumidityQueue = xSemaphoreCreateMutex();
    while (1) {
        int voltage = adc_read(ADC_CHANNEL_0);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemHumidityQueue, portMAX_DELAY);
        xQueueSend(QueueSoilHumidity, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemHumidityQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}
