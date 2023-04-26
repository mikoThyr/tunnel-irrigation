#ifndef DAY_TIME_H
#define DAY_TIME_H
/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc_types_legacy.h"
#include "adc_config.h"
#include "settings.h"
#include "control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t SemDayTimeQueue;

extern QueueHandle_t QueueDayTime;

void check_daytime (void *pvParameters);

#endif
