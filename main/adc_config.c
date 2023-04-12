#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "hal/adc_types.h"
#include "adc_config.h"

// Definicja konfiguracji ADC
#define DEFAULT_VREF    3300    // Domyślne napięcie odniesienia (1.1V)
#define NO_OF_SAMPLES   64      // Liczba próbek ADC

// Inicjalizacja ADC
static esp_adc_cal_characteristics_t *adc_chars; // Struktura kalibracyjna ADC

void adc_init(adc2_channel_t channel) {
    // Konfiguracja ADC
    adc2_config_channel_atten(channel, ADC_ATTEN_DB_11); // Wzmocnienie x3.6 (11 dB)

    // Inicjalizacja kalibracji ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}

// Odczyt wartości ADC
uint32_t adc_read(adc2_channel_t channel) {
    int adc_reading = 0;
    int value = 0;
    for (uint8_t i = 0; i < NO_OF_SAMPLES; i++) {
        adc2_get_raw(channel, ADC_WIDTH_BIT_12, &adc_reading);
        value = value + adc_reading;
    }
    value /= NO_OF_SAMPLES;
    uint32_t voltage = esp_adc_cal_raw_to_voltage(value, adc_chars);
    return voltage;
}

