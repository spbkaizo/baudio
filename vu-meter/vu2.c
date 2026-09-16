#include "vu2.h"
#include "led_control.h"
#include <avr/io.h>
#include <stdint.h>
#include "adc.h"
#include "main.h"  // Include the header file to access global baselines

#define HALF_COUNT (LED_COUNT / 2)  // LEDs available to each channel

/* Deviation from the baseline, in ADC counts, that lights the last LED.
   With the 4.34 V reference one count is about 4.2 mV, so 192 counts is
   roughly 0.8 V of swing. Lower it if the meter sits short of full scale at
   your normal listening volume, raise it if it pins. */
#define SENSITIVITY 192

/* Deviations below this are treated as silence, to stop the meter flickering
   on converter noise and hum. */
#define NOISE_THRESHOLD 2

/* Samples in the moving average. Larger is smoother but slower to respond. */
#define WINDOW_SIZE 24

/* Rectified moving average, one instance per channel.

   The sample is rectified against the baseline before it enters the window.
   Averaging the raw reading and subtracting the baseline afterwards lets the
   positive and negative halves of the waveform cancel, so the result depends
   on where the window happens to fall in the cycle. Rectifying first gives a
   mean deviation that rises with amplitude regardless of phase, which is
   what a level meter needs. */
typedef struct {
    uint16_t readings[WINDOW_SIZE];
    uint16_t sum;
    uint8_t index;
    uint8_t count;
} envelope_t;

/* Adds one reading and returns the current mean deviation.
   sum stays within uint16_t: WINDOW_SIZE * 1023 is 24552. */
static uint16_t envelope_update(envelope_t *env, uint16_t sample, uint16_t baseline) {
    uint16_t deviation = (sample > baseline) ? (sample - baseline)
                                             : (baseline - sample);

    env->sum -= env->readings[env->index];
    env->readings[env->index] = deviation;
    env->sum += deviation;
    env->index = (uint8_t)((env->index + 1) % WINDOW_SIZE);

    if (env->count < WINDOW_SIZE) {
        env->count++;
    }
    return (uint16_t)(env->sum / env->count);
}

/* Converts a mean deviation into a lit-LED count, saturating rather than
   wrapping: the product overflows a uint8_t well inside the input range. */
static uint8_t scale_to_leds(uint16_t level) {
    uint32_t scaled;

    if (level < NOISE_THRESHOLD) {
        return 0;
    }
    scaled = ((uint32_t)level * HALF_COUNT) / SENSITIVITY;
    if (scaled > HALF_COUNT) {
        scaled = HALF_COUNT;
    }
    return (uint8_t)scaled;
}

void display_volume(uint16_t volume_left, uint16_t volume_right) {
    static envelope_t env_left;
    static envelope_t env_right;

    uint8_t level_left = scale_to_leds(envelope_update(&env_left, volume_left, baseline_left));
    uint8_t level_right = scale_to_leds(envelope_update(&env_right, volume_right, baseline_right));

    /* Left channel occupies the low half of the bar, right the high half. */
    for (uint8_t i = 0; i < HALF_COUNT; i++) {
        set_led_state(i, i < level_left ? 1 : 0);
    }
    for (uint8_t i = 0; i < HALF_COUNT; i++) {
        set_led_state(HALF_COUNT + i, i < level_right ? 1 : 0);
    }
}

void volume_meter_task(uint8_t channel_left, uint8_t channel_right) {
    uint16_t volume_left = read_adc(channel_left);
    uint16_t volume_right = read_adc(channel_right);
    display_volume(volume_left, volume_right);
}
