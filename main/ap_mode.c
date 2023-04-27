/**
 *  @file ap_mode.c
 *  @brief  ISR which run task to run the httpd server.
 */
#include "ap_mode.h"

gpio_config_t io_wifi_button = {
    .intr_type = GPIO_INTR_NEGEDGE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL << PIN_AP_MODE),
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
};

/**
 * @brief
 */
void IRAM_ATTR button_interrupt_handler (void *arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    gpio_intr_disable(PIN_AP_MODE);
    xTaskNotifyFromISR(task_ButtonRunAP , 0, eNoAction, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
    gpio_intr_enable(PIN_AP_MODE);
}

/**
 * @brief
 */
void button_run_ap (void *arg) {
    esp_err_t error_status;
    httpd_handle_t server_handle;
    wifi_sta_list_t user_status;

    // Initialize gpio and isr
    gpio_config(&io_wifi_button);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIN_AP_MODE, button_interrupt_handler, NULL);

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        gpio_intr_disable(PIN_AP_MODE);

        error_status = start_wifi(WIFI_MODE_AP);
        if (error_status == ESP_OK) {
            server_handle = start_webserver();
            // Wait 20 sec to connection with device and either run or stop
            // httpd.
            vTaskDelay(20 * 1000 / portTICK_PERIOD_MS);         /* time to connect to the wifi */
            esp_wifi_ap_get_sta_list(&user_status);             /* amount connected users */
            while (user_status.num != 0) {                      /* don't stop if there is more than 0 */
                vTaskDelay(5 * 1000 / portTICK_PERIOD_MS);
                esp_wifi_ap_get_sta_list(&user_status);
            }
            stop_webserver(server_handle);
            esp_wifi_stop();

            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_intr_enable(PIN_AP_MODE);
            esp_restart();
        }
    }
}
