#include "config.h"
#include "system.h"
#include "delay.h"
#include "usb_conkbd.h"
#include "usb_handler.h"
#include "gpio.h"
#include "mapping.h"
#include "uart.h"

void USB_ISR(void) __interrupt(INT_NO_USB)
{
    USB_interrupt();
}

void UART1_ISR(void) __interrupt(INT_NO_UART1)
{
    UART_interrupt();
}

#if 0
void print_char(char c)
{
    if (c == '\n')
    {
        print_char('\r');
    }

    UART_send(c);
}

void print_hex(char x)
{
    static const char hex[] = "0123456789abcdef";
    print_char(hex[((unsigned char)x) >> 4]);
    print_char(hex[((unsigned char)x) & 15]);
}

void print(const char *str)
{
    while (*str != 0)
    {
        print_char(*(str++));
    }
}
#endif

void main(void)
{
    /* RTS low */
    PIN_output(P15);
    PIN_low(P15);

    // Setup
    UART_init();
    CLK_config();
    DLY_ms(10); // wait for clock to settle

    // Wait for DCD
    PIN_input(P14);
    while (PIN_read(P14) != 1);

    // Set RTS high
    PIN_high(P15);

    // Init USB keyboard
    KBD_init();
    WDT_start();

    while(1)
    {
        WDT_reset();

        int tmp = UART_recv();
        if (tmp != -1)
        {
            MAP_handle(tmp);
        }
    }
}
