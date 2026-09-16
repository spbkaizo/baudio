// cylon.c
#include <util/delay.h>
#include "cylon.h"
#include "led_control.h"
#include "main.h"

/* Middle-out scanner. The two centre LEDs light first and the pair sweeps
   outwards to the ends, then back in.

   LEDs are driven through set_led_state() rather than by writing PORTA
   directly: the eight LEDs are split across both ports and are not in pin
   order (LED 0 is PB1, LED 5 is PA6), so a raw port write lights the wrong
   ones and misses the two on PORTB entirely. */

#define STEP_DELAY 60  // milliseconds per step
#define HALF (LED_COUNT / 2)

void init_cylon() {
    /* Pin directions are set once by init_ports() in main.c. */
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        set_led_state(i, 0);
    }
}

/* Lights the mirrored pair at distance `offset` from the centre. */
static void show_pair(uint8_t offset) {
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        set_led_state(i, 0);
    }
    set_led_state(HALF - 1 - offset, 1);  // inner half, sweeping left
    set_led_state(HALF + offset, 1);      // outer half, sweeping right
}

void run_cylon() {
    while (mode == MODE_MIDDLE_OUT) {
        // Outwards: centre to the ends.
        for (uint8_t offset = 0; offset < HALF && mode == MODE_MIDDLE_OUT; offset++) {
            show_pair(offset);
            _delay_ms(STEP_DELAY);
        }

        // Inwards: ends back to the centre, skipping the positions just shown.
        for (uint8_t offset = HALF - 1; offset > 0 && mode == MODE_MIDDLE_OUT; offset--) {
            show_pair(offset - 1);
            _delay_ms(STEP_DELAY);
        }
    }
}
