// Converted to avr-libc by Simon B <simonb@kaizo.org> 2024
// 
// Original Arduino Source from : https://www.arduino.cc/reference/en/libraries/fix_fft/
#ifndef FIXFFT_H
#define FIXFFT_H

#include <avr/pgmspace.h>

/*
  fix_fft() - perform forward/inverse fast Fourier transform.
  fr[n],fi[n] are real and imaginary arrays, both INPUT AND
  RESULT (in-place FFT), with 0 <= n < 2**m; set inverse to
  0 for forward transform (FFT), or 1 for iFFT.
*/
int fix_fft(char fr[], char fi[], int m, int inverse);

/*
  fix_fftr() - forward/inverse FFT on array of real numbers.
  Real FFT/iFFT using half-size complex FFT by distributing
  even/odd samples into real/imaginary arrays respectively.
  In order to save data space (i.e. to avoid two arrays, one
  for real, one for imaginary samples), we proceed in the
  following two steps: a) samples are rearranged in the real
  array so that all even samples are in places 0-(N/2-1) and
  all imaginary samples in places (N/2)-(N-1), and b) fix_fft
  is called with fr and fi pointing to index 0 and index N/2
  respectively in the original array. The above guarantees
  that fix_fft "sees" consecutive real samples as alternating
  real and imaginary samples in the complex array.
*/
int fix_fftr(char f[], int m, int inverse);

/* Fixed-point multiply with rounding, returning a Q7 result.

   The product is taken in int, shifted down by 6, then rounded by adding the
   bit shifted out. The result is clamped to the signed 8-bit range: with both
   inputs at -128 the rounded value is +128, which does not fit and would wrap
   to -128, inverting the sign inside the butterfly. That case cannot arise
   while one operand always comes from the sine table, whose magnitude never
   exceeds 127, but the clamp costs little and removes the trap.

   int8_t rather than char, whose signedness is implementation-defined. */
static inline int8_t FIX_MPY(int8_t a, int8_t b) {
    int c = ((int)a * (int)b) >> 6;
    int rounded = (c >> 1) + (c & 0x01);

    if (rounded > 127) {
        rounded = 127;
    } else if (rounded < -128) {
        rounded = -128;
    }
    return (int8_t)rounded;
}

#endif // FIXFFT_H
