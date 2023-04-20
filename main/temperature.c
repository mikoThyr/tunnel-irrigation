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
    QueueAirTemperature = xQueueCreate(3, sizeof( int ));
    SemAirTemperatureQueue = xSemaphoreCreateMutex();
    uint16_t B = log((float)R50/R25)/((1.00/(K0+50))-(1.00/(K0+25)));
    while (1) {
        // int voltage_ref = 0;
        uint32_t voltage = 0;
        int8_t temp = 0;
        uint16_t resistance = 0;
        voltage = adc_read(ADC_CHANNEL_3);
        // voltage_ref = adc_read(ADC_CHANNEL_0);
        resistance = (voltage * 10000) / (2900 - voltage);
        // printf("resistance: %.3f\nvoltage_ref: %d\nvoltage: %d\n", resistance, voltage_ref, voltage);
        temp = (1 / ((log((float)resistance / R25) / B) + (1 / (K0+25)))) - K0;

        xSemaphoreTake(SemAirTemperatureQueue, portMAX_DELAY);
        xQueueSend(QueueAirTemperature, &temp, portMAX_DELAY);
        xSemaphoreGive(SemAirTemperatureQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}

void check_watertemperature (void *pvParameters) {
    QueueWaterTemperature = xQueueCreate(3, sizeof( int ));
    SemWaterTemperatureQueue = xSemaphoreCreateMutex();
    uint16_t B = 3950; //log((float)R50/R25)/((1.00/(K0+50))-(1.00/(K0+25)));
    while (1) {
        uint16_t voltage_ref = 0;
        uint32_t voltage = 0;
        int8_t temp = 0;
        uint16_t resistance = 0;
        voltage = adc_read(ADC_CHANNEL_6);
        resistance = (voltage * 10000) / (2900 - voltage);
        printf("resistance: %.3d   voltage_ref: %d   voltage: %d B: %d\n", resistance, voltage_ref, voltage, B);
        temp = (1 / ((log((float)resistance / R25) / B) + (1 / (K0+25)))) - K0;

        xSemaphoreTake(SemWaterTemperatureQueue, portMAX_DELAY);
        xQueueSend(QueueWaterTemperature, &temp, portMAX_DELAY);
        xSemaphoreGive(SemWaterTemperatureQueue);
        vTaskDelay(3 * 1000 / portTICK_PERIOD_MS);
    }
}
