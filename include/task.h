#ifndef TASK_H
#define TASK_H

/**
 * @file idle.h
 * @brief The tasks and functions to check humidity of soil.
 */
#include "esp32/ulp.h"
#include "hal/rtc_io_types.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "ulp_common.h"

#include "soil_humidity.h"
#include "temperature.h"
#include "day_time.h"
#include "irrigation.h"
#include "ulp_main.h"
#include "ap_mode.h"
#include "water_level.h"

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void vTaskIdle( void *pvParameters );
void run_tasks (void);
void run_set_task (void);
void run_ulp (void);

#endif
