#include "water_level.h"

gpio_config_t io_water_config = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL << PIN_WATERLEVEL),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
};

void check_water (void *pvParameters) {
    gpio_config(&io_water_config);
    while (1) {

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
