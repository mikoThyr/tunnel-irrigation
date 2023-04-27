/**
 * @file day_time.c
 * @brief   The task to check the time of day. The one of the options is either
 *          watering during the day or during the day and night. The Task
 *          checks the time of the day and indicate that by sending to the
 *          queue either 1 or 0.
 */
#include "day_time.h"

// #define ADC_DAYTIME          GPIO_NUM_35     RTC_GPIO05  ADC1_CH7

QueueHandle_t QueueDayTime;
SemaphoreHandle_t SemDayTimeQueue = NULL;

/**
 * @brief Task to check the time of day.
 */
void check_daytime (void *pvParameters) {
    int8_t user_day;
    int16_t converted_value;
    uint32_t voltage;
    uint32_t voltage_ref;

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
            voltage = adc_read(ADC_CHANNEL_7);
            voltage_ref = adc_read(ADC_CHANNEL_4);

            converted_value = (voltage * 100) / voltage_ref;
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

