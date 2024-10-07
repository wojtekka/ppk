#pragma once

#include <stdint.h>

/** Init UART to 9600 bps. */
void UART_init(void);

/** UART interrupt handler. */
void UART_interrupt(void);

/** Receive data from UART.
 * \return byte received or -1 if nothing received.
 */
int UART_recv(void);

/** Send byte to UART.
 * \param x byte to be sent
 * \note blocking function
 */
void UART_send(uint8_t x);
