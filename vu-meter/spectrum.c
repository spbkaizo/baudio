// spectrum.c

#include "spectrum.h"
#include "fixfft.h"
#include "adc.h"
#include "led_control.h"

#define FFT_SIZE 256            // Size of the FFT. Must be a power of two.
#define LOG2_FFT_SIZE 8         // log2(FFT_SIZE)
#define USABLE_BINS (FFT_SIZE / 2)  // The upper half mirrors the lower half.

/* Magnitude a band must reach before its LED lights. fix_fft output is
   scaled down as it goes, so this sits well below the nominal full scale. */
#define BAND_THRESHOLD 8

static char real[FFT_SIZE];     // Array for real components of the signal.
static char imag[FFT_SIZE];     // Array for imaginary components of the signal (initially zero).

void spectrum_init() {
    /* The ADC is configured once by main(); this only clears the buffers. */
    for (int i = 0; i < FFT_SIZE; i++) {
        real[i] = 0;
        imag[i] = 0;
    }
}

void spectrum_update() {
    /* Gather samples. The ADC is 10-bit unsigned; fix_fft wants signed 8-bit
       samples centred on zero, so shift down to 0..255 and bias by 128. */
    for (int i = 0; i < FFT_SIZE; i++) {
        real[i] = (char)((int16_t)(read_adc(ADC_CHANNEL_LEFT) >> 2) - 128);
        imag[i] = 0;
    }

    fix_fft(real, imag, LOG2_FFT_SIZE, 0);  // 0 for forward FFT.

    /* Fold the usable bins into one band per LED. Bin 0 is the DC term and
       carries no audio, so skip it. Each band takes the largest magnitude it
       contains, which keeps a narrow tone visible instead of averaging it
       away. Magnitude uses |re| + |im|, close enough to the true modulus for
       a threshold test and far cheaper than sqrt() on an 8-bit core. */
    const uint8_t band_width = (USABLE_BINS - 1) / LED_COUNT;

    for (uint8_t led = 0; led < LED_COUNT; led++) {
        int16_t peak = 0;
        uint8_t first = 1 + led * band_width;

        for (uint8_t n = 0; n < band_width; n++) {
            uint8_t bin = first + n;
            int16_t re = real[bin];
            int16_t im = imag[bin];
            int16_t magnitude = (re < 0 ? -re : re) + (im < 0 ? -im : im);

            if (magnitude > peak) {
                peak = magnitude;
            }
        }

        /* set_led_state() ORs its argument into the port, so it must be given
           0 or 1 and nothing else. */
        set_led_state(led, peak >= BAND_THRESHOLD ? 1 : 0);
    }
}
