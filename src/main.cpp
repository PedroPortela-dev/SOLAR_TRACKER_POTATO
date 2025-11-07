#include "drivers.hpp"
#include "devices.hpp"

uart ss;
TinyGPS gps;
Pit timer;
Gpio led(GPIO_NUM_2);

uart terminal;

extern "C" void app_main()
{
    ss.init(UART_NUM_2, 9600, 17, 16); // RX=16, TX=17
    timer.init();
    gps.init();
    led.init(GPIO_MODE_OUTPUT);

    timer.start();

    printf("Simple TinyGPS library v. %d\n", TinyGPS::library_version());
    printf("by Portelinha\n\n");

    while (1)
    {

        bool newData = false;
        unsigned long chars;
        unsigned short sentences, failed;
        uint8_t c;

        // For one second we parse GPS data and report some key values
    
        while (ss.data_len())
        {
            ss.read(&c, 1);
            if (gps.encode(c))
                newData = true;

            printf("%c", c);
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
            gps.stats(&chars, &sentences, &failed);
    
            printf(" CHARS=%lu SENTENCES=%hu CSUM ERR=%hu\n", chars, sentences, failed);

            led.toggle();
        }  

        vTaskDelay(1);
    }
    
}