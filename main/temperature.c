/**
 * @file temperature.h
 * @brief The task to check the value of the water and air.
 */
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"
#include "temperature.h"

#define MAX_DEVICES             (1)
#define DS18B20_RESOLUTION      (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD           (1000)   /* milliseconds */
#define DQ_PIN                  GPIO_NUM_32

gpio_config_t dq_line = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT_OD,
    .pin_bit_mask = (1ULL << DQ_PIN),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_ENABLE,
};

OneWireBus_ROMCode known_device = {
    .fields.family = { 0x28 },
    .fields.serial_number = { 0xff, 0x27, 0xcc, 0x86, 0x16, 0x04 },
    .fields.crc = { 0x5e },
};

void check_temperature (void *pvParameters) {
    gpio_config(&dq_line);
    /* Create a 1-Wire bus, using the RMT timeslot driver */
    OneWireBus * owb;
    owb_rmt_driver_info rmt_driver_info;
    owb = owb_rmt_initialize(&rmt_driver_info, DQ_PIN, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(owb, true);  /* enable CRC check for ROM code */

    // Find all connected devices
    printf("Find devices:\n");
    OneWireBus_ROMCode device_rom_codes[MAX_DEVICES] = {0};
    int num_devices = 0;
    OneWireBus_SearchState search_state = {0};
    bool found = false;
    owb_search_first(owb, &search_state, &found);
    while (found) {
        char rom_code_s[17];
        owb_string_from_rom_code(search_state.rom_code, rom_code_s, sizeof(rom_code_s));
        printf("  %d : %s\n", num_devices, rom_code_s);
        device_rom_codes[num_devices] = search_state.rom_code;
        ++num_devices;
        owb_search_next(owb, &search_state, &found);
    }
    printf("Found %d device%s\n", num_devices, num_devices == 1 ? "" : "s");


    char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
    owb_string_from_rom_code(known_device, rom_code_s, sizeof(rom_code_s));
    bool is_present = false;

    owb_status search_status = owb_verify_rom(owb, known_device, &is_present);
    if (search_status == OWB_STATUS_OK) {
        printf("Device %s is %s\n", rom_code_s, is_present ? "present" : "not present");
    } else {
        printf("An error occurred searching for known device: %d", search_status);
    }
    DS18B20_Info * devices[MAX_DEVICES] = {0};
    for (int i = 0; i < num_devices; ++i) {
        DS18B20_Info * ds18b20_info = ds18b20_malloc();         // heap allocation
        devices[i] = ds18b20_info;
        ds18b20_init(ds18b20_info, owb, device_rom_codes[i]);   // associate with bus and device
        ds18b20_use_crc(ds18b20_info, true);                    // enable CRC check on all reads
        ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION);
    }

    int errors_count[MAX_DEVICES] = {0};
    TickType_t last_wake_time = xTaskGetTickCount();
	while (1) {
        ds18b20_convert_all(owb);

        // In this application all devices use the same resolution,
        // so use the first device to determine the delay
        ds18b20_wait_for_conversion(devices[0]);

        // Read the results immediately after conversion otherwise it may fail
        // (using printf before reading may take too long)
        float readings[MAX_DEVICES] = { 0 };
        DS18B20_ERROR errors[MAX_DEVICES] = { 0 };

        for (int i = 0; i < num_devices; ++i) {
            errors[i] = ds18b20_read_temp(devices[i], &readings[i]);
        }

        // Print results in a separate loop, after all have been read
        for (int i = 0; i < num_devices; ++i) {
            if (errors[i] != DS18B20_OK) {
                ++errors_count[i];
            }
            printf("  %d: %.1f    %d errors\n", i, readings[i], errors_count[i]);
            xSemaphoreTake(SemTemperatureQueue, portMAX_DELAY);
            xQueueSend(QueueAirTemperature, &readings[i], portMAX_DELAY);
            xSemaphoreGive(SemTemperatureQueue);
        }
        vTaskDelayUntil(&last_wake_time, SAMPLE_PERIOD / portTICK_PERIOD_MS);
    }
}
