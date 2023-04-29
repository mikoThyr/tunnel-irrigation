/**
 * @file temperature.c
 * @brief   The task to check the value of the water and air temperature by ADC.
 *          The measured values by ADC are sending to the queue.
 */
#include "temperature.h"
#include "math.h"

#define R25     10000   /**< Resistance [Ohm] of the termistor in the 25 [C] */
#define R50     4100    /**< Resistance [Ohm] of the termistor in the 50 [C] */
#define R100    1000    /**< Resistance [Ohm] of the termistor in the 100 [C] */
#define K0      273.15

QueueHandle_t QueueAirTemperature;
QueueHandle_t QueueWaterTemperature;
SemaphoreHandle_t SemAirTemperatureQueue = NULL;
SemaphoreHandle_t SemWaterTemperatureQueue = NULL;

/**
 * @brief   Task to check air temperature by adc.
 */
void check_airtemperature (void *pvParameters) {
    uint32_t voltage = 0;
    int16_t temp = 0;
    uint16_t resistance = 0;
    uint32_t voltage_ref = 0;

    QueueAirTemperature = xQueueCreate(2, sizeof( int16_t ));
    SemAirTemperatureQueue = xSemaphoreCreateMutex();
    uint16_t B = log((float)R50/R25)/(((float)1/(K0+50))-((float)1/(K0+25)));
    while (1) {
        voltage = adc_read(ADC_CHANNEL_3);
        voltage_ref = adc_read(ADC_CHANNEL_4);

        resistance = (voltage * 10000) / (voltage_ref - voltage + 200);
        temp = (1 / ((log((float)resistance / R25) / B) + (1 / (K0+25)))) - K0;
        writeQueue(QueueAirTemperature, SemAirTemperatureQueue, &temp);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief   Task to check water temperature by adc.
 */
void check_watertemperature (void *pvParameters) {
    uint32_t voltage = 0;
    int16_t temp = 0;
    uint16_t resistance = 0;
    uint32_t voltage_ref = 0;

    QueueWaterTemperature = xQueueCreate(2, sizeof( int16_t ));
    SemWaterTemperatureQueue = xSemaphoreCreateMutex();
    uint16_t B = 3950; //log((float)R50/R25)/((1.00/(K0+50))-(1.00/(K0+25)));
    while (1) {
        voltage = adc_read(ADC_CHANNEL_6);
        voltage_ref = adc_read(ADC_CHANNEL_4);

        resistance = (voltage * 10000) / (voltage_ref - voltage + 150);
        temp = (1 / ((log((float)resistance / R25) / B) + (1 / (K0+25)))) - K0;
        writeQueue(QueueWaterTemperature, SemWaterTemperatureQueue, &temp);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
