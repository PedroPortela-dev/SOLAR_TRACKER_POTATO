#ifndef UART_HPP
#define UART_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

class uart
{
    public:

        void init(uart_port_t uart_num, int baud_rate, int pin_tx = UART_PIN_NO_CHANGE, int pin_rx = UART_PIN_NO_CHANGE);

        int read(uint8_t* buf, uint32_t size, uint32_t timeout = 100);
        // int read_no_flush(uint8_t* buf, uint32_t size, uint32_t timeout = 100);
        int write(uint8_t* buf, uint32_t size, uint32_t timeout = 100);

        // int read_intr(uint8_t* buf, uint32_t size, uint32_t timeout = 100);
        // int read_intr_no_flush(uint8_t* buf, uint32_t size, uint32_t timeout = 100);
        // int write_interrupt(uint8_t* buf, uint32_t size, uint32_t timeout = 100);

        uint32_t data_len();
        uint32_t data_read();

    private:
        uart_port_t m_uart_num;
        bool is_initialized = false;
        int data_read_bytes = 0;
};

#endif