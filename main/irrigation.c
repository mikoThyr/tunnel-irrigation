/**
 * @file    irrigation.c
 * @brief   Task controlled by "control task". Task to control the pump and
 *          watering time.
 */
#include "irrigation.h"


// #define PIN_PUMP1_IRRIGATION GPIO_NUM_25     RTC_GPIO06

SemaphoreHandle_t SemIrrigation = NULL;

gpio_config_t io_pump_config = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << PIN_PUMP1_IRRIGATION),
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
};

/**
 * @brief   Task to control the pump and watering time. Task by checking
 *          humidity measurements try to fit optional watering time.
 */
void irrigation (void *pvParameters) {
    gpio_config(&io_pump_config);
    int16_t adcHumidity;                                                        // adc measurement
    int16_t last_humidity;                                                      // to remember the last measurement
    uint8_t hum_upper;                                                          // user upper elvel of the humidity
    int8_t diff;                                                                // indicate different between last and nwe measurement
    uint8_t watering_time = 30;                                                 // watering time which will be changed

    hum_upper = get_i8_variable("storDev", "hum_high");
    last_humidity = hum_upper;
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        gpio_set_level(PIN_PUMP1_IRRIGATION, 1);                                // Start watering
        printf("Pump 1 (watering): START.\n");
        vTaskDelay(watering_time * 1000 / portTICK_PERIOD_MS);
        printf("Pump 1 (watering): STOP.\n");
        gpio_set_level(PIN_PUMP1_IRRIGATION, 0);                                // Stop watering

        vTaskDelay(10 * 60 * 1000 / portTICK_PERIOD_MS);                        // Wait 10 min
        if (readQueue(QueueSoilHumidity, SemHumidityQueue, &adcHumidity) == ESP_OK){
            diff = adcHumidity - last_humidity;                                 // Different between last and actual measurement
            if (adcHumidity != hum_upper) {                                     // If measured humidity is different than user humidity
                if (diff < 0) {                                                 // Increase time if watering time was to short
                    watering_time += 5;
                } else if (diff > 0) {                                          // Decrease time if watering time was to long
                    watering_time -= 5;
                }
                last_humidity = adcHumidity;
            }
        }
        printf("Next watering time: %d [sec]\n", watering_time);
    }
}
