/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc_types_legacy.h"
#include "temperature.h"
#include "adc_config.h"

/* #define ADC_AIRTEMP      GPIO_NUM_26 */ // RTC_GPIO07   ADC2_CH9
/* #define ADC_WATERTEMP    GPIO_NUM_27 */ // RTC_GPIO17   ADC2_CH7

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_airtemperature (void *pvParameters) {
    adc_init(ADC2_CHANNEL_9);
    while (1) {
        uint32_t voltage = adc_read(ADC2_CHANNEL_9);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemAirTemperatureQueue, portMAX_DELAY);
        xQueueSend(QueueAirTemperature, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemAirTemperatureQueue);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void check_watertemperature (void *pvParameters) {
    adc_init(ADC2_CHANNEL_7);
    while (1) {
        uint32_t voltage = adc_read(ADC2_CHANNEL_7);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemWaterTemperatureQueue, portMAX_DELAY);
        xQueueSend(QueueWaterTemperature, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemWaterTemperatureQueue);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
