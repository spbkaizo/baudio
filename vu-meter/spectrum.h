// spectrum.h

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <stdint.h>
#include <math.h>

// Initializes the spectrum analyzer module.
void spectrum_init();

// Updates the spectrum analyzer, should be called regularly.
void spectrum_update();

#endif // SPECTRUM_H

