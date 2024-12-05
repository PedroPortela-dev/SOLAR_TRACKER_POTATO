#ifndef GPIO_HPP
#define GPIO_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#ifdef __cplusplus
    extern "C" {
#endif

class Gpio{

protected:
    Gpio();

public:
    Gpio(gpio_num_t pin);
    Gpio(gpio_num_t pin, gpio_pullup_t pullup, gpio_pulldown_t pulldown);

    void init(gpio_mode_t mode = GPIO_MODE_INPUT_OUTPUT);

    void write(bool value);
    bool read();
    void toggle();

    void interrupt(gpio_int_type_t intr_type, void (*func)(Gpio self) = nullptr);

    void (*m_func)(Gpio self);

private:
    /* data */
    gpio_num_t m_pin;
    gpio_pullup_t m_pullup;
    gpio_pulldown_t m_pulldown;

    bool m_has_init = false;
};

#ifdef __cplusplus
}
#endif

#endif