#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "adc.h"
#include "led_control.h"
#include "knight.h"
#include "binary.h"
#include "vu1.h"

#define MODE_KNIGHT_RIDER 0
#define MODE_VU_METER 1
#define MODE_BINARY 2

#define BUTTON_PIN PIN3_bp

#define BAUD_RATE 115200

#define DEBOUNCE_THRESHOLD 0  // Increase the debounce threshold

volatile uint8_t mode = MODE_VU_METER;

volatile uint8_t last_state = 1;  // Initialize to 1 since the pull-up resistor keeps the pin high when not pressed

ISR(PORTB_PORT_vect) {
    static uint8_t debounce_counter = 0;
    uint8_t current_state = PORTB.IN & (1 << BUTTON_PIN);

    // Only process button press if state has been stable for 10 checks
    if (current_state == last_state) {
        if (debounce_counter < DEBOUNCE_THRESHOLD ) {
            debounce_counter++;
        } else {
            // Act on button press
            if (current_state == 0) {  // Check if button is pressed (state is low)
                mode = (mode + 1) % 3;
                debounce_counter = 0;  // Reset counter after action
            }
        }
    } else {
        debounce_counter = 0;  // Reset counter if state changes
    }
    last_state = current_state;
    PORTB.INTFLAGS = (1 << BUTTON_PIN);  // Clear the interrupt flag
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
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;  // Enable pull-up and trigger on falling edge
}


int main(void) {
    usart_init();  // Initialize USART
    init_ports();
    setup_adc();
    sei();  // Enable global interrupts
    calibrate_adc_baseline(ADC_CHANNEL_LEFT);

    //usart_print("Hello, world!\n");

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

