#include "drivers.hpp"
#include "devices.hpp"

Adc sensor[4] = {
    Adc(ADC1_CHANNEL_3),
    Adc(ADC1_CHANNEL_6),
    Adc(ADC1_CHANNEL_7),
    Adc(ADC1_CHANNEL_4)};

PWM pins[2] = {
    PWM(GPIO_NUM_2, LEDC_TIMER_12_BIT, 50),
    PWM(GPIO_NUM_15, LEDC_TIMER_12_BIT, 50)};

servo servos[2] = {
    servo(pins[0]),
    servo(pins[1])};

extern "C" void app_main()
{
    sensor[0].init(0, 1, 0, 2300);
    sensor[1].init(0, 1, 0, 600);
    sensor[2].init(0, 1, 0, 3100);
    sensor[3].init(0, 1, 0, 1100);


    servos[0].init(1, 2);
    servos[1].init(0.5, 2.5);

    float sensores[4];
    float kp[2] = {2.0, 2.0};
    float erro[2];
    float posicao[2] = {0};

    while (1)
    {
        for (size_t i = 0; i < 4; i++)
        {
            sensores[i] = sensor[i].read();
        }

        erro[0] = sensores[3] - sensores[0];
        erro[1] = sensores[2] - sensores[1];

        if (abs(erro[0]) < 0.05)
            erro[0] = 0;
        if (abs(erro[1]) < 0.07)
            erro[1] = 0;

        printf("S1: %.2f S2 %.2f S3 %.2f S4 %.2f", sensores[0] , sensores[1], sensores[2], sensores[3]);
        printf("\terro1: %.2f \terro2: %.2f", erro[0], erro[1]);

        posicao[0] += erro[0] * kp[0] * 0.01;

        if(posicao[0] >= 0)
        {
            posicao[1] -= erro[1] * kp[1] * 0.01;
        }else
        {
            posicao[1] += erro[1] * kp[1] * 0.01;
        }

        if (posicao[0] > 1)
            posicao[0] = 1;
        else if (posicao[0] < -1)
            posicao[0] = -1;

        if (posicao[1] > 1)
            posicao[1] = 1;
        else if (posicao[1] < -1)
            posicao[1] = -1;

        printf("\tservo1: %.2f \tservo2: %.2f\r\n", posicao[0], posicao[1]);

        servos[0].write(posicao[0]);
        servos[1].write(posicao[1]);

        vTaskDelay(1);
    }
}