#ifndef PWM_HPP
#define PWM_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "math.h"

class PWM
{
private:
    ledc_timer_bit_t m_resolution;
    ledc_mode_t m_mode;
    ledc_timer_t m_timer;
    uint32_t m_freq_hz;
    gpio_num_t m_pin;
    ledc_channel_t m_channel;

    float m_max_input;
    float m_min_input;
    float m_max_output;
    float m_min_output;

    uint32_t calculate_duty(float duty);
    float inv_calculate_duty(uint32_t duty);

    bool m_has_init = false;

    PWM();
public:
    PWM(gpio_num_t pin, ledc_timer_bit_t resolution, uint32_t freq_hz = 0);

    void init(float min_input = 0, float max_input = 0, float min_output = 0, float max_output = 0);
    void write(float duty);
    float getDuty();
};

#endif