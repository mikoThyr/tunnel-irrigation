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
#include "driver/rtc_io.h"
#include "ulp_main.h"
#include "esp_sleep.h"
#include "esp_log.h"
/* My own files */
// #include "control.h"
#include "task.h"
// #include "settings.h"
#include "wifi.h"
// #include "http.h"
#include "pwm.h"
#include "ap_mode.h"

// #define ADC_HUMIDITY         GPIO_NUM_36     RTC_GPIO00  ADC1_CH0
// #define ADC_AIRTEMP          GPIO_NUM_39     RTC_GPIO03  ADC1_CH3
// #define ADC_WATERTEMP        GPIO_NUM_34     RTC_GPIO04  ADC1_CH6
// #define ADC_DAYTIME          GPIO_NUM_35     RTC_GPIO05  ADC1_CH7
// #define ADC_REF              GPIO_NUM_32     RTC_GPIO05  ADC1_CH4
// #define PIN_PUMP1_IRRIGATION GPIO_NUM_25     RTC_GPIO06
// #define PIN_WATERLEVEL       GPIO_NUM_26     RTC_GPIO07
// #define PIN_AP_MODE          GPIO_NUM_27     RTC_GPIO17

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");


void app_main (void) {
    int8_t device_mode;
    ulp_day_time = 0;

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    if (cause != ESP_SLEEP_WAKEUP_ULP) {
        ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));
        printf("ULP-coprocessor\n");
    }

    adc_init();
    configure_nvm();
    set_global_variables();
    configure_wifi();
    pwm_init();

    // The eco mode is no ready yet.
    device_mode = get_i8_variable("storDev", "mode");
    // ulp_run_settings indicate taht the button was ushed in the deep sleep.
    if ((device_mode == NORMAL) || ((ulp_run_settings & UINT16_MAX) == 1)) {
        if ((ulp_run_settings & UINT16_MAX) == 1) {
            gpio_intr_enable(PIN_AP_MODE);
            ulp_run_settings = 0;
        }
        run_set_task();
        run_tasks();
    } else if ((device_mode == ECO) && ((ulp_run_settings & UINT16_MAX) != 1)) {
        rtc_gpio_init(PIN_AP_MODE);
        rtc_gpio_pulldown_en(PIN_AP_MODE);
        rtc_gpio_set_direction(PIN_AP_MODE, RTC_GPIO_MODE_INPUT_ONLY);

        run_ulp();
    } else {
        printf("Can't choose a mode.\n");
    }
}
