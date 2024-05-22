// cylon.c
#include <util/delay.h>
#include "cylon.h"
#include "adc.h"  // Include ADC functionality for speed control

#define LED_PORT PORTA
#define LED_PIN_START 1
#define LED_PIN_COUNT 5  // Number of LEDs in the scanner

void init_cylon() {
    // Initialize LEDs as outputs
    LED_PORT.DIR |= ((1 << LED_PIN_COUNT) - 1) << LED_PIN_START;
}

void run_cylon() {
    static uint8_t direction = 0;  // 0 for increasing, 1 for decreasing
    static uint8_t current_led = LED_PIN_START;

    // Read ADC value to determine delay (speed control)
    uint16_t speed = read_adc(ADC_CHANNEL_RIGHT);  // Adjust channel as needed
    uint8_t delay = (speed / 512) + 10;  // Scale and offset the delay to make it noticeable

    // Reset all LEDs
    LED_PORT.OUT &= ~(((1 << LED_PIN_COUNT) - 1) << LED_PIN_START);
    
    // Set current LED
    LED_PORT.OUT |= (1 << current_led);

    // Update current LED based on direction
    if (!direction) {
        if (current_led < LED_PIN_START + LED_PIN_COUNT - 1) {
            current_led++;
        } else {
            direction = 1;  // Change direction to decreasing
        }
    } else {
        if (current_led > LED_PIN_START) {
            current_led--;
        } else {
            direction = 0;  // Change direction to increasing
        }
    }

    // Delay based on ADC value
    while (delay--) {
        _delay_ms(1);  // Simple delay; consider using timers for non-blocking delay
    }
}

