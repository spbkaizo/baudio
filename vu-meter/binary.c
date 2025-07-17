#include "binary.h"
#include "led_control.h"
#include <avr/io.h>
#include <util/delay.h>
#include "main.h"

#define DELAY_TIME 50

void display_binary(uint8_t number) {
    // Use set_led_state to set each LED according to the bits of the number
    set_led_state(0, (number & (1 << 0)) >> 0);
    set_led_state(1, (number & (1 << 1)) >> 1);
    set_led_state(2, (number & (1 << 2)) >> 2);
    set_led_state(3, (number & (1 << 3)) >> 3);
    set_led_state(4, (number & (1 << 4)) >> 4);
    set_led_state(5, (number & (1 << 5)) >> 5);
    set_led_state(6, (number & (1 << 6)) >> 6);
    set_led_state(7, (number & (1 << 7)) >> 7);
}

void binary_count() {
    // Count up
    for (uint8_t i = 0; i < 255 && mode == MODE_BINARY; i++) {
        display_binary(i);
        _delay_ms(DELAY_TIME);
    }
    
    // Check if mode has changed
    if (mode != MODE_BINARY) {
        return;
    }

    // Count down
    for (uint8_t i = 255; i > 0 && mode == MODE_BINARY; i--) {
        display_binary(i);
        _delay_ms(DELAY_TIME);
    }
}
