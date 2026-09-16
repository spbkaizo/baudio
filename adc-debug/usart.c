#include <avr/io.h>
#include "usart.h"

#define USART_BAUD 115200UL

/* tinyAVR 0/1-series BAUD is a 16-bit value with six fractional bits:
   BAUD = 64 * F_CPU / (16 * baud rate). The classic ATmega expression
   F_CPU/(16*baud) - 1 is not valid here. */
#define USART_BAUD_REG ((uint16_t)((64UL * F_CPU) / (16UL * USART_BAUD)))

void usart_init(void) {
    USART0.BAUD = USART_BAUD_REG;
    USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

    /* USART0 is on PB2 (TXD) and PB3 (RXD) in the default port mux
       position. PA1 and PA2 carry no USART signal on this part.

       PB2 is also where the VU meter sheet routes the right channel audio
       net; see HW-2 in HARDWARE-ISSUES.md. The two firmwares are separate
       builds so they never contend, but serial debug cannot be used while
       that input is connected. */
    PORTB.DIR |= (1 << 2);   // PB2 as output for USART TX
    PORTB.DIR &= ~(1 << 3);  // PB3 as input for USART RX
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
