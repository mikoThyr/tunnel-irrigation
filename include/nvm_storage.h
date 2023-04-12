#include "esp_err.h"


void configure_nvm (void);
esp_err_t write_nvm_data (char *namespace, char *key, char *value);
char read_nvm_data (char *namespace, char *key);
