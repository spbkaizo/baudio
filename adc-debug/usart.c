#include <avr/io.h>
#include "usart.h"

#define F_CPU 20000000UL
#define USART_BAUD 115200
#define BSEL ((F_CPU / (16 * USART_BAUD)) - 1)

void usart_init(void) {
    USART0.BAUD = (BSEL & 0x0FFF);
    USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

    PORTA.DIR |= (1 << 1);  // Set PA1 as output for USART TX
    PORTA.DIR &= ~(1 << 2); // Set PA2 as input for USART RX
}

void usart_write_char(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm));  // Wait for data register empty
    USART0.TXDATAL = c;
}

void usart_write_string(const char* str) {
    while (*str) {
        usart_write_char(*str++);
    }
}
