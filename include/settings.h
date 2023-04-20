#ifndef SETTINGS_H
#define SETTINGS_H
/**
 *  @file
 */
#include <stdlib.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "ulp_main.h"

#define ERROR_NVM   -1

typedef enum {
    NORMAL,
    ECO,
} program_mode_t;

typedef enum {
    DAYNIGHT,
    DAY,
} time_day_t;

typedef enum {
    WIFI_OFF,
    WIFI_ON,
} wifi_flag_t;

typedef enum {
    NO_SET,
    SET,
} set_value_t;

/**
 *  @brief  Function to read global variables from non-volatile memory. If there
 *          is no set variable in the memory than funtion will set default value.
 *
 *  @param  char namespace: namespace in the nvm
 *          char key:       key in the nvm
 *          int8_t value:   variable which will be set if there is not set in
 *                          the memory.
 *  @return int8_t value:   variable which was get from or which was set in
 *                          the memory.
 */
int8_t check_i8_variable (char namespace[], char key[], int8_t value, set_value_t flag);

int8_t get_i8_variable (char namespace[], char key[]);

esp_err_t check_str_variable (const char* namespace, const char* key, char* value, set_value_t flag);

char* get_str_variable (const char *namespace, const char *key);

void set_global_variables (void);

void configure_nvm (void);

esp_err_t write_nvm_data (char *namespace, char *key, char *value);

#endif
