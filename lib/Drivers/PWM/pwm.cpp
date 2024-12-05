#include "pwm.hpp"

static ledc_timer_bit_t timer_resolution[LEDC_TIMER_MAX];
static uint8_t timer_lock[LEDC_TIMER_MAX];
static uint8_t channel_lock[LEDC_SPEED_MODE_MAX][LEDC_CHANNEL_MAX];

PWM::PWM(){}

PWM::PWM(gpio_num_t pin, ledc_timer_bit_t resolution, uint32_t freq_hz)
{
    m_pin = pin;
    m_resolution = resolution;
    m_freq_hz = freq_hz;
}

void PWM::init(float min_input, float max_input, float min_output, float max_output)
{
    if(m_has_init) return;

    //definindo frequencia
    if(m_freq_hz == 0) m_freq_hz = 65536000/pow(2,(uint8_t)m_resolution);
    
    //definindo timer
    for (uint8_t i = 0; i < LEDC_TIMER_MAX; i++)
    {
        if (timer_resolution[i] == 0)
        {
            timer_resolution[i] = m_resolution;
            m_timer = (ledc_timer_t)i;
            break;
        }
        else if (timer_resolution[i] == m_resolution)
        {
            m_timer = (ledc_timer_t)i;
            break;
        }
    }

    //definindo modo e canal
    for(uint8_t mode = 0; mode < LEDC_SPEED_MODE_MAX; mode++)
    {
        for (uint8_t i = 0; i < LEDC_CHANNEL_MAX; i++)
        {
            if (channel_lock[mode][i] != 1)
            {
                channel_lock[mode][i] = 1;
                m_mode = (ledc_mode_t)mode;
                m_channel = (ledc_channel_t)i;
                break;
            }
        }
    }

    //construindo o timer
    if (timer_lock[m_timer] != 1)
    {
        timer_lock[m_timer] = 1;

        #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        ledc_timer_config_t ledc_timer = {
            .speed_mode = m_mode,
            .duty_resolution = m_resolution,
            .timer_num = m_timer,
            .freq_hz = m_freq_hz,
            .clk_cfg = LEDC_USE_APB_CLK
        };
        #pragma GCC diagnostic pop
        ledc_timer_config(&ledc_timer);
    }

    //construindo o channel
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    ledc_channel_config_t ledc_channel = {
        .gpio_num = m_pin,
        .speed_mode = m_mode,
        .channel = m_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = m_timer,
        .duty = 0,
        .hpoint = 0
    };
    #pragma GCC diagnostic pop
    ledc_channel_config(&ledc_channel);

    if(max_input == min_input)
    {
        m_max_input = pow(2,(uint8_t)m_resolution);
        m_min_input = 0;
        m_max_output = pow(2,(uint8_t)m_resolution);
        m_min_output = 0;
    }else
    if(max_output == min_output)
    {
        m_max_input = max_input;
        m_min_input = min_input;
        m_max_output = pow(2,(uint8_t)m_resolution);
        m_min_output = 0;

    }else
    {
        m_max_input = max_input;
        m_min_input = min_input;
        m_max_output = max_output;
        m_min_output = min_output;
    }

    m_has_init = true;
}

void PWM::write(float duty)
{
    if(!m_has_init) return;

    ledc_set_duty(m_mode, m_channel, calculate_duty(duty));
    ledc_update_duty(m_mode, m_channel);
}

float PWM::getDuty()
{
    if(!m_has_init) return 0;

    return inv_calculate_duty(ledc_get_duty(m_mode, m_channel));
}

uint32_t PWM::calculate_duty(float duty)
{
    if(m_max_input == 0 && m_min_input == 0) return duty;
    
    if(duty > m_max_input) duty = m_max_input;
    if(duty < m_min_input) duty = m_min_input;

    float coeficiente_angular = (m_max_output - m_min_output)/(m_max_input - m_min_input);
    return (duty - m_min_input)*coeficiente_angular + m_min_output;
}

float PWM::inv_calculate_duty(uint32_t duty)
{
    if(m_max_input == 0 && m_min_input == 0) return duty;

    float coeficiente_angular = (m_max_output - m_min_output)/(m_max_input - m_min_input);
    return (duty - m_min_output)/coeficiente_angular + m_min_input;
}