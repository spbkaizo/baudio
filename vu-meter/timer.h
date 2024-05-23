// timer.h

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Initialize the timer
void timer_init();

// Return the number of milliseconds since timer was initialized
uint32_t millis();

#endif // TIMER_H
