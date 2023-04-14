#ifndef CONTROL_H
#define CONTROL_H
/**
 * @file control.h
 * @brief The tasks and functions to check humidity of soil.
 */
#include <stdlib.h>
#include "esp_timer.h"
#include "ulp_main.h"
#include "esp_wifi.h"
#include "soil_humidity.h"
#include "temperature.h"
#include "day_time.h"
#include "settings.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

extern TaskHandle_t task_SoilHumidity;
extern TaskHandle_t task_Control;
extern TaskHandle_t task_AirTemperature;
extern TaskHandle_t task_WaterTemperature;
extern TaskHandle_t task_DayTime;
extern TaskHandle_t task_WaterLevel;

void control_task (void *pvParameters);

#endif
