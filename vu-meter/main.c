#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "adc.h"
#include "led_control.h"
#include "knight.h"
#include "binary.h"
#include "vu2.h"
#include "spectrum.h"
#include "timer.h"
#include "main.h"

#ifdef USE_LCD
#include "lcd.h"  // Include the LCD header if using the LCD
LCD_I2C lcd;  // Create an instance of the LCD_I2C struct
#endif

#define BUTTON_PIN PIN3_bp
#define DEBOUNCE_TIME 50  // Debounce time in milliseconds

volatile uint8_t mode = MODE_VU_METER;
volatile uint8_t last_state = 1;  // Initialize to 1 since the pull-up resistor keeps the pin high when not pressed

uint16_t baseline_left = 0;
uint16_t baseline_right = 0;

ISR(PORTB_PORT_vect) {
    static uint32_t last_debounce_time = 0;
    uint32_t current_time = millis();  // Function returning system uptime in milliseconds
    uint8_t current_state = PORTB.IN & (1 << BUTTON_PIN);

    if (current_time - last_debounce_time > DEBOUNCE_TIME) {
        if (current_state == 0) {  // Check if button is pressed (state is low)
            mode = (mode + 1) % 5;  // Update to include the new mode
            last_debounce_time = current_time;  // Update the last debounce time
        }
    }
    PORTB.INTFLAGS = (1 << BUTTON_PIN);  // Clear the interrupt flag
}

void init_clock() {
    // Enable the 20 MHz internal oscillator
    CPU_CCP = CCP_IOREG_gc;  // Unlock protected register
    CLKCTRL.OSC20MCTRLA = CLKCTRL_RUNSTDBY_bm;  // Enable OSC20M to run in standby mode

    // Wait for the oscillator to stabilize
    while (!(CLKCTRL.MCLKSTATUS & CLKCTRL_OSC20MS_bm));

    // Set the main clock source to the 20 MHz internal oscillator
    CPU_CCP = CCP_IOREG_gc;  // Unlock protected register
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc;

    // Disable the prescaler
    CPU_CCP = CCP_IOREG_gc;  // Unlock protected register
    CLKCTRL.MCLKCTRLB = 0;
}

void init_ports() {
    // Configure PORTA and PORTB pins as output for LEDs
    PORTA.DIR |= (1 << PIN1_bp) | (1 << PIN2_bp) | (1 << PIN3_bp) | (1 << PIN4_bp) | (1 << PIN5_bp) | (1 << PIN6_bp);

#ifndef USE_LCD
    PORTB.DIR |= (1 << PIN0_bp) | (1 << PIN1_bp);  // Ensure PB0 and PB1 are set as output if not using LCD
#endif
    PORTB.DIR |= (1 << PIN7_bp);  // Ensure PB7 is set as output

    // Configure PB3 as input with pull-up enabled for button
    PORTB.DIR &= ~(1 << BUTTON_PIN);
    PORTB.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;  // Enable pull-up and trigger on both edges

    // Enable pull-up resistor on the reset pin (PA0) if used as a reset
    PORTA.PIN0CTRL = PORT_PULLUPEN_bm;
}

int main(void) {
    init_clock();  // Initialize clock to 20 MHz
    init_ports();
    timer_init();  // Initialize the timer
    setup_adc();

#ifdef USE_LCD
    lcd_init(&lcd, 0x20, 16, 2);  // Initialize the LCD with address 0x20, 16 columns, 2 rows
#endif

    // Light up all the LEDs
#ifndef USE_LCD
    for (uint8_t i = 0; i < 8; i++) {
        set_led_state(i, 1);
    }
#endif

    sei();  // Enable global interrupts
    _delay_ms(500);  // Delay to allow other system components to initialize

    calibrate_adc_baseline(ADC_CHANNEL_LEFT, &baseline_left);
    calibrate_adc_baseline(ADC_CHANNEL_RIGHT, &baseline_right);

    while(1) {
#ifdef USE_LCD
        switch (mode) {
            case MODE_LCD_MESSAGE:
                lcd_set_cursor(&lcd, 0, 0);
                lcd_print(&lcd, "Hello, World!");
                _delay_ms(1000);
                lcd_command(&lcd, LCD_CLEARDISPLAY);
                break;
            case MODE_LCD_COUNTER:
                lcd_set_cursor(&lcd, 0, 0);
                lcd_print(&lcd, "Counter:");
                for (uint8_t i = 0; i < 100; i++) {
                    char buffer[4];
                    itoa(i, buffer, 10);
                    lcd_set_cursor(&lcd, 8, 0);
                    lcd_print(&lcd, buffer);
                    _delay_ms(500);
                    if (mode != MODE_LCD_COUNTER) break;  // Exit if mode changes
                }
                lcd_command(&lcd, LCD_CLEARDISPLAY);
                break;
            case MODE_LCD_SENSOR:
                lcd_set_cursor(&lcd, 0, 0);
                lcd_print(&lcd, "Sensor:");
                uint16_t sensor_value = read_adc(ADC_CHANNEL_LEFT);
                char buffer[6];
                itoa(sensor_value, buffer, 10);
                lcd_set_cursor(&lcd, 8, 0);
                lcd_print(&lcd, buffer);
                _delay_ms(1000);
                lcd_command(&lcd, LCD_CLEARDISPLAY);
                break;
        }
#else
        switch (mode) {
            case MODE_KNIGHT_RIDER:
                knight_rider_effect();
                break;
            case MODE_VU_METER:
                volume_meter_task(ADC_CHANNEL_LEFT, ADC_CHANNEL_RIGHT);  // Update for new VU meter task
                break;
            case MODE_BINARY:
                binary_count();
                break;
            case MODE_SPECTRUM_ANALYZER:
                spectrum_update();
                break;
            case MODE_MIDDLE_OUT:  // New case for middle out mode
                volume_meter_task(ADC_CHANNEL_LEFT, ADC_CHANNEL_RIGHT);  // Reuse the volume meter task for now
                break;
        }
#endif
    }
}
