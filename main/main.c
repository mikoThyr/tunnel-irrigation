#include <stdio.h>
//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
//My own files
#include "control.h"
#include "soil_humidity.h"
#include "water_level.h"
#include "temperature.h"

QueueHandle_t QueueSoilHumidity;

void app_main (void) {
    QueueSoilHumidity = xQueueCreate( 5, sizeof( uint32_t ) );

}
