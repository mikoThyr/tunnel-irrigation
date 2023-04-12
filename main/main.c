/**
 * @file main.c
 * @brief The tasks and functions to check humidity of soil.
 */

#include <stdio.h>
#include <string.h>
/* FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp32/ulp.h"
#include "hal/rtc_io_types.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "ulp_common.h"
#include "ulp_main.h"
#include "driver/rtc_io.h"
#include "ulp_main.h"
#include "esp_sleep.h"
#include "esp_log.h"
/* My own files */
#include "control.h"
#include "soil_humidity.h"
#include "temperature.h"
#include "wifi.h"
#include "idle.h"
#include "nvm_storage.h"
#include "settings.h"

#define LED_PIN         GPIO_NUM_33 // RTC_GPIO08
#define ADC_AIRTEMP     GPIO_NUM_25 // RTC_GPIO06   ADC2_CH8
#define ADC_HUMMIDITY   GPIO_NUM_26 // RTC_GPIO07   ADC2_CH9


extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

TaskHandle_t task_SoilHumidity;
TaskHandle_t task_Control;
TaskHandle_t task_AirTemperature;
TaskHandle_t task_WaterTemperature;

void run_tasks (void);
void run_ulp (void);


void app_main (void) {
    program_mode_t dev_mode;
    configure_nvm();
    set_global_variables();

    dev_mode = NORMAL; // check_one_variable("storage", "mode");
    if (dev_mode == NORMAL) {
        configure_wifi();
        run_tasks();
    } else if (dev_mode == ECO) {
        run_ulp();
    } else {
        printf("Mode status: %d\n", dev_mode);
    }
}

void run_tasks (void) {
    xTaskCreate(control_task, "The one to control others.", 2048, NULL, 2, &task_Control);
    xTaskCreate(check_humidity, "Check humidity of the soil.", 1024, NULL, 1, &task_SoilHumidity);
    xTaskCreate(check_airtemperature, "Check temperature of the air.", 1024, NULL, 1, &task_AirTemperature);
    xTaskCreate(check_watertemperature, "Check temperature of the water.", 1024, NULL, 1, &task_WaterTemperature);
    xTaskCreate(vTaskIdle, "Idle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

void run_ulp (void) {
    rtc_gpio_init(LED_PIN);
    rtc_gpio_pulldown_en(LED_PIN);
    rtc_gpio_set_direction(LED_PIN, RTC_GPIO_MODE_OUTPUT_ONLY);

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    if (cause != ESP_SLEEP_WAKEUP_TIMER) { // ESP_SLEEP_WAKEUP_ULP) {
        ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));
        printf("ULP-coprocessor\n");
    } else {
        printf("ulp_day_flag: %d\n", ulp_day_flag & UINT16_MAX);
        vTaskDelay(5 * 1000 / portTICK_PERIOD_MS);
    }
    ulp_run(&ulp_entry - RTC_SLOW_MEM);
    esp_sleep_enable_timer_wakeup(10 * 1000 * 1000);
    // esp_sleep_enable_ulp_wakeup();
    esp_deep_sleep_start();
}
