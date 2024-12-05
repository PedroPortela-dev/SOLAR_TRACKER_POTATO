#include "ADC.hpp"

#define D_MAX 4095
#define MAX_VOLTS 3
#define MIN_VOLTS 0.1

Adc::Adc() {}

Adc::Adc(adc1_channel_t channel) : m_channel(channel)
{
    m_number = 0;
}

Adc::Adc(adc2_channel_t channel) : m_channel_2(channel)
{
    m_number = 1;
}

void Adc::init(float min_output, float max_output, float min_input, float max_input)
{
    if(m_has_init) return;

    if(!m_number)
    {
        ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));
        ESP_ERROR_CHECK(adc1_config_channel_atten(m_channel, ADC_ATTEN_DB_11));
    }else
    {
        ESP_ERROR_CHECK(adc2_config_channel_atten(m_channel_2, ADC_ATTEN_DB_11));
    }

    if(min_output == max_output)
    {
        m_min_output = 0;
        m_max_output = pow(2, (uint8_t)ADC_WIDTH_BIT_12);
        m_min_input = 0;
        m_max_input = pow(2, (uint8_t)ADC_WIDTH_BIT_12);
    }else
    if(min_input == max_input)
    {
        m_min_output = min_output;
        m_max_output = max_output;
        m_min_input = 0;
        m_max_input = pow(2, (uint8_t)ADC_WIDTH_BIT_12);
    }else
    {
        m_min_output = min_output;
        m_max_output = max_output;
        m_min_input = min_input;
        m_max_input = max_input;
    }

    m_has_init = true;
}

float Adc::read()
{
    if(!m_has_init) return 0;

    if (!m_number)
        return  calculate_value(adc1_get_raw(m_channel)); 

    adc2_get_raw(m_channel_2, ADC_WIDTH_BIT_12, &m_value);
    return calculate_value(m_value);
}

float Adc::readVolt()
{
    if(!m_has_init) return 0;

    if (!m_number)
        return adc1_get_raw(m_channel)*(float)(MAX_VOLTS-MIN_VOLTS) / D_MAX + MIN_VOLTS;

    adc2_get_raw(m_channel_2, ADC_WIDTH_BIT_12, &m_value);
    return m_value*(float)(MAX_VOLTS-MIN_VOLTS) / D_MAX + MIN_VOLTS;
}

float Adc::calculate_value(int value)
{
    if(m_max_output == 0 && m_min_output == 0) return value;
    
    if(value > m_max_input) value = m_max_input;
    if(value < m_min_input) value = m_min_input;

    float coeficiente_angular = (m_max_output - m_min_output)/(m_max_input - m_min_input);
    return (value - m_min_input)*coeficiente_angular + m_min_output;
}