/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include "temperature.h"
#include "math.h"

#define R25     10000
#define R50     4100
#define R100    1000
#define K0      273.15

QueueHandle_t QueueAirTemperature;
QueueHandle_t QueueWaterTemperature;
SemaphoreHandle_t SemAirTemperatureQueue = NULL;
SemaphoreHandle_t SemWaterTemperatureQueue = NULL;

/**
 * @brief Function to check soil humidity. Capacitive sensor is connected to the
 *      mictrocontrller pin which measure the value by ADC.
 * @param
 */
void check_airtemperature (void *pvParameters) {
    uint32_t voltage = 0;
    int16_t temp = 0;
    uint16_t resistance = 0;

    QueueAirTemperature = xQueueCreate(2, sizeof( int16_t ));
    SemAirTemperatureQueue = xSemaphoreCreateMutex();
    uint16_t B = log((float)R50/R25)/(((float)1/(K0+50))-((float)1/(K0+25)));
    while (1) {
        voltage = adc_read(ADC_CHANNEL_3);
        resistance = (voltage * 10000) / (3300 - voltage);
        temp = (1 / ((log((float)resistance / R25) / B) + (1 / (K0+25)))) - K0;
        writeQueue(QueueAirTemperature, SemAirTemperatureQueue, &temp);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void check_watertemperature (void *pvParameters) {
    uint32_t voltage = 0;
    int16_t temp = 0;
    uint16_t resistance = 0;

    QueueWaterTemperature = xQueueCreate(2, sizeof( int16_t ));
    SemWaterTemperatureQueue = xSemaphoreCreateMutex();
    uint16_t B = 3950; //log((float)R50/R25)/((1.00/(K0+50))-(1.00/(K0+25)));
    while (1) {
        voltage = adc_read(ADC_CHANNEL_6);
        resistance = (voltage * 10000) / (3300 - voltage);
        temp = (1 / ((log((float)resistance / R25) / B) + (1 / (K0+25)))) - K0;
        writeQueue(QueueWaterTemperature, SemWaterTemperatureQueue, &temp);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
