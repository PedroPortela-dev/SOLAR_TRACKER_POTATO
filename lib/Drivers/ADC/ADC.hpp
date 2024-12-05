#ifndef ADC_HPP
#define ADC_HPP

#include "freertos/FreeRTOS.h"
#include "driver/adc.h"
#include "math.h"

class Adc
{
protected:
    Adc();
public:
    Adc(adc1_channel_t channel);
    Adc(adc2_channel_t channel);

    void init(float min_output = 0, float max_output = 0, float min_input = 0, float max_input = 0);

    float read();
    float readVolt();

private:
    bool m_has_init = false;

    adc1_channel_t m_channel;
    adc2_channel_t m_channel_2;
    bool m_number;
    int m_value;

    float m_max_input;
    float m_min_input;
    float m_max_output;
    float m_min_output;

    float calculate_value(int value);
};

#endif