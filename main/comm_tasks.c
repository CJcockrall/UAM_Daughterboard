// comm_tasks.c

#include "comm_tasks.h"

#define TAG "comm_tasks"

extern int sock; // Global variable defined in UAM_Daughterboard.c that holds socket file descriptor

// This source file contains the implementation of communication tasks
// 2 tasks, one for UART -> ESP32 -> Wi-Fi/Ethernet and another for Wi-Fi/Ethernet -> ESP32 -> UART 
// Both functions work for Wi-Fi or Ethernet, because of how the socket API works

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
            ESP_LOGE(TAG, "Failed to get buffered data length: %s", esp_err_to_name(ret));
            continue; // Skip this iteration if there was an error
        }
        if (length > 0) {
            length = uart_read_bytes(UART_NUM_1, data, sizeof(data), 100);
            ESP_LOGI(TAG, "Received %d bytes from UART", length); // log the number of bytes received
            ESP_LOG_BUFFER_HEX(TAG, data, length); // log the received data in hex format
            if (*use_wifi) {
                // Send data over Wi-Fi (TCP socket)
                int sent = send(sock, data, length, 0);
                if (sent < 0) {
                    ESP_LOGE(TAG, "Failed to send data over Wi-Fi: %s", esp_err_to_name(errno));
                } else {
                    ESP_LOGI(TAG, "Sent %d bytes over Wi-Fi", sent);
                }
            }
        }
    }
}

void transmit_uart(void *pvParameters)
{
    // This task will read data from Wi-Fi or Ethernet and send it over UART.
    // It will use the UART parameters already configured in uart_setup().

    bool *use_wifi = (bool *)pvParameters;

    while (1) {
        // Read data from Wi-Fi or Ethernet
        uint8_t data[128]; // Buffer to hold received data
        size_t length = 0; 
        if (*use_wifi) {
            // Read data from Wi-Fi (TCP socket)
            length = recv(sock,data, sizeof(data), 0);
            if (length > 0) {
                ESP_LOGI(TAG, "Received %d bytes from socket", length);
                ESP_LOG_BUFFER_HEX(TAG, data, length); // log received data in hex format
                // Send data over UART to main board
                int written = uart_write_bytes(UART_NUM_1, (const char *)data, length);
                if (written < 0) {
                    ESP_LOGE(TAG, "Failed to write data to UART: %s", esp_err_to_name(written));
                }
            } else if (length == 0) {
                ESP_LOGW(TAG, "Socket closed");
                // Handle socket closure in future maybe, for now just log it and break
                break;
            } else {
                ESP_LOGE(TAG, "Socket receive error: %s", esp_err_to_name(errno));
                vTaskDelay(pdMS_TO_TICKS(1000)); // Wait 1s before retrying 
            }

        }

    }
    vTaskDelete(NULL); // Delete task if loop exits ( socket closed)
}
