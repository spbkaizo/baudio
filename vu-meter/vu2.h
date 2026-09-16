#ifndef VU2_H
#define VU2_H

#include <stdint.h>

/* setup_adc() and read_adc() belong to adc.h; they were redeclared here. */
void display_volume(uint16_t volume_left, uint16_t volume_right);
void volume_meter_task(uint8_t channel_left, uint8_t channel_right);

#endif // VU2_H
