/**
 * @file soil_humidity.h
 * @brief The tasks and functions to check humidity of soil.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "control.h"

/**
 * @brief The task check the voltage value of soil humidity ADC. After reading
 *      a raw data, a value is converted in the readable form and send to the
 *      queue.
 * @param
 */
void check_humidity (void *pvParameters);
