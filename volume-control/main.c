#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define AD5242_ADDR 0x2C  // Adjust based on A0, A1 pins setup
#define VOLUME_UP_PIN PIN2 // PA1 for Volume Down
#define VOLUME_DOWN_PIN PIN1 // PA2 for Volume Up
#define LED_PIN PIN3        // PA3 for LED


#define DOT_DURATION 100
#define DASH_DURATION 300
#define ELEMENT_PAUSE 100
#define LETTER_PAUSE 300
#define WORD_PAUSE 700

// we have 22 possible values, which is twice as good as being able to go to 11!
uint8_t log_volume_map[] = {
    0, 1, 2, 3, 6, 10, 16, 23, 32, 43, 56, 70, 85, 100, 120, 140, 160, 180, 200, 220, 240, 255
};
int current_index = 6;  // Current index in the logarithmic map



void i2c_init(void) {
    TWI0.MBAUD = (uint8_t)((F_CPU - 2 * 100000UL) / (2 * 100000UL));
    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void i2c_start(void) {
    TWI0.MADDR = AD5242_ADDR << 1;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

void i2c_stop(void) {
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
}

void i2c_write(uint8_t data) {
    TWI0.MDATA = data;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

void ad5242_set_wiper(uint8_t channel, uint8_t value) {
    i2c_start();
    i2c_write((channel << 7) | 0x00);
    i2c_write(value);
    i2c_stop();
}

void gpio_init(void) {
    // Initialize GPIO for buttons as inputs and enable internal pull-up resistors
    PORTA.DIR &= ~((1 << VOLUME_UP_PIN) | (1 << VOLUME_DOWN_PIN));  // Buttons as inputs
    PORTA.PIN1CTRL |= PORT_PULLUPEN_bm; // Enable pull-up for Volume Up
    PORTA.PIN2CTRL |= PORT_PULLUPEN_bm; // Enable pull-up for Volume Down
    PORTA.DIR |= (1 << LED_PIN);  // LED as output
}

bool debounce(uint8_t pin) {
    if (!(PORTA.IN & (1 << pin))) { // Check if button is pressed (low due to pull to ground)
        _delay_ms(50); // Debounce delay
        if (!(PORTA.IN & (1 << pin))) {
            return true;
        }
    }
    return false;
}

void morse_signal(char* code) {
    PORTA.DIR |= (1 << LED_PIN); // Set LED pin as output

    for (int i = 0; code[i] != '\0'; i++) {
        switch (code[i]) {
            case '.':
                PORTA.OUT |= (1 << LED_PIN);
                _delay_ms(DOT_DURATION);
                PORTA.OUT &= ~(1 << LED_PIN);
                _delay_ms(ELEMENT_PAUSE);
                break;
            case '-':
                PORTA.OUT |= (1 << LED_PIN);
                _delay_ms(DASH_DURATION);
                PORTA.OUT &= ~(1 << LED_PIN);
                _delay_ms(ELEMENT_PAUSE);
                break;
            case ' ':
                _delay_ms(LETTER_PAUSE);
                break;
            case '/':
                _delay_ms(WORD_PAUSE);
                break;
        }
    }
}


int main(void) {
    i2c_init();
    gpio_init();

    ad5242_set_wiper(0, log_volume_map[current_index]); // Initial volume
    ad5242_set_wiper(1, log_volume_map[current_index]);

    morse_signal("-... .- ..- -.. .. --- /");

    while (1) {
        if (debounce(VOLUME_UP_PIN)) {
            PORTA.OUT |= (1 << LED_PIN); // Turn on LED
            if (current_index < sizeof(log_volume_map) / sizeof(log_volume_map[0]) - 1) {
                current_index++;
                ad5242_set_wiper(0, log_volume_map[current_index]);
                ad5242_set_wiper(1, log_volume_map[current_index]);
            }
        }
        if (debounce(VOLUME_DOWN_PIN)) {
            PORTA.OUT |= (1 << LED_PIN); // Turn on LED
            if (current_index > 0) {
                current_index--;
                ad5242_set_wiper(0, log_volume_map[current_index]);
                ad5242_set_wiper(1, log_volume_map[current_index]);
            }
        }
        _delay_ms(100); // Add a small delay to reduce bouncing effect further
        PORTA.OUT &= ~(1 << LED_PIN); // Turn off LED
    }
}
