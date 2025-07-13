// UART_comm.c

#include "UART_comm.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_err.h"

#define TAG "UART_comm"

// The uart_setup function initializes the UART communication parameters and installs the UART driver.
// There are also two more functions: one to receive data from UART and send it over Wi-Fi, 
// and another to receive data from Wi-Fi and send it over UART. 
void uart_setup(void)
{
    // Configure UART Parameters
    // Using UART1 for communication with main board
    const uart_port_t uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    // Set UART parameters
    esp_err_t ret = uart_param_config(uart_num, &uart_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure UART parameters: %s", esp_err_to_name(ret));
        return;
    }

    // Set UART pins(TX: 43, RX: 44, no flow control)
    ret = (uart_set_pin(UART_NUM_1, 43, 44, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set UART pins: %s", esp_err_to_name(ret));
        return;
    }

    // Install UART driver 
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    ret = (uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install UART driver: %s", esp_err_to_name(ret));
        return;
    }

}

