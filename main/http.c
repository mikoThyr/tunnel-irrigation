#include "http.h"

static const char *TAG = "HTTP";

typedef struct {
	char ssid[32];
    char pass[32];
} user_data_t;

esp_err_t get_handler (httpd_req_t *req) {
    httpd_resp_send(req, "<html>"
	    "<head>"
			"<title>ESP32</title>"
			"<style type=\"text/css\">"
				"body { background-color: #6591A8; }"
			"</style>"
		"</head>"
		"<body>"
			"<div class=\"outer\"><br><br>"
			"<form action=\"/url\" method=\"post\">"
				"<h3>Complete the fields below with the network ssid and password</h3><br><br>"
				"<label for=\"ssid\">SSID:</label><br>"
				"<input type=\"text\" id=\"ssid\" name=\"ssid\" required><br><br>"
				"<label for=\"pass\">Password:</label><br>"
				"<input type=\"password\" id=\"pass\" name=\"pass\" required><br>"
				"<input type=\"submit\" value=\"Save\">"
			"</form>"
			"</div>"
		"</body>"
		"</html>", -1);

	return ESP_OK;
}

esp_err_t post_handler (httpd_req_t *req) {
    char* saveptr;
    char* token;
    user_data_t user_data;
    char data[64];
    memset(&user_data, 0, sizeof(user_data));
    memset(&data, 0, sizeof(data));

    httpd_req_recv(req, data, sizeof(data));
    data[strlen(data)] = '\0';
    token = strtok_r(data, "&", &saveptr);
    while (token != NULL) {
        if (strstr(token, "ssid=") == token) {
            char* val = token + strlen("ssid=");
            strncpy(user_data.ssid, val, sizeof(user_data.ssid) - 1);
        } else if (strstr(token, "pass=") == token) {
            char* val = token + strlen("pass=");
            strncpy(user_data.pass, val, sizeof(user_data.pass) - 1);
        }

        token = strtok_r(NULL, "&", &saveptr);
    }
    httpd_resp_send(req, "<html>"
	    "<head>"
		    "<title>ESP32</title>"
			    "<style type=\"text/css\">"
				    "body { background-color: #6591A8; }"
			    "</style>"
		    "</head>"
	    	"<body>"
			    "<h3>OK.</h3>"
		    "</body>"
		    "</html>", -1);

    write_nvm_data("storage", "ssid", user_data.ssid);
    write_nvm_data("storage", "pass", user_data.pass);
	return ESP_OK;
}

httpd_uri_t uri_get = {
	.uri = "/uri",
	.method = HTTP_GET,
	.handler = get_handler,
	.user_ctx = NULL
};

httpd_uri_t uri_post = {
	.uri       = "/url",
	.method    = HTTP_POST,
	.handler   = post_handler,
	.user_ctx  = NULL
};

httpd_handle_t start_webserver (void) {
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	httpd_handle_t server = NULL;
	if (httpd_start(&server, &config) == ESP_OK) {
		httpd_register_uri_handler(server, &uri_get);
		httpd_register_uri_handler(server, &uri_post);
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

