/**
 * @file soil_humidity.c
 * @brief The task and function to check humidity of soil by ADC measurement.
 */
#include "soil_humidity.h"

QueueHandle_t QueueSoilHumidity;
SemaphoreHandle_t SemHumidityQueue = NULL;

// #define ADC_HUMMIDITY        GPIO_NUM_36     RTC_GPIO00  ADC1_CH0

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_humidity (void *pvParameters) {
    int voltage;
    int16_t converted_value;
    QueueSoilHumidity = xQueueCreate( 2, sizeof( int16_t ) );
    SemHumidityQueue = xSemaphoreCreateMutex();
    while (1) {
        voltage = adc_read(ADC_CHANNEL_0);
        converted_value = (voltage * 100) / 3300;
        writeQueue(QueueSoilHumidity, SemHumidityQueue, &converted_value);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
