#include "vu2.h"
#include "led_control.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "adc.h"
#include "main.h"  // Include the header file to access global baselines

#define LED_COUNT 8
#define NOISE_THRESHOLD 1 
#define SAMPLE_COUNT 100
#define SENSITIVITY 3 // adjust up/down if the vu meter isn't triggered satisfactorily at your normal listening volume
#define WINDOW_SIZE 24 // samples before updating - can make it smoother or faster.

void display_volume(uint16_t volume_left, uint16_t volume_right) {
    static uint16_t readings_left[WINDOW_SIZE] = {0};
    static uint16_t readings_right[WINDOW_SIZE] = {0};
    static int readIndex = 0;
    static uint32_t sum_left = 0;
    static uint32_t sum_right = 0;
    static uint8_t numReadings = 0;

    // Update readings and sums for left channel
    sum_left -= readings_left[readIndex];
    readings_left[readIndex] = volume_left;
    sum_left += readings_left[readIndex];

    // Update readings and sums for right channel
    sum_right -= readings_right[readIndex];
    readings_right[readIndex] = volume_right;
    sum_right += readings_right[readIndex];

    // Advance to the next position in the array
    readIndex = (readIndex + 1) % WINDOW_SIZE;

    // Increment the number of readings until the array is filled
    if (numReadings < WINDOW_SIZE) {
        numReadings++;
    }

    uint16_t averaged_volume_left = sum_left / numReadings;
    uint16_t averaged_volume_right = sum_right / numReadings;

    int16_t normalized_volume_left = averaged_volume_left - baseline_left;  // Use the correct baseline
    int16_t normalized_volume_right = averaged_volume_right - baseline_right;  // Use the correct baseline

    if (abs(normalized_volume_left) < NOISE_THRESHOLD) {
        normalized_volume_left = 0;
    }
    if (abs(normalized_volume_right) < NOISE_THRESHOLD) {
        normalized_volume_right = 0;
    }

    uint8_t ledLevel_left = (uint8_t)(abs(normalized_volume_left) * (LED_COUNT / 2) / SENSITIVITY);
    uint8_t ledLevel_right = (uint8_t)(abs(normalized_volume_right) * (LED_COUNT / 2) / SENSITIVITY);

    for (uint8_t i = 0; i < LED_COUNT / 2; i++) {
        set_led_state(i, i < ledLevel_left ? 1 : 0);
    }
    for (uint8_t i = LED_COUNT / 2; i < LED_COUNT; i++) {
        set_led_state(i, (i - LED_COUNT / 2) < ledLevel_right ? 1 : 0);
    }
}

void volume_meter_task(uint8_t channel_left, uint8_t channel_right) {
    uint16_t volume_left = read_adc(channel_left);
    uint16_t volume_right = read_adc(channel_right);
    display_volume(volume_left, volume_right);
}

