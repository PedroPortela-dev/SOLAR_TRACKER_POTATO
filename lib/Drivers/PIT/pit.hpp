#ifndef PIT_HPP
#define PIT_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gptimer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        uint64_t start_timer;   
        uint64_t countLimit;
    } driverPITParammeters_t;

    class Pit
    {

        public:
            void init();

            void start();
            void stop();
            void reset();
            uint64_t get();

            void write(uint64_t countsToPerform);
            bool read();

            static void delay(uint64_t value);
            

        private:
            gptimer_handle_t gptimer = NULL;
            driverPITParammeters_t Timer;

            bool m_has_init = false;
    };

#ifdef __cplusplus
}
#endif

#endif