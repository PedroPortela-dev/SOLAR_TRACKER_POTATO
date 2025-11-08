/**
 * @file lib_mqtt.hpp
 * @author Daniel Januario (daniel.rocha@fieb.org.br)
 * @brief Embeddo MQTT layer library
 * @version 0.1
 * @date 2023-11-28
 *
 */

#ifndef LIB_MQTT_HPP
#define LIB_MQTT_HPP

#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "mqtt_client.h"
#include "esp_log.h"

#define MQTT_MAX_TOPICS 16


class MQTT

{
public: 
    MQTT();
    void init(uint16_t m_port, char * m_host);
    void write(char * topic, char * data);

private:
    char * m_topic;
    char * m_data;

};

#endif