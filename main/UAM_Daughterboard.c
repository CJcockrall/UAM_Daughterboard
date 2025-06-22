// UAM_Daughterboard.c
#include <stdio.h>

#include "esp_log.h"
#include "esp_wifi.h"

#include "wifi_main.h"
#include "wifi_monitor.h"

#include "freertos/task.h"

#define TAG "main"

// Variable to decide if Wi-Fi or Ethernet are being used. Commented out for now until implemented
// static bool use_wifi = true;

// Enter the Wi-Fi credentials here
#define WIFI_SSID "Oilersrock"
#define WIFI_PASSWORD "1234567890"

// Main function for daughter board
void app_main(void)
{
    // Set up GPIO 33 and 34 as outputs for LEDs. 
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_34, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "Starting Program...");

    ESP_ERROR_CHECK(UAMwifi_init()); // Initialize Wi-Fi

    // Start Wi-Fi monitor task, see wifi_monitor.c and wifi_monitor.h
    xTaskCreate(wifi_status_task, "wifi_status_task", 4096, NULL, 5, NULL);

    // Connect to wi-fi
    esp_err_t ret = UAMwifi_connect(WIFI_SSID, WIFI_PASSWORD);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi network");
    }

    // Look at the wi-fi access point information
    wifi_ap_record_t ap_info; // Structure to hold access point information
    ret = esp_wifi_sta_get_ap_info(&ap_info); // Get access point information
    if (ret == ESP_ERR_WIFI_CONN) {
        ESP_LOGE(TAG, "Wi-Fi station interface not initialized");
    }
    else if (ret == ESP_ERR_WIFI_NOT_CONNECT) {
        ESP_LOGE(TAG, "Wi-Fi station is not connected");
    } else {
        ESP_LOGI(TAG, "--- Access Point Information ---");
        ESP_LOG_BUFFER_HEX("MAC Address", ap_info.bssid, sizeof(ap_info.bssid));
        ESP_LOG_BUFFER_CHAR("SSID", ap_info.ssid, sizeof(ap_info.ssid));
        ESP_LOGI(TAG, "Primary Channel: %d", ap_info.primary);
        ESP_LOGI(TAG, "RSSI: %d", ap_info.rssi);

        ESP_LOGI(TAG, "Disconnecting in 5 seconds...");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }


    // Temporary functictions to disconnect from wi-fi and deinitialize
    // Next will be to receive UART and transmit the data over wifi and vice versa. 
    // Use two taskss, one for UART - > ESP32 - > Wi-Fi and another for Wi-Fi -> ESP32 -> UART
    ESP_ERROR_CHECK(UAMwifi_disconnect()); // Temporary

    ESP_ERROR_CHECK(UAMwifi_deinit()); // Temporary

    ESP_LOGI(TAG, "End of Program...");
}

