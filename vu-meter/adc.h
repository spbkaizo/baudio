// adc.h

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

// ADC channel definitions
#define ADC_CHANNEL_RIGHT ADC_MUXPOS_AIN10_gc  // PB2
#define ADC_CHANNEL_LEFT  ADC_MUXPOS_AIN7_gc   // PA7

// Function prototypes
void setup_adc();
uint16_t read_adc(uint8_t channel);

#endif // ADC_H
