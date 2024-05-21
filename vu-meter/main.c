#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led_control.h"
#include "knight.h"
#include "binary.h"
#include "vu1.h"  // Make sure this is included

#define MODE_KNIGHT_RIDER 0
#define MODE_VU_METER 1
#define MODE_BINARY 2

#define BUTTON_PIN PIN3_bp

// Define ADC channels based on connected pins
#define ADC_CHANNEL_RIGHT ADC_MUXPOS_AIN10_gc  // : PB2
#define ADC_CHANNEL_LEFT  ADC_MUXPOS_AIN7_gc   // : PA7

#define BAUD_RATE 115200

volatile uint8_t mode = MODE_KNIGHT_RIDER;

ISR(PORTB_PORT_vect) {
    if (PORTB.INTFLAGS & (1 << BUTTON_PIN)) {  // Check if the interrupt is from the button pin
        mode = (mode + 1) % 3;  // Cycle through the modes
        PORTB.INTFLAGS = (1 << BUTTON_PIN);  // Clear the interrupt flag
    }
}


void usart_init() {
    // Set the baud rate
    uint16_t ubrr_value = (F_CPU / (16 * BAUD_RATE)) - 1;
    USART0.BAUD = ubrr_value;

    // Configure USART0 mode: asynchronous, no parity, one stop bit, 8-bit size
    USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;

    // Enable the transmitter and receiver
    USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void usart_putchar(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm));  // Wait for the data register to be empty
    USART0.TXDATAL = c;
}

void usart_print(const char* str) {
    while (*str) {
        usart_putchar(*str++);
    }
}

void init_ports() {
    PORTA.DIR |= (1<<PIN1_bp) | (1<<PIN2_bp) | (1<<PIN3_bp) | (1<<PIN6_bp) | (1<<PIN5_bp) | (1<<PIN4_bp);
    PORTB.DIR |= (1<<PIN1_bp) | (1<<PIN0_bp);
    // Configure PB3 as input with pull-up enabled
    PORTB.DIR &= ~(1<<BUTTON_PIN);
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
}


int main(void) {
    usart_init();  // Initialize USART
    init_ports();
    setup_adc();
	sei();  // Enable global interrupts

    //usart_print("Hello, world!\n");

    uint8_t mode = 0; // Start with Knight Rider effect
    uint8_t knight_rider_cycles = 0;


    while(1) {
		switch (mode) {
            case MODE_KNIGHT_RIDER:
                knight_rider_effect();
                break;
            case MODE_VU_METER:
                volume_meter_task(ADC_CHANNEL_LEFT); // Assuming ADC channel 0 for VU meter
                break;
            case MODE_BINARY:
                binary_count();
                break;
        }
    }
}

