/**
 * @file soil_humidity.c
 * @brief The task and function to check humidity of soil by ADC measurement.
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
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void irrigation (void *pvParameters) {
    gpio_config(&io_pump_config);

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        gpio_set_level(PIN_PUMP1_IRRIGATION, 1);
        printf("Pump 1 (watering): START.\n");
        vTaskDelay(10 * 1000 / portTICK_PERIOD_MS);
        printf("Pump 1 (watering): STOP.\n");
        gpio_set_level(PIN_PUMP1_IRRIGATION, 0);
    }
}
