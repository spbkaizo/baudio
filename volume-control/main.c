#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

/* 7-bit address. The five high bits are fixed at 01011 and the low two
   follow the AD0 and AD1 pins, so this assumes both are tied low. */
#define AD5242_ADDR 0x2C

/* Buttons are active low with internal pull-ups; the LED is active high.
   The comments here previously named the opposite pin for each button. */
#define VOLUME_UP_PIN PIN2_bp    // PA2
#define VOLUME_DOWN_PIN PIN1_bp  // PA1
#define LED_PIN PIN3_bp          // PA3


#define DEBOUNCE_MS 50    // Settling time before a button read is trusted
#define LED_BLINK_MS 40   // Acknowledgement flash on a volume step
#define REPEAT_MS 150     // Interval at which a held button repeats

#define DOT_DURATION 100
#define DASH_DURATION 300
#define ELEMENT_PAUSE 100
#define LETTER_PAUSE 300
#define WORD_PAUSE 700

// we have 22 possible values, which is twice as good as being able to go to 11!
uint8_t log_volume_map[] = {
    0, 1, 2, 3, 6, 10, 16, 23, 32, 43, 56, 70, 85, 100, 120, 140, 160, 180, 200, 220, 240, 255
};
#define VOLUME_STEPS (sizeof(log_volume_map) / sizeof(log_volume_map[0]))
uint8_t current_index = 6;  // Current index in the logarithmic map



#define I2C_SCL_HZ 100000UL
/* MBAUD per ATtiny1614 datasheet: F_CPU/(2*f_SCL) - 5 - (F_CPU*t_rise)/2.
   t_rise is treated as negligible for the short traces on this board. */
#define I2C_MBAUD ((uint8_t)((F_CPU / (2 * I2C_SCL_HZ)) - 5))

/* Bounds the WIF spin loops so a missing or wedged device cannot hang the
   firmware. Each unit is one iteration, not a calibrated time. */
#define I2C_TIMEOUT 10000

/* Wait for the write-interrupt flag. Returns false on timeout. */
static bool i2c_wait(void) {
    uint16_t guard = I2C_TIMEOUT;
    while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
        if (--guard == 0) {
            return false;
        }
    }
    return true;
}

void i2c_init(void) {
    TWI0.MBAUD = I2C_MBAUD;
    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void i2c_stop(void) {
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
}

/* Addresses the device for writing. Returns false if the bus timed out, the
   device did not acknowledge, or the controller flagged an error. */
static bool i2c_start(void) {
    TWI0.MADDR = AD5242_ADDR << 1;  /* 7-bit address, R/W = 0 */
    if (!i2c_wait()) {
        return false;
    }
    if (TWI0.MSTATUS & (TWI_ARBLOST_bm | TWI_BUSERR_bm)) {
        return false;
    }
    return !(TWI0.MSTATUS & TWI_RXACK_bm);
}

/* Sends one byte. Returns false if the byte was not acknowledged. */
static bool i2c_write(uint8_t data) {
    TWI0.MDATA = data;
    if (!i2c_wait()) {
        return false;
    }
    return !(TWI0.MSTATUS & TWI_RXACK_bm);
}

/* Writes one wiper. Always issues a STOP so a failed transfer releases the
   bus instead of leaving the controller owning it. Returns false on error. */
bool ad5242_set_wiper(uint8_t channel, uint8_t value) {
    bool ok = i2c_start()
           && i2c_write((channel << 7) | 0x00)  /* instruction: A/B select */
           && i2c_write(value);                 /* data: wiper position */
    i2c_stop();
    return ok;
}

void gpio_init(void) {
    // Initialize GPIO for buttons as inputs and enable internal pull-up resistors
    PORTA.DIR &= ~((1 << VOLUME_UP_PIN) | (1 << VOLUME_DOWN_PIN));  // Buttons as inputs
    PORTA.PIN1CTRL |= PORT_PULLUPEN_bm; // Enable pull-up for Volume Down (PA1)
    PORTA.PIN2CTRL |= PORT_PULLUPEN_bm; // Enable pull-up for Volume Up (PA2)
    PORTA.DIR |= (1 << LED_PIN);  // LED as output
}

/* True while the button on `pin` reads pressed, confirmed across a short
   settling delay. Active low: the internal pull-up holds the pin high until
   the button pulls it to ground. */
static bool button_pressed(uint8_t pin) {
    if (PORTA.IN & (1 << pin)) {
        return false;
    }
    _delay_ms(DEBOUNCE_MS);
    return !(PORTA.IN & (1 << pin));
}

/* Applies a wiper setting to both channels. Returns false if either write
   failed, so the caller can tell a real change from a dropped transfer. */
static bool apply_volume(uint8_t index) {
    bool ok = ad5242_set_wiper(0, log_volume_map[index]);
    return ad5242_set_wiper(1, log_volume_map[index]) && ok;
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

    apply_volume(current_index);  // Initial volume

    morse_signal("-... .- ..- -.. .. --- /");

    while (1) {
        bool up = button_pressed(VOLUME_UP_PIN);
        bool down = button_pressed(VOLUME_DOWN_PIN);
        bool changed = false;

        /* Both buttons held is ambiguous, so ignore it rather than letting
           whichever test runs first win. */
        /* Commit the new index only once the pot has acknowledged it, so a
           dropped transfer does not leave the stored index describing a
           wiper position the device never took. */
        if (up && !down && current_index < VOLUME_STEPS - 1) {
            changed = apply_volume(current_index + 1);
            if (changed) {
                current_index++;
            }
        } else if (down && !up && current_index > 0) {
            changed = apply_volume(current_index - 1);
            if (changed) {
                current_index--;
            }
        }

        /* Flash only on a step that actually happened. Lighting the LED on
           any press, as before, signalled success at the ends of the range
           where nothing moved, and when the pot failed to acknowledge. */
        if (changed) {
            PORTA.OUT |= (1 << LED_PIN);
            _delay_ms(LED_BLINK_MS);
            PORTA.OUT &= ~(1 << LED_PIN);
        }

        /* Held buttons repeat at this interval, which is the intended way to
           traverse the range; a single tap moves one step. */
        _delay_ms(REPEAT_MS);
    }
}

