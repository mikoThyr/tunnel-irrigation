#include "adc_config.h"
#include "esp_err.h"

adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle;

adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT_1,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_11,
};

adc_cali_line_fitting_config_t cali_config = {
    .unit_id = ADC_UNIT_1,
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
};


void adc_init (void) {
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_cali_create_scheme_line_fitting(&cali_config, &adc1_cali_handle));

    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_3, &config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config));
}

int adc_read (adc_channel_t channel) {
    int adc_raw = 0;
    int voltage = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(adc1_handle, channel, &adc_raw));
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw, &voltage));

    return voltage;
}
