#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include <esp_system.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvm_storage.h"

void configure_nvm (void) {
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
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

char read_nvm_data (char *namespace, char *key) {
    nvs_handle_t nvs_handle;
    char *value = "";
    esp_err_t err;

    size_t key_len = sizeof(*key);

    err = nvs_open(namespace, NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        nvs_get_str(nvs_handle, key, value, &key_len);
    } else {
        printf("nvs_open(): %s\n", esp_err_to_name(err));
    }
    nvs_close(nvs_handle);
    return *value;
}

