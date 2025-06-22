// wifi_main.h
#pragma once

#include "esp_err.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

esp_err_t UAMwifi_init(void);

esp_err_t UAMwifi_connect(char* wifi_ssid, char* wifi_password);

esp_err_t UAMwifi_disconnect(void);

esp_err_t UAMwifi_deinit(void);
