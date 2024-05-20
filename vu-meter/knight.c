#include "knight.h"
#include "led_control.h"
#include <avr/io.h>
#include <util/delay.h>

#define DELAY_TIME 10  // milliseconds

void knight_rider_effect() {
    for (int i = 0; i < 8; i++) {
        set_led_state(i, 1);
        _delay_ms(DELAY_TIME);
        set_led_state(i, 0);
    }
    
    for (int i = 6; i >= 0; i--) {
        set_led_state(i, 1);
        _delay_ms(DELAY_TIME);
        set_led_state(i, 0);
    }
}
