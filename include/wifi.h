#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include <string.h>
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_err.h"
#include <esp_system.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "settings.h"

#define ESP_WIFI_SSID_AP	CONFIG_ESP_WIFI_SSID_AP
#define ESP_WIFI_PASS_AP	CONFIG_ESP_WIFI_PASSWORD_AP
#define ESP_WIFI_CHANNEL	CONFIG_ESP_WIFI_CHANNEL
#define MAX_STA_CONN		CONFIG_ESP_MAX_STA_CONN

#define ESP_WIFI_SSID_ST	CONFIG_ESP_WIFI_SSID_ST
#define ESP_WIFI_PASS_ST	CONFIG_ESP_WIFI_PASSWORD_ST

void configure_wifi(void);
esp_err_t start_wifi (wifi_mode_t set_mode);

#endif
