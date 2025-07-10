// wifi_main.h
#pragma once

#include "esp_err.h"
#include "esp_log.h"

esp_err_t UAMwifi_init(void);

esp_err_t UAMwifi_connect(char* wifi_ssid, char* wifi_password);

esp_err_t UAMwifi_disconnect(void);

esp_err_t UAMwifi_deinit(void);

int network_socket_setup(const char *server_ip, uint16_t server_port);
