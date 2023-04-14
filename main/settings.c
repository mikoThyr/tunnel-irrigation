/**
 *  @files settings.c
 *  @brief
 */
#include "settings.h"

void configure_nvm (void) {
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
}

/**
 *  @brief  Function to read and write global (int8_t) variables from/ to
 *          non-volatile memory. A "flag" argument can be set to: SET or NO_SET.
 *          If SET the "value" argument will be set. Despite NO_SET the "value"
 *          argument will be set if there is no nvm key in the memory (otherwise
 *          it will be skipped).
 *
 *  @param  char namespace: namespace in the nvm
 *          char key:       key in the nvm
 *          int8_t value:   variable which will be set if there is no value
 *                          in the memory.
 *          set_value_t flag: enum with two options: SET and NO_SET.
 *                          If SET then value will be changed in the int8_t value.
 *                          The value in the memory can be empty so despite NO_SET
 *                          the value will be changed in the int8_t value.
 *  @return int8_t value:   variable which was get from or which was set in
 *                          the memory.
 */
int8_t check_i8_variable (char namespace[], char key[], int8_t value, set_value_t flag) {
    esp_err_t err;

    nvs_handle_t nvs_handle;
    err = nvs_open( namespace, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        err = nvs_get_i8(nvs_handle, key, &value);
        printf("Check variable (get_i8): %s\n", esp_err_to_name(err));
        /* ESP_ERR_NVS_NOT_FOUND: if there is no key in the nvm */
        if ((err == ESP_ERR_NVS_NOT_FOUND) || (flag == SET)) {
            err = nvs_set_i8(nvs_handle, key, value);
            printf("Check variable (set_i8): %s\n", esp_err_to_name(err));
            err = nvs_commit(nvs_handle);
            printf("Check variable (commit): %s\n", esp_err_to_name(err));
        }
    } else {
        printf("Check variable (open): %s\n", esp_err_to_name(err));
        value = ERROR_NVM;
    }
    nvs_close(nvs_handle);

    return value;
}

/**
 *  @brief
 *  @param
 *  @return
 */
int8_t get_i8_variable (char namespace[], char key[]) {
    esp_err_t err;
    int8_t value;

    nvs_handle_t nvs_handle;
    err = nvs_open( namespace, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        ESP_ERROR_CHECK(nvs_get_i8(nvs_handle, key, &value));
    } else {
        printf("Check variable (open): %s\n", esp_err_to_name(err));
        value = ERROR_NVM;
    }
    nvs_close(nvs_handle);

    return value;
}

esp_err_t check_str_variable (const char* namespace, const char* key, char* value, set_value_t flag) {
    esp_err_t err;
    nvs_handle_t nvs_handle;
    char data[64] = {0};
    size_t value_len = sizeof(data);// strlen(data) * sizeof(char);
    err = nvs_open(namespace, NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        err = nvs_get_str(nvs_handle, key, data, &value_len);
        printf("Check variable (get_str): %s %s\n", esp_err_to_name(err), data);
        /* ESP_ERR_NVS_NOT_FOUND: if there is no key in the nvm */
        if ((err == ESP_ERR_NVS_NOT_FOUND) || (flag == SET)) {
            err = nvs_set_str(nvs_handle, key, value);
            printf("Check variable (set_str): %s\n", esp_err_to_name(err));
            err = nvs_commit(nvs_handle);
            printf("Check variable (commit): %s\n", esp_err_to_name(err));
        }
    } else {
        printf("Check variable (open): %s\n", esp_err_to_name(err));
    }
    nvs_close(nvs_handle);

    return err;
}

char* get_str_variable (const char *namespace, const char *key) {
    nvs_handle_t nvs_handle;
    char data[64] = {0};
    esp_err_t err;

    size_t data_len = sizeof(data);
    err = nvs_open(namespace, NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        nvs_get_str(nvs_handle, key, data, &data_len);
    } else {
        printf("nvs_open(): %s\n", esp_err_to_name(err));
    }
    nvs_close(nvs_handle);

    char* value = malloc(strlen(data) + 1);
    strcpy(value, data);
    return value;
}

esp_err_t write_nvm_data (char *namespace, char *key, char *value) {
    /* Zapisanie wartości pól do pamięci NVM */
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(namespace, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }
    err = nvs_set_str(nvs_handle, key, value);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }
    nvs_close(nvs_handle);
    return ESP_OK;
}

/**
 *  @brief
 */
void set_global_variables (void) {
    program_mode_t dev_mode;
    dev_mode = check_i8_variable("storage", "mode", NORMAL, NO_SET);
    printf("Program mode (-1:ERROR, 0:NORMAL, 1:ECO): %d\n", dev_mode);

    time_day_t day_flag;
    day_flag = check_i8_variable("storage", "time_day", DAY, NO_SET);
    ulp_day_flag = day_flag;
    printf("Time of day (-1:ERROR, 0:DAY and NIGHT, 1:DAY): %d\n", day_flag);

    wifi_flag_t dev_wifi;
    dev_wifi = check_i8_variable("storage", "wifi", WIFI_OFF, NO_SET);
    printf("Wifi (-1:ERROR, 0:OFF, 1:ON): %d\n", dev_wifi);
    check_str_variable("storage", "ssid", "SiecImperialna1", NO_SET);
    check_str_variable("storage", "pass", "1mPer1UMm", NO_SET);

    uint8_t value;
    value = check_i8_variable("storage", "hum_low", 60, NO_SET);
    printf("Humidity lower level.\n");

    value = check_i8_variable("storage", "hum_high", 70, NO_SET);
    printf("Humidity upper level.\n");

}
