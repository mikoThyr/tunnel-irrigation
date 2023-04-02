/**
 * @file control.h
 * @brief The tasks and functions to check humidity of soil.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

// Queue to save data gathered from humidity sensor.
extern QueueHandle_t QueueSoilHumidity;

extern SemaphoreHandle_t SemHumidityQueue;

void control_task (void *pvParameters);
