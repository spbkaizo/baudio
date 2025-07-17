#include "knight.h"
#include "led_control.h"
#include "main.h"
#include <avr/io.h>
#include <util/delay.h>

#define DELAY_TIME 50  // milliseconds

extern volatile uint8_t mode;

void knight_rider_effect() {
    while (mode == MODE_KNIGHT_RIDER) {
        for (int i = 0; i < 8 && mode == MODE_KNIGHT_RIDER; i++) {
            set_led_state(i, 1);
            _delay_ms(DELAY_TIME);
            set_led_state(i, 0);
        }
        
        for (int i = 6; i >= 0 && mode == MODE_KNIGHT_RIDER; i--) {
            set_led_state(i, 1);
            _delay_ms(DELAY_TIME);
            set_led_state(i, 0);
        }
    }
}

