#include "uart.hpp"

void uart::init(uart_port_t uart_num, int baud_rate, int pin_tx, int pin_rx)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // Configure UART parameters
    esp_err_t ret = uart_param_config(uart_num, &uart_config);

    // Set UART pins (using UART0 default pins ie no changes.)
    ret = uart_set_pin(uart_num, pin_tx, pin_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    if(uart_is_driver_installed(uart_num)) {
        return;
    }
    
    // Install UART driver, and get the queue.
    ret = uart_driver_install(uart_num, 4096, 0, 0, NULL, 0);

    m_uart_num = uart_num;
    is_initialized = true;
}

int uart::read(uint8_t* buf, uint32_t size, uint32_t timeout)
{
    if(!is_initialized) {
        return 0;
    }
    
    // uart_flush(uart_num);

    int len;
    len = uart_read_bytes(m_uart_num, buf, size, timeout);
    data_read_bytes = len;

    return len;
}

// int uart::read_no_flush(uint8_t* buf, uint32_t size, uint32_t timeout = 100)
// {
//     if(!is_initialized) {
//         return 0;
//     }
    
//     int len;
//     len = uart_read_bytes(uart_num, buf, size, timeout);
//     data_read_bytes = len;

//     return len;
// }

int uart::write(uint8_t* buf, uint32_t size, uint32_t timeout)
{
    if(!is_initialized) {
        return 0;
    }
    
    int len = uart_write_bytes(m_uart_num, buf, size);

    uart_wait_tx_done(m_uart_num, timeout);

    return len;

}

// int uart_read_interrupt(uart_port_t uart_num, uint8_t* buf, uint32_t size, uint32_t timeout = 100);
// int uart_read_intr_no_flush(uart_port_t uart_num, uint8_t* buf, uint32_t size, uint32_t timeout = 100);
// int uart_write_interrupt(uart_port_t uart_num, uint8_t* buf, uint32_t size, uint32_t timeout = 100);

uint32_t uart::data_len()
{
    size_t len = 0;
    uart_get_buffered_data_len(m_uart_num, &len);
    return len;
}

uint32_t uart::data_read()
{
    return data_read_bytes;
}