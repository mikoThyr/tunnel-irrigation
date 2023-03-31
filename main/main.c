#include <stdio.h>
//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
//My own files
#include "control.h"
#include "soil_humidity.h"
#include "idle.h"

TaskHandle_t task_SoilHumidity;
TaskHandle_t task_Control;

// WARN: (GROUP_1) structure to check adc voltage of soil humidity
gpio_config_t led_test = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << GPIO_NUM_25),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_ENABLE,
};

void app_main (void) {
    // (GROUP_1)
    gpio_config(&led_test);
    gpio_set_level(GPIO_NUM_25, 1);

    xTaskCreate(control_task, "The one to control others.", 2048, NULL, 1, &task_Control);
    xTaskCreate(check_humidity, "Check the soil humidity.", 2048, NULL, 1, &task_SoilHumidity);
    xTaskCreate( vTaskIdle, "Idle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
}
