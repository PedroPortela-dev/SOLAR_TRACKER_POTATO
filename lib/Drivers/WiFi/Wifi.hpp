/**
 * @file drv_wifi.h
 * @author Pedro Portela Rodrigues (pedro.rodrigues@fbest.org.br)
 * @brief 
 * @version 1.0
 * @date 2023-09-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WIFI_DRV_
#define WIFI_DRV_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "esp_wifi.h"

#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include <string.h>

void wifi_start(char * ssid, char * password);

bool wifi_connected();

#endif /* WIFI_DRV */