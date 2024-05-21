#include "vu1.h"
#include "led_control.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>  // Make sure to include stdlib for abs()

#define ADC_MIDPOINT 512  // Adjust this if necessary
#define ADC_MAX_VALUE 1023
#define LED_COUNT 8
#define NOISE_THRESHOLD 5  // Possibly lower this if it's too high
#define SAMPLE_COUNT 100

uint16_t baseline = 0;

void calibrate_adc_baseline(uint8_t channel) {
    uint32_t sum = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sum += read_adc(channel);  // Assuming channel 0 is used for audio input
        _delay_ms(10);       // Delay to allow for new readings
    }
    baseline = sum / SAMPLE_COUNT;
}


void display_volume(uint16_t volume) {
    int16_t normalized_volume = volume - baseline;

    if (abs(normalized_volume) < NOISE_THRESHOLD) {
        normalized_volume = 0;
    }

    uint8_t ledLevel = (uint8_t)(abs(normalized_volume) * LED_COUNT / 200);  // Adjusted for expected 0.4V swing

    for (uint8_t i = 0; i < LED_COUNT; i++) {
        set_led_state(i, i < ledLevel ? 1 : 0);
    }
}

void setup_adc() {
    ADC0.CTRLC = ADC_PRESC_DIV16_gc;       // Set the ADC prescaler to 16 (20 MHz / 16 = 1.25 MHz ADC clock)
    ADC0.CTRLA = ADC_ENABLE_bm;            // Enable the ADC
	ADC0.CTRLC |= ADC_REFSEL_INTREF_gc;   // Use the internal reference
    VREF.CTRLA |= VREF_ADC0REFSEL_2V5_gc; // Set the internal reference to 2.5V for ADC0
}

uint16_t read_adc(uint8_t channel) {
    ADC0.MUXPOS = channel;  // Set the ADC channel to read
    ADC0.COMMAND = ADC_STCONV_bm;  // Start an ADC conversion
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));  // Wait for conversion to complete
    ADC0.INTFLAGS = ADC_RESRDY_bm;  // Clear the interrupt flag
    return ADC0.RES;  // Return the ADC result
}

void volume_meter_task(uint8_t channel) {
    uint16_t volume = read_adc(channel);
    display_volume(volume);
}
