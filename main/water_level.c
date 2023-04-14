#include "water_level.h"

gpio_config_t io_water_config = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL << PIN_WATERLEVEL),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
};

gpio_config_t io_pump_config = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << PIN_TANK_PUMP),
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
};

void check_water (void *pvParameters) {
    gpio_config(&io_water_config);
    gpio_config(&io_pump_config);
    uint8_t status = 0;
    while (1) {
        if (gpio_get_level(PIN_WATERLEVEL) == HIGH) {
            if (!status) {
                gpio_set_level(PIN_TANK_PUMP, HIGH);
                printf("Send signal to add water.\n");
                status = 1;
            }
        } else if (gpio_get_level(PIN_WATERLEVEL) == LOW){
            if (status) {
                gpio_set_level(PIN_TANK_PUMP, LOW);
                status = 0;
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
