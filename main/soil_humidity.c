/**
 * @file soil_humidity.c
 * @brief The tasks and functions to check humidity of soil.
 */

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

#include "hal/adc_types.h"
#include "soil_humidity.h"


QueueHandle_t QueueSoilHumidity;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_humidity (void *pvParameters) {
    int adc_raw[1][1];
    // Resorce allocation
    adc_oneshot_unit_handle_t adc_humidity;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_humidity));

    // Unit configuration
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_humidity, ADC_CHANNEL_0, &config));

    while (1) {
        // Read raw resoult
        ESP_ERROR_CHECK(adc_oneshot_read(adc_humidity, ADC_CHANNEL_0, &adc_raw[0][0]));
        xQueueSend(QueueSoilHumidity, &adc_raw[1][1], portMAX_DELAY);
    }
}
