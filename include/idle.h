#ifndef IDLE_H
#define IDLE_H

/**
 * @file idle.h
 * @brief The tasks and functions to check humidity of soil.
 */

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void vTaskIdle( void *pvParameters );

#endif
