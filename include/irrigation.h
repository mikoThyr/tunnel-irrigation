#ifndef IRRIGATION_H
#define IRRIGATION_H

/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc_types_legacy.h"
#include "driver/gpio.h"
#include "control.h"
#include "task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define PIN_PUMP1_IRRIGATION    GPIO_NUM_25    // RTC_GPIO06

extern TaskHandle_t task_Irrigation;
extern QueueHandle_t QueueSoilHumidity;
extern SemaphoreHandle_t SemHumidityQueue;

void irrigation (void *pvParameters);

#endif
