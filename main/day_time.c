/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include "day_time.h"

QueueHandle_t QueueDayTime;
SemaphoreHandle_t SemDayTimeQueue = NULL;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_daytime (void *pvParameters) {
    QueueDayTime = xQueueCreate(3, sizeof( int ));
    SemDayTimeQueue = xSemaphoreCreateMutex();
    while (1) {
        int voltage = adc_read(ADC_CHANNEL_7);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemDayTimeQueue, portMAX_DELAY);
        xQueueSend(QueueDayTime, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemDayTimeQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}

