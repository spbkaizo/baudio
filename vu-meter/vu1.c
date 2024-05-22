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
    uint16_t sample;

    // Take multiple samples
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sample = read_adc(channel);
        sum += sample;
        _delay_ms(10);  // Delay to allow ADC to settle
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

void volume_meter_task(uint8_t channel) {
    uint16_t volume = read_adc(channel);
    display_volume(volume);
}
