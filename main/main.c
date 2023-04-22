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
#include "day_time.h"
#include "idle.h"
#include "settings.h"
#include "wifi.h"
#include "water_level.h"
#include "http.h"
#include "ap_mode.h"
#include "irrigation.h"

// #define ADC_HUMMIDITY        GPIO_NUM_36     RTC_GPIO00  ADC1_CH0
// #define ADC_AIRTEMP          GPIO_NUM_39     RTC_GPIO03  ADC1_CH3
// #define ADC_WATERTEMP        GPIO_NUM_34     RTC_GPIO04  ADC1_CH6
// #define ADC_DAYTIME          GPIO_NUM_35     RTC_GPIO05  ADC1_CH7
// #define ADC_REF              GPIO_NUM_32     RTC_GPIO05  ADC1_CH4
// #define PIN_PUMP1_IRRIGATION GPIO_NUM_25     RTC_GPIO06
// #define PIN_WATERLEVEL       GPIO_NUM_26     RTC_GPIO07
// #define PIN_AP_MODE          GPIO_NUM_27

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

TaskHandle_t task_SoilHumidity;
TaskHandle_t task_Control;
TaskHandle_t task_AirTemperature;
TaskHandle_t task_WaterTemperature;
TaskHandle_t task_DayTime;
TaskHandle_t task_WaterLevel;
TaskHandle_t task_ButtonRunAP;
TaskHandle_t task_Irrigation;

void run_tasks (void);
void run_ulp (void);


void app_main (void) {
    esp_err_t error_status;
    int8_t device_mode;
    int8_t wifi_status;
    configure_nvm();
    set_global_variables();

    /*  A ECO mode has not yet been programmed so function set_global_variables
        after first device start run program in default. */
    device_mode = get_i8_variable("storage", "mode");
    if (device_mode == NORMAL) {
        configure_wifi();
        wifi_status = get_i8_variable("storage", "wifi");
        if (wifi_status == WIFI_ON) {
            error_status = start_wifi(WIFI_MODE_STA);
            if (error_status == ESP_OK) {
                // start_http_client(temperature, humidity);
                printf("Start HTTP.\n");
            }
        }
        run_tasks();
    } else if (device_mode == ECO) {
        run_ulp();
    } else {
        printf("Mode status: %d\n", device_mode);
    }
}

void run_tasks (void) {
    xTaskCreatePinnedToCore(control_task, "The one to control others.", 4096, NULL, 2, &task_Control, 1);
    xTaskCreatePinnedToCore(check_humidity, "Check humidity of the soil.", 1024, NULL, 1, &task_SoilHumidity, 1);
    xTaskCreatePinnedToCore(check_airtemperature, "Check temperature of the air.", 2048, NULL, 1, &task_AirTemperature, 1);
    xTaskCreatePinnedToCore(check_watertemperature, "Check temperature of the water.", 2048, NULL, 1, &task_WaterTemperature, 1);
    xTaskCreatePinnedToCore(check_daytime, "Check time of day.", 1024, NULL, 1, &task_DayTime, 1);
    xTaskCreatePinnedToCore(check_water, "Check water level.", 2048, NULL, 1, &task_WaterLevel, 1);
    xTaskCreate(button_run_ap, "Run wifi in ap mode.", 2048, NULL, 5, &task_ButtonRunAP);
    xTaskCreatePinnedToCore(irrigation, "Run soil wattering.", 2048, NULL, 1, &task_Irrigation, 1);
    xTaskCreate(vTaskIdle, "Idle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

void run_ulp (void) {
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
