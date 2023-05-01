#ifndef AP_MODE_H
#define AP_MODE_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "http.h"
#include "settings.h"
#include "task.h"

#define PIN_AP_MODE     GPIO_NUM_27     // RTC_GPIO17

extern TaskHandle_t task_ButtonRunAP;

void button_run_ap (void *arg);

#endif
