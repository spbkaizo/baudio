#include "led_control.h"
#include <avr/io.h>

void set_led_state(uint8_t led_num, uint8_t state) {
    switch(led_num) {
        case 0: PORTB.OUT = (PORTB.OUT & ~(1<<PIN1_bp)) | (state << PIN1_bp); break;
        case 1: PORTB.OUT = (PORTB.OUT & ~(1<<PIN0_bp)) | (state << PIN0_bp); break;
        case 2: PORTA.OUT = (PORTA.OUT & ~(1<<PIN1_bp)) | (state << PIN1_bp); break;
        case 3: PORTA.OUT = (PORTA.OUT & ~(1<<PIN2_bp)) | (state << PIN2_bp); break;
        case 4: PORTA.OUT = (PORTA.OUT & ~(1<<PIN3_bp)) | (state << PIN3_bp); break;
        case 5: PORTA.OUT = (PORTA.OUT & ~(1<<PIN6_bp)) | (state << PIN6_bp); break;
        case 6: PORTA.OUT = (PORTA.OUT & ~(1<<PIN5_bp)) | (state << PIN5_bp); break;
        case 7: PORTA.OUT = (PORTA.OUT & ~(1<<PIN4_bp)) | (state << PIN4_bp); break;
    }
}
