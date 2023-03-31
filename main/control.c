#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "control.h"

QueueHandle_t QueueSoilHumidity;

int readHumidityQueue (void) {
    int adcValue[1][1] = {0};
    if (uxQueueMessagesWaiting(QueueSoilHumidity) != 0) {
        xQueueReceive(QueueSoilHumidity, &adcValue, portMAX_DELAY);
        /* TODO:
            Value was properly sended and here must be programmed rest.
            */
    } else {
        printf("The soil humidity queue is empty.\n");
    }
    return adcValue[0][0];
}

void control_task (void *pvParameters) {
    int adcVoltage;
    QueueSoilHumidity = xQueueCreate( 3, sizeof( uint32_t ) );

    while (1) {
        adcVoltage = readHumidityQueue();
        printf("adcValue: %d\n", adcVoltage);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
