/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "control.h"

void check_temperature (void *pvParameters);
