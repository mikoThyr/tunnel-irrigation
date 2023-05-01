
#include "task.h"

TaskHandle_t task_SoilHumidity;
TaskHandle_t task_Control;
TaskHandle_t task_AirTemperature;
TaskHandle_t task_WaterTemperature;
TaskHandle_t task_DayTime;
TaskHandle_t task_WaterLevel;
TaskHandle_t task_ButtonRunAP;
TaskHandle_t task_Irrigation;

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

void run_tasks (void) {
    xTaskCreate(check_humidity, "Check humidity of the soil.", 2048, NULL, 1, &task_SoilHumidity);
    xTaskCreate(check_airtemperature, "Check temperature of the air.", 2048, NULL, 1, &task_AirTemperature);
    xTaskCreate(check_watertemperature, "Check temperature of the water.", 2048, NULL, 1, &task_WaterTemperature);
    xTaskCreate(check_daytime, "Check time of day.", 2048, NULL, 1, &task_DayTime);
    xTaskCreate(check_water, "Check water level.", 2048, NULL, 1, &task_WaterLevel);
    xTaskCreate(irrigation, "Run soil wattering.", 2048, NULL, 1, &task_Irrigation);
    xTaskCreate(control_task, "The one to control others.", 4096, NULL, 1, &task_Control);
    xTaskCreate(vTaskIdle, "Idle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

void run_set_task (void) {
    xTaskCreate(button_run_ap, "Run wifi in ap mode.", 4096, NULL, 5, &task_ButtonRunAP);
}

void run_ulp (void) {
    ulp_run(&ulp_entry - RTC_SLOW_MEM);
    esp_sleep_enable_ulp_wakeup();
    esp_deep_sleep_start();
}
