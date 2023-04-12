//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "idle.h"

void vTaskIdle( void *pvParameters ) {
    while (1) {
        /* Task should only run when there is no other task ready to run */
        if( uxTaskGetNumberOfTasks() == 1 )
        {
            /* Do some idle work */
            vTaskDelay( pdMS_TO_TICKS(10) );
        }
    }
}
