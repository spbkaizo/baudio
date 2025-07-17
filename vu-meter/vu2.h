#ifndef VU2_H
#define VU2_H

#include <stdint.h>

void setup_adc();
uint16_t read_adc(uint8_t channel);
void volume_meter_task(uint8_t channel_left, uint8_t channel_right);

#endif // VU2_H
