#include "adc.h"

void setup_adc() {
    ADC0.CTRLC = ADC_PRESC_DIV16_gc;       // Set the ADC prescaler to 16 (20 MHz / 16 = 1.25 MHz ADC clock)
    ADC0.CTRLA = ADC_ENABLE_bm;            // Enable the ADC
    ADC0.CTRLC |= ADC_REFSEL_INTREF_gc;   // Use the internal reference
    VREF.CTRLA |= VREF_ADC0REFSEL_2V5_gc; // Set the internal reference to 2.5V for ADC0
}

uint16_t read_adc(uint8_t channel) {
    ADC0.MUXPOS = channel;  // Set the channel
    ADC0.COMMAND = ADC_STCONV_bm;  // Start conversion
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));  // Wait for conversion to complete
    ADC0.INTFLAGS = ADC_RESRDY_bm;  // Clear the interrupt flag
    return ADC0.RES;  // Return the ADC result
}

