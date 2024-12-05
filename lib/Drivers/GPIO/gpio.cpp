#include "gpio.hpp"

bool init_itr = false;

Gpio::Gpio() {}

Gpio::Gpio(gpio_num_t pin)
{
    m_pin = pin;
    m_pulldown = GPIO_PULLDOWN_DISABLE;
    m_pullup = GPIO_PULLUP_DISABLE;
}

Gpio::Gpio(gpio_num_t pin, gpio_pullup_t pullup, gpio_pulldown_t pulldown)
{
    m_pin = pin;
    m_pullup = pullup;
    m_pulldown = pulldown;
}

void Gpio::init(gpio_mode_t mode)
{
    if(m_has_init) return;

    gpio_config_t config = {
        .pin_bit_mask = 1ULL << m_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = m_pullup,
        .pull_down_en = m_pulldown,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&config);

    m_has_init = true;
}

void Gpio::write(bool value)
{
    if(!m_has_init) return;

    gpio_set_level(m_pin, value);
}

bool Gpio::read()
{
    if(!m_has_init) return false;

    return gpio_get_level(m_pin);
}

void Gpio::toggle()
{
    if(!m_has_init) return;

    gpio_set_level(m_pin, !gpio_get_level(m_pin));
}

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    Gpio *m_gpio = (Gpio *)arg;
    xQueueSendFromISR(gpio_evt_queue, &m_gpio, NULL);
}

static void gpio_task_interrupt(void *arg)
{
    Gpio *m_gpio;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &m_gpio, portMAX_DELAY))
        {
            if (m_gpio->m_func == nullptr)
                continue;
            m_gpio->m_func(*m_gpio);
        }
    }
}

void Gpio::interrupt(gpio_int_type_t intr_type, void (*func)(Gpio self))
{
    if(!m_has_init) return;

    m_func = func;

    gpio_set_intr_type(m_pin, intr_type);

    if (!init_itr)
    {
        gpio_evt_queue = xQueueCreate(10, sizeof(Gpio *));

        xTaskCreate(gpio_task_interrupt, "gpio_task_interrupt", 2048, NULL, 10, NULL);

        gpio_install_isr_service(0);
        
        init_itr = true;
    }

    gpio_isr_handler_add(m_pin, gpio_isr_handler, this);
}