#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

/* AIN10 is PB1, not PB2. See the note in vu-meter/adc.h: the right channel
   as wired on the v1.4 board goes to PB2, which has no ADC channel. */
#define ADC_CHANNEL_RIGHT ADC_MUXPOS_AIN10_gc  // PB1

void setup_adc();
uint16_t read_adc(uint8_t channel);

#endif // ADC_H



