#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Queue to save data gathered from humidity sensor.
extern QueueHandle_t QueueSoilHumidity;


void control_task (void *pvParameters);
