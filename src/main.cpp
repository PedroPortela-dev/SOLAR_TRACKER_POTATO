#include "drivers.hpp"
#include "devices.hpp"

uart gps_uart;
Pit timer;
Nvs storage;
Pit mqtt_timer;

TinyGPS gps;
MQTT mqtt;

extern "C" void app_main()
{
    gps_uart.init(UART_NUM_2, 9600, 17, 16);
    timer.init();
    timer.start();
    storage.init();

    wifi_start("CIMATEC-VISITANTE", "");

    while(!wifi_connected())
    {
        vTaskDelay(1);

        if(timer.read())
        {
            timer.write(1000);
            printf("Connecting to WiFi...\n");
        }
    }

    mqtt.init(1883, "mqtt://test.mosquitto.org");

    gps.init();

    while (1)
    {

        bool newData = false;
        unsigned long chars;
        unsigned short sentences, failed;
        uint8_t c;

    
        while (gps_uart.data_len())
        {
            gps_uart.read(&c, 1);
            if (gps.encode(c))
                newData = true;
        }

        if (newData)
        {
            float flat, flon;
            unsigned long age;
            gps.f_get_position(&flat, &flon, &age);

            printf("LAT=%f LON=%f SAT=%d PREC=%lu\n",
                flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat,
                flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon,
                gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites(),
                gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop()
            );

            storage["latitude"] = flat;
            storage["longitude"] = flon;
        }

        if(timer.read())
        {
            float latitude = storage["latitude"];
            float longitude = storage["longitude"];

            char payload[100];
            snprintf(payload, sizeof(payload), "{\"latitude\": %.6f, \"longitude\": %.6f }", latitude, longitude);
            mqtt.write("esp32/gps", payload);

            timer.write(10000); // 5 seconds
        }


        
        vTaskDelay(1);
    }
    
}