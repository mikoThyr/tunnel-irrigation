/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include "day_time.h"

// #define ADC_DAYTIME          GPIO_NUM_35     RTC_GPIO05  ADC1_CH7

QueueHandle_t QueueDayTime;
SemaphoreHandle_t SemDayTimeQueue = NULL;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_daytime (void *pvParameters) {
    uint8_t converted_value;
    QueueDayTime = xQueueCreate(3, sizeof( int ));
    SemDayTimeQueue = xSemaphoreCreateMutex();
    while (1) {
        int voltage = adc_read(ADC_CHANNEL_7);
        converted_value = ((float)voltage / 3300) * 100;

        xSemaphoreTake(SemDayTimeQueue, portMAX_DELAY);
        xQueueSend(QueueDayTime, &converted_value, portMAX_DELAY);
        xSemaphoreGive(SemDayTimeQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}

