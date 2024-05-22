#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

#define ADC_CHANNEL_RIGHT ADC_MUXPOS_AIN10_gc  // PB2

void setup_adc();
uint16_t read_adc(uint8_t channel);

#endif // ADC_H



