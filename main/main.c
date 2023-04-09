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
#include "nvs.h"
/* My own files */
#include "control.h"
#include "soil_humidity.h"
#include "temperature.h"
#include "wifi.h"
#include "idle.h"
#include "nvm_storage.h"

#define LED_PIN         GPIO_NUM_33
#define ADC_LIGHT       GPIO_NUM_25
#define ADC_HUMMIDITY   GPIO_NUM_24

typedef enum {
    NORMAL,
    ECO,
} program_mode_t;

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

TaskHandle_t task_SoilHumidity;
TaskHandle_t task_Control;
TaskHandle_t task_WaterTemperature;

/**
 *  @brief  Check what mode is running (eco or normal). After first start
 *          the mode is set in the normal mode.
 *          Eco: start ulp and deep sleep.
 *          Normal: run FreeRTOS.
 */
program_mode_t check_mode (void);

/**
 *  @brief
 */
void run_tasks (void);

/**
 *  @brief
 */
void run_ulp (void);


void app_main (void) {
    program_mode_t dev_mode;
    configure_nvm();
    dev_mode = check_mode();
    if (dev_mode == NORMAL) {
        configure_wifi();
        run_tasks();
    } else if (dev_mode == ECO) {
        run_ulp();
    } else {
        printf("Mode can't be set.");
    }
}

void run_tasks (void) {
    xTaskCreate(control_task, "The one to control others.", 2048, NULL, 1, &task_Control);
    xTaskCreate(check_humidity, "Check the soil humidity.", 2048, NULL, 1, &task_SoilHumidity);
    xTaskCreate(check_temperature, "Check the temperature of water.", 2048, NULL, 2, &task_WaterTemperature);
    xTaskCreate(vTaskIdle, "Idle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

void run_ulp (void) {
    rtc_gpio_init(LED_PIN);
    rtc_gpio_pulldown_en(LED_PIN);
    rtc_gpio_set_direction(LED_PIN, RTC_GPIO_MODE_OUTPUT_ONLY);

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    if (cause != ESP_SLEEP_WAKEUP_ULP) {
        ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));
        printf("ULP-coprocessor\n");
    } else {
        printf("ulp_value: %d\n", ulp_value & UINT16_MAX);
    }
    ulp_run(&ulp_entry - RTC_SLOW_MEM);
    esp_sleep_enable_ulp_wakeup();
    esp_deep_sleep_start();
}

program_mode_t check_mode (void) {
    program_mode_t set_mode;
    char namespace[] = "storage";
    char key[] = "mode";
    char value[] = "normal";
    esp_err_t err;

    size_t key_len = sizeof(namespace);

    nvs_handle_t nvs_handle;
    err = nvs_open( namespace, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        err = nvs_get_str(nvs_handle, key, value, &key_len);
        printf("Get mode(get value): %s\n", esp_err_to_name(err));
        if ((err == ESP_ERR_NVS_INVALID_NAME) || (err == ESP_ERR_NVS_NOT_FOUND)) {
            err = nvs_set_str(nvs_handle, key, value);
            printf("Get mode(set value): %s\n", esp_err_to_name(err));
            err = nvs_commit(nvs_handle);
            printf("Get mode(commit): %s\n", esp_err_to_name(err));
        }
    } else {
        printf("Get mode(nvs_open()): %s\n", esp_err_to_name(err));
    }
    nvs_close(nvs_handle);

    if (strcmp(value, "eco") == 0) {
        set_mode = ECO;
    } else {
        set_mode = NORMAL;
    }
    /* WARNING: If code of eco mode will be written than thile need to be deleted. */
    set_mode = ECO;
    return set_mode;
}
