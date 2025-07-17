#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t milliseconds = 0;

void timer_init() {
    // Configure TCB0 to run in periodic interrupt mode
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    // Set the compare value for 1ms intervals assuming a 20MHz clock with no prescaler
    TCB0.CCMP = 20000;
    // Enable the timer and set the clock source to CLK_PER (no prescaler)
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;
    // Enable the interrupt
    TCB0.INTCTRL = TCB_CAPT_bm;
}

ISR(TCB0_INT_vect) {
    milliseconds++;
    TCB0.INTFLAGS = TCB_CAPT_bm;  // Clear the interrupt flag
}

uint32_t millis() {
    uint32_t millis_copy;
    cli();
    millis_copy = milliseconds;
    sei();
    return millis_copy;
}
