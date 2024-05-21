#ifndef VU1_H
#define VU1_H

#include <stdint.h>

void setup_adc();
void volume_meter_task(uint8_t channel);
uint16_t read_adc(uint8_t channel);

#endif // VU1_H

