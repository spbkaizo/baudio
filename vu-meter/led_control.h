#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>

#define LED_COUNT 8  // Total number of LEDs

void set_led_state(uint8_t led_num, uint8_t state);

#endif // LED_CONTROL_H
