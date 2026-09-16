#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

/* ADC channel definitions.

   AIN7 is PA7, which matches the adclo net on the VU meter schematic.

   AIN10 is PB1, NOT PB2 as the comment here used to claim. Two things do
   not line up with that:

     - main.c drives PB1 as an output for LED 0, so sampling AIN10 reads a
       pin this firmware is itself driving.
     - the schematic routes the adcro net to PB2, and PB2 has no ADC channel
       on this part at all. The ATtiny1614 analogue inputs are AIN0-AIN7 on
       PA0-PA7 plus AIN10 on PB1 and AIN11 on PB0.

   So the right channel as wired on the v1.4 board cannot be read by the
   ADC, and the channel selected here is a pin used for an LED. Left it as
   found pending a check against the assembled board: changing it blind
   would only move the problem to a different wrong pin. AIN11 (PB0) is the
   nearest usable input if the board is reworked, though PB0 is currently
   LED 1. */
#define ADC_CHANNEL_RIGHT ADC_MUXPOS_AIN10_gc  // PB1 - see note above
#define ADC_CHANNEL_LEFT  ADC_MUXPOS_AIN7_gc   // PA7

// Function prototypes
void calibrate_adc_baseline(uint8_t channel, uint16_t *baseline);
void setup_adc();
uint16_t read_adc(uint8_t channel);

#endif // ADC_H
