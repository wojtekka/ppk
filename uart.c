#include "uart.h"

#include "system.h"

static __idata volatile uint8_t uart1_buf[16];
static __idata volatile uint8_t uart1_read = 0;
static __idata volatile uint8_t uart1_write = 0;

void UART_interrupt(void)
{
    if (U1RI)
    {
        U1RI = 0;

        uint8_t x = SBUF1;

        if (uart1_write - uart1_read == 16)
            return;

        uart1_buf[(uart1_write++) & 15] = x;
    }
}

void UART_init(void)
{
    U1REN = 1;
    SBAUD1 = 204;
    IE_UART1 = 1;
}

int UART_recv(void)
{
    int result = -1;

    EA = 0;
    if (uart1_read != uart1_write)
    {
        result = uart1_buf[(uart1_read++) & 15];
    }
    EA = 1;

    return result;
}

void UART_send(uint8_t x)
{
    SBUF1 = x;
    while (!U1TI);
    U1TI = 0;
}
