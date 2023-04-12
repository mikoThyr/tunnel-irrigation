/**
 *  @file
 */
#include <stdlib.h>

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
int8_t check_variable (char namespace[], char key[], int8_t value, set_value_t flag);

int8_t check_one_variable (char namespace[], char key[]);

void set_global_variables (void);
