#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "hal/adc_types.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

void adc_init (void);

int adc_read (adc_channel_t channel);

#endif
