#ifndef SERVO_HPP
#define SERVO_HPP

#include "PWM/pwm.hpp"

class servo
{
    
    public:
        servo(PWM &Pin);

        void init(float min, float max);

        void set_max_power(float value);
        float get_max_power();

        void write(float value);
        float read();

        void set_power(float value);
        float get_power();

        void front();
        void back();
        void stop();

    private:

        float calculate_PWM(float data);
        float inv_calculate_PWM(float data);

        PWM &m_Pin;

        float m_max_input;
        float m_min_input;
        float m_max_output;
        float m_min_output;

        float m_power = 1;
        float m_max_power = 1;

        bool m_has_init = false;
};


#endif