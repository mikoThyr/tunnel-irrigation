/**
 * @file control.h
 * @brief The tasks and functions to check humidity of soil.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

// Queue to save data gathered from humidity sensor.
extern QueueHandle_t QueueSoilHumidity;
extern QueueHandle_t QueueAirTemperature;
extern QueueHandle_t QueueWaterTemperature;

extern SemaphoreHandle_t SemHumidityQueue;
extern SemaphoreHandle_t SemTemperatureQueue;

void control_task (void *pvParameters);
