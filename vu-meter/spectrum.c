// spectrum.c

#include "spectrum.h"
#include "fixfft.h"
#include "adc.h"
#include "led_control.h"

#define FFT_SIZE 256            // Size of the FFT. Must be a power of two.
#define LOG2_FFT_SIZE 8         // log2(FFT_SIZE)

static char real[FFT_SIZE];     // Array for real components of the signal.
static char imag[FFT_SIZE];     // Array for imaginary components of the signal (initially zero).

void spectrum_init() {
    setup_adc();  // Setup ADC from adc.h
    // Initialize real and imaginary arrays to zero.
    for (int i = 0; i < FFT_SIZE; i++) {
        real[i] = 0;
        imag[i] = 0;
    }
}

void spectrum_update() {
    // Gather samples for FFT.
    for (int i = 0; i < FFT_SIZE; i++) {
        real[i] = read_adc(ADC_CHANNEL_LEFT) >> 2; // Assuming 10-bit ADC, shift to scale down to 8 bits.
        imag[i] = 0; // FFT expects complex numbers; imag part is 0 for real inputs.
    }

    // Perform FFT.
    fix_fft(real, imag, LOG2_FFT_SIZE, 0); // 0 for forward FFT.

    // Update LEDs based on FFT output with logarithmic scaling.
    for (int i = 0; i < FFT_SIZE / 2; i++) { // Use only the first half of the FFT output (symmetrical).
        int magnitude = sqrt(real[i] * real[i] + imag[i] * imag[i]);
        
        // Logarithmic scaling: log10(1 + magnitude) to avoid log(0)
        // Scale to fit LED brightness levels, adjust scale factor as needed
        uint8_t scaled_magnitude = (uint8_t)(log10(1 + magnitude) * (255 / log10(1 + 1024))); // Example scale for 8-bit brightness

        set_led_state(i % LED_COUNT, scaled_magnitude);
    }
}
