#include "fixfft.h"
#include <avr/pgmspace.h>
#include <math.h>  // If needed for any math operations not shown

#define N_WAVE      256    // Full length of Sinewave[]
#define LOG2_N_WAVE 8      // log2(N_WAVE)

// The Sinewave is a quarter sine but to be used in full circle (0-255)
const int8_t Sinewave[N_WAVE-N_WAVE/4] PROGMEM = {
    // ... your existing Sinewave data ...
};

/*
  fix_fft() - perform forward/inverse fast Fourier transform.
  fr[n],fi[n] are real and imaginary arrays, both INPUT AND
  RESULT (in-place FFT), with 0 <= n < 2**m; set inverse to
  0 for forward transform (FFT), or 1 for iFFT.
*/
int fix_fft(char fr[], char fi[], int m, int inverse) {
    int mr, nn, i, j, l, k, istep, n, scale, shift;
    char qr, qi, tr, ti, wr, wi;

    n = 1 << m;
    if (n > N_WAVE) return -1;

    mr = 0;
    nn = n - 1;
    scale = 0;

    for (m = 1; m <= nn; ++m) {
        l = n;
        do {
            l >>= 1;
        } while (mr + l > nn);
        mr = (mr & (l-1)) + l;

        if (mr <= m) continue;
        tr = fr[m];
        fr[m] = fr[mr];
        fr[mr] = tr;
        ti = fi[m];
        fi[m] = fi[mr];
        fi[mr] = ti;
    }

    l = 1;
    k = LOG2_N_WAVE - 1;
    while (l < n) {
        if (inverse) {
            shift = 0;
            for (i = 0; i < n; ++i) {
                j = fr[i];
                if (j < 0) j = -j;
                m = fi[i];
                if (m < 0) m = -m;
                if (j > 16383 || m > 16383) {
                    shift = 1;
                    break;
                }
            }
            if (shift) ++scale;
        } else {
            shift = 1;
        }

        istep = l << 1;
        for (m = 0; m < l; ++m) {
            j = m << k;
            wr = pgm_read_byte_near(Sinewave + j + N_WAVE/4);
            wi = -pgm_read_byte_near(Sinewave + j);
            if (inverse) wi = -wi;
            if (shift) {
                wr >>= 1;
                wi >>= 1;
            }

            for (i = m; i < n; i += istep) {
                j = i + l;
                tr = FIX_MPY(wr, fr[j]) - FIX_MPY(wi, fi[j]);
                ti = FIX_MPY(wr, fi[j]) + FIX_MPY(wi, fr[j]);
                qr = fr[i];
                qi = fi[i];
                if (shift) {
                    qr >>= 1;
                    qi >>= 1;
                }
                fr[j] = qr - tr;
                fi[j] = qi - ti;
                fr[i] = qr + tr;
                fi[i] = qi + ti;
            }
        }
        --k;
        l = istep;
    }
    return scale;
}

/*
  fix_fftr() - forward/inverse FFT on array of real numbers.
*/
int fix_fftr(char f[], int m, int inverse) {
    int i, N = 1<<(m-1), scale = 0;
    char tt, *fr=f, *fi=&f[N];

    if (inverse)
        scale = fix_fft(fi, fr, m-1, inverse);
    for (i = 1; i < N; i += 2) {
        tt = f[N + i - 1];
        f[N + i - 1] = f[i];
        f[i] = tt;
    }
    if (!inverse)
        scale = fix_fft(fi, fr, m-1, inverse);
    return scale;
}
