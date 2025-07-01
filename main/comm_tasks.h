// comm_tasks.h

#include "esp_log.h"
#include "esp_wifi.h"
#include "driver/uart.h"
#include "UART_comm.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

void esp_wifi_send(const uint8_t *data, size_t length);

void receive_uart(void *pvParameters);
void transmit_uart(void *pvParameters);
