#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_tls_crypto.h"
#include "nvs.h"
#include "wifi.h"
#include "settings.h"

/**
 *  @brief
 */
httpd_handle_t start_webserver (void);
void start_http_client ( uint8_t temperature, uint8_t humidity );
void stop_webserver (httpd_handle_t server);

#endif
