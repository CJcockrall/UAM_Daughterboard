// UAM_Daughterboard.c
/*
This is the main source file for the daughter board of the UAM project...
*/

#include "wifi_main.h"
#include "wifi_monitor.h"
#include "UART_comm.h"
#include "comm_tasks.h"
#include "ethernet_main.h"

#define TAG "main"

///////// User configurations: ///////
// Decide if Wi-Fi or Ethernet are being used.
static bool use_wifi = true;

// Enter the Wi-Fi credentials here
#define WIFI_SSID "Oilersrock"
#define WIFI_PASSWORD "1234567890"

#define SERVER_IP "10.0.0.194" // Replace with your server's IP address
#define SERVER_PORT 8080 // Replace with your server's port number
///////////////////////////////////////

int sock = -1; // Global variable to hold socket file descriptor, initialized with invalid value
void app_main(void)
{

    ESP_LOGI(TAG, "Starting Program...");

    uart_setup(); // Function to set up UART communication, see UART_comm.c and UART_comm.h
    ESP_LOGI(TAG, "UART communication setup complete");

    // Initialize Wi-Fi or Ethernet based on the use_wifi variable
    if (use_wifi) {

        ESP_LOGI(TAG, "Using Wi-Fi for communication");
        ESP_ERROR_CHECK(UAMwifi_init()); // Initialize Wi-Fi

        // Start Wi-Fi monitoring task for LED, see wifi_monitor.c and wifi_monitor.h
        xTaskCreate(wifi_status_task, "wifi_status_task", 4096, NULL, 5, NULL);

        // Connect to wi-fi according to given SSID and password
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
        } else { // Display AP info in ESP log
            ESP_LOGI(TAG, "--- Access Point Information ---");
            ESP_LOG_BUFFER_HEX("MAC Address", ap_info.bssid, sizeof(ap_info.bssid));
            ESP_LOG_BUFFER_CHAR("SSID", ap_info.ssid, sizeof(ap_info.ssid));
            ESP_LOGI(TAG, "Primary Channel: %d", ap_info.primary);
            ESP_LOGI(TAG, "RSSI: %d", ap_info.rssi);

        }
        
        // Set up socket connection to the server
        sock = network_socket_setup(SERVER_IP, SERVER_PORT);
        if (sock < 0) {
            ESP_LOGE(TAG, "Failed to set up a socket connection");
            return; // Exit if socket setup fails
        } 
        else {
            ESP_LOGI(TAG, "Socket connection established to %s:%d", SERVER_IP, SERVER_PORT);
        }

    } else {
        ESP_LOGI(TAG, "Using Ethernet for communication");
        ethernet_init(); // Initialize Ethernet, see ethernet_main.c and ethernet_main.h
        sock = network_socket_setup(SERVER_IP, SERVER_PORT); 
        if (sock < 0) {
            ESP_LOGE(TAG, "Failed to set up a socket connection");
            return; // Exit if socket setup fails
        } 
        else {
            ESP_LOGI(TAG, "Socket connection established to %s:%d", SERVER_IP, SERVER_PORT);
        }

    }

    // Create tasks for UART and Wi-Fi communication, see comm_tasks.c and comm_tasks.h
    ESP_LOGI(TAG, "Creating communication tasks...");

    TaskHandle_t rx_task_handle = NULL;
    TaskHandle_t tx_task_handle = NULL;

    // Task to receive data from UART and send it over Wi-Fi or Ethernet
    if (xTaskCreate(receive_uart, "receive_uart", 4096, NULL, 5, &rx_task_handle) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create receive_uart task");
        return;
    }
    // Task to transmit data from Wi-Fi or Ethernet to UART
    if (xTaskCreate(transmit_uart, "transmit_uart", 4096, NULL, 5, &tx_task_handle) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create transmit_uart task");
        return;
    }

    ESP_LOGI(TAG, "End of Program...");
}
