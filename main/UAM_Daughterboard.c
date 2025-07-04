// UAM_Daughterboard.c
/*
This is the main source file for the daughter board of the UAM project...
*/

#include <stdio.h>

// Necessary headers
#include "wifi_main.h"
#include "wifi_monitor.h"
#include "UART_comm.h"
#include "comm_tasks.h"

#define TAG "main"

// Variable to decide if Wi-Fi or Ethernet are being used.
static bool use_wifi = true;

// Enter the Wi-Fi credentials here
#define WIFI_SSID "Oilersrock"
#define WIFI_PASSWORD "1234567890"

// Set server IP and port
#define SERVER_IP "192.168.1.100" // Replace with your server's IP address
#define SERVER_PORT 8080 // Replace with your server's port number

// Main function for daughter board
void app_main(void)
{

    ESP_LOGI(TAG, "Starting Program...");

    uart_setup(); // Function to set up UART communication, see UART_comm.c and UART_comm.h
    ESP_LOGI(TAG, "UART communication setup complete");

    // Initialize Wi-Fi or Ethernet based on the use_wifi variable
    if (use_wifi) {

        ESP_LOGI(TAG, "Using Wi-Fi for communication");
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

            //ESP_LOGI(TAG, "Disconnecting in 5 seconds...");
            //vTaskDelay(pdMS_TO_TICKS(5000));
        }
        
        // Set up socket connection to the server
        int sock = wifi_socket_setup(SERVER_IP, SERVER_PORT);
        if (sock < 0) {
            ESP_LOGE(TAG, "Failed to set up a socket connection");
            return; // Exit if socket setup fails
        } 
        else {
            ESP_LOGI(TAG, "Socket connection established to %s:%d", SERVER_IP, SERVER_PORT);
        }

        // functions to disconnect from wi-fi and deinitialize for testing purposes if needed
        //ESP_ERROR_CHECK(UAMwifi_disconnect()); 
        //ESP_ERROR_CHECK(UAMwifi_deinit()); 

    } else {
        ESP_LOGI(TAG, "Using Ethernet for communication");
        // Ethernet initialization code will go here
        // For now, we are focusing on Wi-Fi
    }

    // Create tasks for UART and Wi-Fi communication, see comm_tasks.c and comm_tasks.h
    ESP_LOGI(TAG, "Creating communication tasks...");
    xTaskCreate(receive_uart, "receive_uart", 2048, &use_wifi, 5, NULL); // Task to receive data from UART and send it over Wi-Fi
    xTaskCreate(transmit_uart, "transmit_uart", 2048, &use_wifi, 5, NULL); // Task to receive data from Wi-Fi and send it over UART

    ESP_LOGI(TAG, "End of Program...");
}
