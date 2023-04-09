/**
 * @file soil_humidity.c
 * @brief The task and function to check humidity of soil by ADC measurement.
 */

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "hal/adc_types.h"
#include "soil_humidity.h"

extern TaskHandle_t task_SoilHumidity;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_humidity (void *pvParameters) {
    int adc_raw[1][1]; /* Table to take raw data from ADC measurement. */
    int voltage[1][1]; /* Table with converted value from a adc_raw */
    /*  Resorce allocation */
    adc_oneshot_unit_handle_t adc_humidity;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_humidity));

    /* Unit configuration */
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_humidity, ADC_CHANNEL_9, &config));

    /*  Calibration */
    adc_cali_handle_t cali_handle;
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle));

    while (1) {
        /* Read raw resoult */
        ESP_ERROR_CHECK(adc_oneshot_read(adc_humidity, ADC_CHANNEL_9, &adc_raw[0][0]));
        /* Convert raw data to a readable value. */
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, adc_raw[0][0], &voltage[0][0]));
        /* Send a readable value to the queue. */
        xSemaphoreTake(SemHumidityQueue, portMAX_DELAY);
        xQueueSend(QueueSoilHumidity, &voltage[0][0], portMAX_DELAY);
        xSemaphoreGive(SemHumidityQueue);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
