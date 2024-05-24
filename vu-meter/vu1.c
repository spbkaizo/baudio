#include "vu1.h"
#include "led_control.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#define LED_COUNT 8
#define NOISE_THRESHOLD 1 
#define SAMPLE_COUNT 100
#define SENSITIVITY 3 // adjust up/down if the vu meter isn't triggered satisfactorily at your normal listening volume
#define WINDOW_SIZE 24 // samples before updating - can make it smoother or faster.


uint16_t baseline = 0;

void calibrate_adc_baseline(uint8_t channel) {
    uint32_t sum = 0;
    uint16_t sample;

    // Take multiple samples
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sample = read_adc(channel);
        sum += sample;
        _delay_ms(5);  // Delay to allow ADC to settle
    }
    baseline = sum / SAMPLE_COUNT;
}

void display_volume(uint16_t volume) {
    static uint16_t readings[WINDOW_SIZE] = {0};
    static int readIndex = 0;
    static uint32_t sum = 0;
    static uint8_t numReadings = 0;

    // Subtract the oldest reading
    sum -= readings[readIndex];
    // Read new volume and store it in the array
    readings[readIndex] = volume;
    // Add the new reading to the sum
    sum += readings[readIndex];
    // Advance to the next position in the array
    readIndex = (readIndex + 1) % WINDOW_SIZE;

    // Increment the number of readings until the array is filled
    if (numReadings < WINDOW_SIZE) {
        numReadings++;
    }

    uint16_t averaged_volume = sum / numReadings;
    int16_t normalized_volume = averaged_volume - baseline;

    if (abs(normalized_volume) < NOISE_THRESHOLD) {
        normalized_volume = 0;
    }

    uint8_t ledLevel = (uint8_t)(abs(normalized_volume) * LED_COUNT / SENSITIVITY);
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        set_led_state(i, i < ledLevel ? 1 : 0);
    }
}


void volume_meter_task(uint8_t channel) {
    uint16_t volume = read_adc(channel);
    display_volume(volume);
}
