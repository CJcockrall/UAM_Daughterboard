// comm_tasks.h

#include "esp_log.h"
#include "esp_wifi.h"


void esp_wifi_send(const uint8_t *data, size_t length);

void receive_uart(void *pvParameters);
void transmit_uart(void *pvParameters);
