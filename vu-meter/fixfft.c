#include "fixfft.h"
#include <avr/pgmspace.h>
#include <math.h>  // If needed for any math operations not shown

#define N_WAVE      256    // Full length of Sinewave[]
#define LOG2_N_WAVE 8      // log2(N_WAVE)

/* Sinewave[i] = round(127 * sin(2*pi*i / N_WAVE)), i = 0 .. N_WAVE-N_WAVE/4-1.
   Only three quarters of the cycle are stored; fix_fft() reads the cosine
   term at an offset of N_WAVE/4 into the same table. */
const int8_t Sinewave[N_WAVE-N_WAVE/4] PROGMEM = {
       0,    3,    6,    9,   12,   16,   19,   22,   25,   28,   31,   34,
      37,   40,   43,   46,   49,   51,   54,   57,   60,   63,   65,   68,
      71,   73,   76,   78,   81,   83,   85,   88,   90,   92,   94,   96,
      98,  100,  102,  104,  106,  107,  109,  111,  112,  113,  115,  116,
     117,  118,  120,  121,  122,  122,  123,  124,  125,  125,  126,  126,
     126,  127,  127,  127,  127,  127,  127,  127,  126,  126,  126,  125,
     125,  124,  123,  122,  122,  121,  120,  118,  117,  116,  115,  113,
     112,  111,  109,  107,  106,  104,  102,  100,   98,   96,   94,   92,
      90,   88,   85,   83,   81,   78,   76,   73,   71,   68,   65,   63,
      60,   57,   54,   51,   49,   46,   43,   40,   37,   34,   31,   28,
      25,   22,   19,   16,   12,    9,    6,    3,    0,   -3,   -6,   -9,
     -12,  -16,  -19,  -22,  -25,  -28,  -31,  -34,  -37,  -40,  -43,  -46,
     -49,  -51,  -54,  -57,  -60,  -63,  -65,  -68,  -71,  -73,  -76,  -78,
     -81,  -83,  -85,  -88,  -90,  -92,  -94,  -96,  -98, -100, -102, -104,
    -106, -107, -109, -111, -112, -113, -115, -116, -117, -118, -120, -121,
    -122, -122, -123, -124, -125, -125, -126, -126, -126, -127, -127, -127
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
