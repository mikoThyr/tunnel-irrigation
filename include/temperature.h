#ifndef TEMPERATURE_H
#define TEMPERATURE_H

/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc_types_legacy.h"
#include "math.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "adc_config.h"
#include "control.h"
#include "task.h"

extern TaskHandle_t task_AirTemperature;
extern TaskHandle_t task_WaterTemperature;

extern QueueHandle_t QueueAirTemperature;
extern QueueHandle_t QueueWaterTemperature;

extern SemaphoreHandle_t SemAirTemperatureQueue;
extern SemaphoreHandle_t SemWaterTemperatureQueue;

void check_airtemperature (void *pvParameters);

void check_watertemperature (void *pvParameters);

#endif
