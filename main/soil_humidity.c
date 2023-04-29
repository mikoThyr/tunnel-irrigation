/**
 * @file soil_humidity.c
 * @brief The task to check humidity of soil by ADC measurement.
 */
#include "soil_humidity.h"

QueueHandle_t QueueSoilHumidity;
SemaphoreHandle_t SemHumidityQueue = NULL;

// #define ADC_HUMMIDITY        GPIO_NUM_36     RTC_GPIO00  ADC1_CH0

/**
 * @brief Task to check the soil humidity by adc.
 */
void check_humidity (void *pvParameters) {
    uint32_t voltage;
    int16_t converted_value;
    uint32_t voltage_ref;

    QueueSoilHumidity = xQueueCreate( 2, sizeof( int16_t ) );
    SemHumidityQueue = xSemaphoreCreateMutex();
    while (1) {
        voltage = adc_read(ADC_CHANNEL_0);
        voltage_ref = adc_read(ADC_CHANNEL_4);

        converted_value = (voltage * 100) / voltage_ref;
        writeQueue(QueueSoilHumidity, SemHumidityQueue, &converted_value);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
