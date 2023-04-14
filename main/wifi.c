#include "wifi.h"

static esp_netif_t *netif_ap = NULL;
static esp_netif_t *netif_sta = NULL;


void wifi_ap_mode (void) {
	wifi_config_t wifi_config = {
		.ap = {
			.ssid = ESP_WIFI_SSID_AP,
			.ssid_len = strlen(ESP_WIFI_SSID_AP),
			.channel = ESP_WIFI_CHANNEL,
			.password = ESP_WIFI_PASS_AP,
			.max_connection = MAX_STA_CONN,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK,
			.pmf_cfg = {
				.required = false,
			},
		},
	};

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
}

esp_err_t wifi_sta_mode ( void ) {
    esp_err_t error_status;
    char *value;

    wifi_config_t wifi_config = {};

    value = get_str_variable ( "storage", "ssid" );
    strcpy((char* )wifi_config.sta.ssid, value);
    free(value);
    value = get_str_variable ( "storage", "pass" );
    strcpy((char* )wifi_config.sta.password, value);
    free(value);

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));

    error_status = esp_wifi_start();
	error_status = esp_wifi_connect();
    uint8_t status_loop = 0;
    while (error_status == ESP_ERR_WIFI_NOT_STARTED) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        error_status = esp_wifi_connect();
        status_loop++;
        if (status_loop >= 10) {
            return error_status;
        }
    }
    printf("Start wifi (wifi_sta_mode): %s\n", esp_err_to_name(error_status));
    return error_status;
}

static void event_handler (void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();
		printf("Retry to connect to the AP\n");
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_WIFI_READY) {
        printf("WIFI_EVENT_WIFI_READY\n");
    }
}

void configure_wifi (void) {
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	netif_ap = esp_netif_create_default_wifi_ap();
	assert(netif_ap);
	netif_sta = esp_netif_create_default_wifi_sta();
	assert(netif_sta);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );

	esp_event_handler_instance_t instance_id;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_id));
}

esp_err_t start_wifi (wifi_mode_t set_mode) {
	esp_err_t error_status;
    wifi_mode_t current_mode;

	error_status = esp_wifi_get_mode(&current_mode);

	if (error_status == ESP_OK) {
		switch(current_mode) {
		case WIFI_MODE_STA:
			if (set_mode & WIFI_MODE_STA) {
				//error_status = esp_wifi_start();
			} else {
				//ESP_ERROR_CHECK(esp_wifi_stop());
				//vTaskDelay(10 / portTICK_PERIOD_MS);
				//error_status = esp_wifi_start();
				esp_wifi_disconnect();
				vTaskDelay(100 / portTICK_PERIOD_MS);
				wifi_ap_mode();
			}
			break;
		case WIFI_MODE_AP:
			if (set_mode & WIFI_MODE_AP) {
				//error_status = esp_wifi_start();
			} else {
				//ESP_ERROR_CHECK(esp_wifi_stop());
				//vTaskDelay(10 / portTICK_PERIOD_MS);
				//error_status = esp_wifi_start();
				wifi_ap_mode();
			}
			break;
		case WIFI_MODE_NULL:
            printf("Start wifi: WIFI_MODE_NULL\n");
			if (set_mode & WIFI_MODE_STA) {
                error_status = wifi_sta_mode();
			} else if (set_mode & WIFI_MODE_AP) {
                wifi_ap_mode();
				error_status = esp_wifi_start();
			} else {
				error_status = ESP_FAIL;
			}
			break;
		default:
			break;
		}
	}
	return error_status;
}
