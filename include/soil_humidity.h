#ifndef SOIL_HUMIDITY_H
#define SOIL_HUMIDITY_H

/**
 * @file soil_humidity.h
 * @brief The tasks and functions to check humidity of soil.
 */
#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc_types_legacy.h"
#include "adc_config.h"
#include "control.h"
#include "task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

extern TaskHandle_t task_SoilHumidity;
extern QueueHandle_t QueueSoilHumidity;
extern SemaphoreHandle_t SemHumidityQueue;
/**
 * @brief The task check the voltage value of soil humidity ADC. After reading
 *      a raw data, a value is converted in the readable form and send to the
 *      queue.
 * @param
 */
void check_humidity (void *pvParameters);

#endif
