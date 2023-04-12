/**
 * @file soil_humidity.c
 * @brief The task and function to check humidity of soil by ADC measurement.
 */
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc_types_legacy.h"
#include "soil_humidity.h"
#include "adc_config.h"

/* #define ADC_HUMMIDITY   GPIO_NUM_25 */ // RTC_GPIO07   ADC2_CH8

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_humidity (void *pvParameters) {
    adc_init(ADC2_CHANNEL_8);
    while (1) {
        uint32_t voltage = adc_read(ADC2_CHANNEL_8);

        // Obliczenie temperatury na podstawie napięcia
        // float temp = (voltage - 500.0) / 10.0;
        xSemaphoreTake(SemHumidityQueue, portMAX_DELAY);
        xQueueSend(QueueSoilHumidity, &voltage, portMAX_DELAY);
        xSemaphoreGive(SemHumidityQueue);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
