#include "binary.h"
#include "led_control.h"
#include <avr/io.h>
#include <util/delay.h>

#define DELAY_TIME 50  // Set delay time to 500 milliseconds

void display_binary(uint8_t number) {
    // Map each bit of the number to the corresponding LED
    PORTA.OUT = (PORTA.OUT & 0x1E) | ((number & 0x1) << PIN4_bp) | ((number & 0x2) << (PIN5_bp-1)) | ((number & 0x4) << (PIN6_bp-2)) | ((number & 0x8) << (PIN3_bp-1));
    PORTB.OUT = (PORTB.OUT & 0xFC) | ((number & 0x10) >> 4) | ((number & 0x20) >> 4) | ((number & 0x40) >> 6) | ((number & 0x80) >> 6);
}

void binary_count() {
    for (uint8_t i = 0; i < 255; i++) {
        display_binary(i);
        _delay_ms(DELAY_TIME);
    }
}
