// wifi_monitor.c
#include "wifi_monitor.h"
#include "esp_wifi.h"
#include "wifi_main.h"
#include "driver/gpio.h"

#define TAG "wifi_monitor"

// Task function
void wifi_status_task(void *pvParameters)
{
    // Initialize GPIO 33 as output
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);
    while(1) {
        wifi_ap_record_t ap_info;
        esp_err_t ret = esp_wifi_sta_get_ap_info(&ap_info);
        if (ret == ESP_OK) 
        {
            ESP_LOGI(TAG, "Wi-Fi connected. SSID: %s, RSSI: %d", ap_info.ssid, ap_info.rssi);
            // Toggle GPIO 33 to indicate Wi-Fi connection
            gpio_set_level(GPIO_NUM_33, 1);
        } else 
        {
            ESP_LOGW(TAG, "Wi-Fi not connected.");
            // Set GPIO 33 low to indicate no Wi-Fi connection
            gpio_set_level(GPIO_NUM_33, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds
    }
}