// comm_tasks.c

#include "comm_tasks.h"

// This source file contains the implementation of communication tasks
// 2 tasks, one for UART -> ESP32 -> Wi-Fi/Ethernet and another for Wi-Fi/Ethernet -> ESP32 -> UART 
// Depending of the use_wifi variable, the tasks will either send data over Wi-Fi or Ethernet.

void receive_uart(void *pvParameters)
{
    bool *use_wifi = (bool *)pvParameters;
    // This task will read data from UART based on already configured UART parameters
    // and send it over Wi-Fi or Ethernet.
    while (1) {
        // Read data from UART
        uint8_t data[128]; // Buffer to hold received data
        size_t length = 0;
        esp_err_t ret = uart_get_buffered_data_len(UART_NUM_1, &length);
        if (ret != ESP_OK) {
            ESP_LOGE("UART", "Failed to get buffered data length: %s", esp_err_to_name(ret));
            continue; // Skip this iteration if there was an error
        }
        if (length > 0) {
            length = uart_read_bytes(UART_NUM_1, data, sizeof(data), 100);
            ESP_LOGI("UART", "Received %d bytes from UART", length); // log the number of bytes received
            ESP_LOG_BUFFER_HEX("UART Data", data, length); // log the received data in hex format
            if (*use_wifi) {
                // Send data over Wi-Fi
                esp_wifi_send(data, length);
            } // Implement ethernet later, for now just use Wi-Fi
            
        }

    }
}

void transmit_uart(void *pvParameters)
{
    // This task will read data from Wi-Fi or Ethernet and send it over UART.
    // It will use the UART parameters already configured in uart_setup().

    bool *use_wifi = (bool *)pvParameters;

    while (1) {

    }
}

void esp_wifi_send(const uint8_t *data, size_t length)
{
    // This function sends data over Wi-Fi to an external device on the same network, with TCP
    // Wifi stack is already initialized in UAMwifi_init() and UAMwifi_connect().



}