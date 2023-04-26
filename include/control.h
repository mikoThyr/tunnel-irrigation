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
#include "esp_err.h"
#include "esp_log.h"
#include "soil_humidity.h"
#include "temperature.h"
#include "day_time.h"
#include "settings.h"
#include "irrigation.h"

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
extern TaskHandle_t task_Irrigation;

extern SemaphoreHandle_t SemHumidityQueue;
extern SemaphoreHandle_t SemWaterTemperatureQueue;
extern SemaphoreHandle_t SemAirTemperatureQueue;
extern SemaphoreHandle_t SemDayTimeQueue;

esp_err_t readQueue (QueueHandle_t queue, SemaphoreHandle_t semaphore, int16_t *value);

esp_err_t writeQueue (QueueHandle_t queue, SemaphoreHandle_t semaphore, int16_t *value);

void control_task (void *pvParameters);

#endif
