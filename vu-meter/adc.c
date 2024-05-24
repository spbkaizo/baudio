// adc.c

#include "adc.h"

void setup_adc() {
    // Set the ADC prescaler to 16 (20 MHz / 16 = 1.25 MHz ADC clock)
    ADC0.CTRLC = ADC_PRESC_DIV16_gc | ADC_REFSEL_INTREF_gc; // Use internal reference

    // Enable the ADC
    ADC0.CTRLA = ADC_ENABLE_bm;

    // Set the internal reference to 2.5V for ADC0
    //VREF.CTRLA |= VREF_ADC0REFSEL_2V5_gc;
    //VREF.CTRLA |= VREF_ADC0REFSEL_1V1_gc;
    VREF.CTRLA |= VREF_ADC0REFSEL_4V34_gc;
    //VREF.CTRLA |= VREF_ADC0REFSEL_VDD_gc;
}

uint16_t read_adc(uint8_t channel) {
    // Select the ADC channel
    ADC0.MUXPOS = channel;

    // Start the conversion
    ADC0.COMMAND = ADC_STCONV_bm;

    // Wait for the conversion to complete
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));

    // Clear the interrupt flag
    ADC0.INTFLAGS = ADC_RESRDY_bm;

    // Return the ADC result
    return ADC0.RES;
}
