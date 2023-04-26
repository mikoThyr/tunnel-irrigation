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
    int8_t user_day;
    int16_t converted_value;
    QueueDayTime = xQueueCreate(2, sizeof( int16_t ));
    SemDayTimeQueue = xSemaphoreCreateMutex();

    user_day = get_i8_variable("storDev", "time_day");

    while (1) {
        // The program have to check is it a day or not. If there is zero in the
        // nvm memory the program can run the pump in the day and night so user_day
        // will be equal 1.
        if (user_day == 0) {
            converted_value = 1;
        } else {
            int voltage = adc_read(ADC_CHANNEL_7);
            converted_value = (voltage * 100) / 3300;
            if (converted_value > 30) {
                converted_value = 1;
            } else {
                converted_value = 0;
            }
        }
        writeQueue(QueueDayTime, SemDayTimeQueue, &converted_value);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

