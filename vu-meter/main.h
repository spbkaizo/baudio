#ifndef MAIN_H
#define MAIN_H

// Define USE_LCD to use the LCD instead of LEDs on PB0 and PB1
//#define USE_LCD  // Uncomment to use the LCD instead of LEDs on PB0 and PB1

// LED modes
#define MODE_KNIGHT_RIDER 0
#define MODE_VU_METER 1
#define MODE_BINARY 2
#define MODE_SPECTRUM_ANALYZER 3
#define MODE_MIDDLE_OUT 4  // New mode for middle out

#ifdef USE_LCD
// LCD modes
#define MODE_LCD_MESSAGE 0
#define MODE_LCD_COUNTER 1
#define MODE_LCD_SENSOR 2
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

extern volatile uint8_t mode;
extern uint16_t baseline_left;
extern uint16_t baseline_right;

#endif // MAIN_H
