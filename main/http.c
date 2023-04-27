#include "http.h"

static const char *TAG = "HTTP";

esp_err_t set_handler (httpd_req_t *req) {
    const char *response = "<html> \
<head> \
    <title>ESP32</title> \
    <style type=\"text/css\"> \
        body { \
            background-color: #d1e7dd; \
            margin: 0; \
            padding: 0; \
        } \
        #header { \
            background-color: #2c786c; \
            height: 50px; \
            padding: 10px; \
        } \
        #menu { \
            background-color: #e0f3eb; \
            height: 30px; \
            padding: 5px; \
        } \
        #menu a { \
            display: inline-block; \
            margin-right: 10px; \
            color: #2c786c; \
            text-decoration: none; \
        } \
        #menu a:hover { \
            text-decoration: underline; \
        } \
        #content { \
            margin: 10px; \
            padding: 10px; \
            background-color: #ffffff; \
        } \
    </style> \
</head> \
<body> \
    <div id=\"header\"> \
        <h1>ESP32 Irrigation</h1> \
    </div> \
    <div id=\"menu\" style=\"display: flex; justify-content: space-between; align-items: center;\"> \
    <div> \
        <a href=\"/\">Device</a> \
        <a href=\"/wifi\">Wifi</a> \
    </div> \
        <form action=\"/logout\" method=\"post\" style=\"padding: 10px 0; margin: 0;\"> \
            <input type=\"submit\" value=\"Logout\" style=\"padding: 10px 20px; align-self: center;\"> \
        </form> \
    </div> \
    <div id=\"content\"> \
    <h1>Settings</h1> \
    <form action=\"/end\" method=\"post\"> \
        <fieldset style=\"background-color: #F5F5F5\"> \
            <legend>Watering during:</legend> \
            <label><input type=\"radio\" name=\"time_day\" value=\"day\">Day</label> \
            <label><input type=\"radio\" name=\"time_day\" value=\"day_and_night\">Day and night</label> \
        </fieldset> \
        <br> \
        <fieldset style=\"background-color: #F5F5F5\"> \
            <legend>Run mode:</legend> \
            <label><input type=\"radio\" name=\"mode\" value=\"normal\">Normal</label> \
            <label><input type=\"radio\" name=\"mode\" value=\"eco\">Eco</label> \
        </fieldset> \
        <br> \
        <fieldset style=\"background-color: #F5F5F5\"> \
            <legend>Humidity:</legend> \
            <label>Upper level of the soil humidity (ex 70):</label><br> \
            <input type=\"number\" name=\"humlvlu\" style=\"display: inline-block;\"><br> \
            <label>Lower level when watering will be run (ex 60):</label><br> \
            <input type=\"number\" name=\"humlvld\" style=\"display: inline-block;\"> \
        </fieldset> \
        <br> \
        <fieldset style=\"background-color: #F5F5F5\"> \
            <legend>Temperature (ex 20):</legend> \
            <label>Water temperature to watering plants:</label><br> \
            <input type=\"number\" name=\"temp\" style=\"display: inline-block;\"><br> \
        </fieldset> \
        <br> \
        <input type=\"submit\" value=\"Save\"> \
    </form> \
</div> \
</body> \
</html>";
    httpd_resp_send(req, response, strlen(response));
	return ESP_OK;
}

esp_err_t wifi_handler (httpd_req_t *req) {
    const char *response = "<html> \
<head> \
    <title>ESP32</title> \
    <style type=\"text/css\"> \
        body { \
            background-color: #d1e7dd; \
            margin: 0; \
            padding: 0; \
        } \
        #header { \
            background-color: #2c786c; \
            height: 50px; \
            padding: 10px; \
        } \
        #menu { \
            background-color: #e0f3eb; \
            height: 30px; \
            padding: 5px; \
        } \
        #menu a { \
            display: inline-block; \
            margin-right: 10px; \
            color: #2c786c; \
            text-decoration: none; \
        } \
        #menu a:hover { \
            text-decoration: underline; \
        } \
        #content { \
            margin: 10px; \
            padding: 10px; \
            background-color: #ffffff; \
        } \
    </style> \
</head> \
<body> \
    <div id=\"header\"> \
        <h1>ESP32 Irrigation</h1> \
    </div> \
    <div id=\"menu\" style=\"display: flex; justify-content: space-between; align-items: center;\"> \
    <div> \
        <a href=\"/\">Device</a> \
        <a href=\"/wifi\">Wifi</a> \
    </div> \
        <form action=\"/logout\" method=\"post\" style=\"padding: 10px 0; margin: 0;\"> \
            <input type=\"submit\" value=\"Logout\" style=\"padding: 10px 20px; align-self: center;\"> \
        </form> \
    </div> \
    <div id=\"content\"> \
    <h1>Wifi</h1> \
    <form action=\"/end\" method=\"post\"> \
        <fieldset style=\"background-color: #F5F5F5\"> \
            <legend>WiFi:</legend> \
            <label><input type=\"radio\" name=\"wifi\" value=\"on\">On</label> \
            <label><input type=\"radio\" name=\"wifi\" value=\"off\">Off</label> \
            <br><br> \
            <legend>SSID:</legend> \
            <input type=\"text\" name=\"ssid\" style=\"display: inline-block;\"><br> \
            <legend>Password:</legend> \
            <input type=\"password\" name=\"pass\" style=\"display: inline-block;\"> \
        </fieldset> \
        <br> \
        <input type=\"submit\" value=\"Save\"> \
    </form> \
</div> \
</body> \
</html>";
    httpd_resp_send(req, response, strlen(response));
	return ESP_OK;
}

esp_err_t post_handler (httpd_req_t *req) {
    uint8_t value;
    char* saveptr;
    char* token;
    char data[128];
    data[0] = '\0';
    httpd_req_recv(req, data, sizeof(data));
    printf("%s\n", data);
    token = strtok_r(data, "&", &saveptr);
    while (token != NULL) {
        if (strstr(token, "wifi=") == token) {
            char* val = token + strlen("wifi=");
            if (strncmp(val, "on", strlen("on")) == 0) {
                check_i8_variable("storWifi", "wifi", (uint8_t)WIFI_ON, SET);
                printf("val: %s\n", val);
            } else if (strncmp(val, "off", strlen("off")) == 0) {
                check_i8_variable("storWifi", "wifi", (uint8_t)WIFI_OFF, SET);
                printf("val: %s\n", val);
            }
        } else if (strstr(token, "ssid=") == token) {
            char* val = token + strlen("ssid=");
            if (strlen(val) != 0) {
                write_nvm_data("storWifi", "ssid", val);
            }
        } else if (strstr(token, "pass=") == token) {
            char* val = token + strlen("pass=");
            if ((strlen(val) != 0) && (isascii((int)val[0]) != 0)) {
                write_nvm_data("storWifi", "pass", val);
            }
        } else if (strstr(token, "time_day=") == token) {
            char* val = token + strlen("time_day=");
            if (strncmp(val, "day_and_night", strlen("day_and_night")) == 0) {
                check_i8_variable("storDev", "time_day", (uint8_t)DAYNIGHT, SET);
                printf("val: %s\n", val);
            } else if (strncmp(val, "day", strlen("day")) == 0) {
                check_i8_variable("storDev", "time_day", (uint8_t)DAY, SET);
                printf("val: %s\n", val);
            }
        } else if (strstr(token, "mode=") == token) {
            char* val = token + strlen("mode=");
            if (strncmp(val, "normal", strlen("normal")) == 0) {
                check_i8_variable("storDev", "mode", (uint8_t)NORMAL, SET);
                printf("val: %s\n", val);
            } else if (strncmp(val, "eco", strlen("eco")) == 0) {
                check_i8_variable("storDev", "mode", (uint8_t)ECO, SET);
                printf("val: %s\n", val);
            }
        } else if (strstr(token, "humlvlu=") == token) {
            char* val = token + strlen("humlvlu=");
            printf("strlen humlvlu: %d\n", strlen(val));
            if (strlen(val) != 0) {
                value = atoi(val);
                check_i8_variable("storDev", "hum_high", (int8_t)value, SET);
            }
        } else if (strstr(token, "humlvld=") == token) {
            char* val = token + strlen("humlvld=");
            if (strlen(val) != 0) {
                value = atoi(val);
                check_i8_variable("storDev", "hum_low", (int8_t)value, SET);
            }
        } else if (strstr(token, "temp=") == token) {
            char* val = token + strlen("temp=");
            if ((strlen(val) != 0) && (isdigit((int)val[0]) != 0)) {
                value = atoi(val);
                check_i8_variable("storDev", "temp", (int8_t)value, SET);
            }
        }
        token = strtok_r(NULL, "&", &saveptr);
    }
    const char *response = "<html> \
<head> \
    <title>ESP32</title> \
    <style type=\"text/css\"> \
        body { \
            background-color: #d1e7dd; \
            margin: 0; \
            padding: 0; \
        } \
        #header { \
            background-color: #2c786c; \
            height: 50px; \
            padding: 10px; \
        } \
        #menu { \
            background-color: #e0f3eb; \
            height: 30px; \
            padding: 5px; \
        } \
        #menu a { \
            display: inline-block; \
            margin-right: 10px; \
            color: #2c786c; \
            text-decoration: none; \
        } \
        #menu a:hover { \
            text-decoration: underline; \
        } \
        #content { \
            margin: 10px; \
            padding: 10px; \
            background-color: #ffffff; \
        } \
    </style> \
</head> \
<body> \
    <div id=\"header\"> \
        <h1>ESP32 Irrigation</h1> \
    </div> \
    <div id=\"menu\" style=\"display: flex; justify-content: space-between; align-items: center;\"> \
    <div> \
        <a href=\"/\">Device</a> \
        <a href=\"/wifi\">Wifi</a> \
    </div> \
        <form action=\"/logout\" method=\"post\" style=\"padding: 10px 0; margin: 0;\"> \
            <input type=\"submit\" value=\"Logout\" style=\"padding: 10px 20px; align-self: center;\"> \
        </form> \
    </div> \
    <div id=\"content\"> \
    <h1>Your changes will be set after wifi disconnect.</h1> \
    </div> \
</body> \
</html>";
    httpd_resp_send(req, response, strlen(response));
	return ESP_OK;
}

esp_err_t logout_handler (httpd_req_t *req) {

    esp_wifi_stop();
    return ESP_OK;
}

httpd_uri_t set_get = {
	.uri = "/",
	.method = HTTP_GET,
	.handler = set_handler,
	.user_ctx = NULL
};

httpd_uri_t wifi_get = {
	.uri = "/wifi",
	.method = HTTP_GET,
	.handler = wifi_handler,
	.user_ctx = NULL
};

httpd_uri_t end_post = {
	.uri       = "/end",
	.method    = HTTP_POST,
	.handler   = post_handler,
	.user_ctx  = NULL
};

httpd_uri_t logout_post = {
	.uri       = "/logout",
	.method    = HTTP_POST,
	.handler   = logout_handler,
	.user_ctx  = NULL
};

httpd_handle_t start_webserver (void) {
	esp_err_t error_status;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	httpd_handle_t server = NULL;
    error_status = httpd_start(&server, &config);
	if (error_status == ESP_OK) {
        httpd_register_uri_handler(server, &set_get);
		httpd_register_uri_handler(server, &end_post);
        httpd_register_uri_handler(server, &wifi_get);
        httpd_register_uri_handler(server, &logout_post);
	} else {
        printf("Start httpd: %s\n", esp_err_to_name(error_status));
    }
	return server;
}

void stop_webserver (httpd_handle_t server) {
	if (server) {
		httpd_stop(server);
	}
}

void start_http_client ( uint8_t temperature, uint8_t humidity ) {
    char post_data[50];
    snprintf(post_data, sizeof(post_data), "temperature=%d&humidity=%d", temperature, humidity);

    esp_http_client_config_t config = {
        .host = "192.168.1.10",
        .path = "/",
        .port = 5000,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 1000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        if (status_code == 200) {
            ESP_LOGI(TAG, "POST request successful");
        } else {
            ESP_LOGI(TAG, "POST request failed with status code: %d", status_code);
        }
    } else {
        ESP_LOGE(TAG, "Error performing POST request: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    esp_http_client_close(client);
}

