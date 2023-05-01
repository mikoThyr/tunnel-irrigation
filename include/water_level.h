#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H

/**
 * @file water_level.h
 * @brief The tasks and functions to check humidity of soil.
 */
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "task.h"

#define PIN_WATERLEVEL          GPIO_NUM_26    // RTC_GPIO07

extern TaskHandle_t task_WaterLevel;

/**
 *  @brief
 */
void check_water (void *pvParameters);

#endif
