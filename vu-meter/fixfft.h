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

// inline this here:
static inline char FIX_MPY(char a, char b) {
    int c = ((int)a * (int)b) >> 6;
    b = c & 0x01;
    a = (c >> 1) + b;
    return a;
}

#endif // FIXFFT_H
