// timer.c

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t milliseconds = 0;

void timer_init() {
    // Configure the timer to CTC (Clear Timer on Compare Match) mode
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;

    // Set the prescaler to divide by 16 and enable the timer
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm;

    // Set the compare match value for 1ms assuming a 20MHz clock
    TCA0.SINGLE.CMP0 = 1250;

    // Enable compare match interrupt
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0EN_bm;
}

ISR(TCA0_CMP0_vect) {
    milliseconds++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

uint32_t millis() {
    uint32_t millis_copy;

    // Disable interrupts while reading the milliseconds to ensure atomic access
    cli();
    millis_copy = milliseconds;
    sei();

    return millis_copy;
}
