#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h> // Include the EEPROM library

#define LPOT_CS_bm (1 << 4)  // Use the correct bit mask for PA4
#define RPOT_CS_bm (1 << 3)  // Use the correct bit mask for PA3
#define SCK_PIN_bm (1 << 1)  // Use the correct bit mask for PA1
#define SI_PIN_bm  (1 << 2)  // Use the correct bit mask for PA2
#define VOL_UP_bm  (1 << 5)  // Use the correct bit mask for PA5
#define VOL_DOWN_bm (1 << 6) // Use the correct bit mask for PA6
#define LED_PIN_bm (1 << 7)  // LED pin PA7

volatile uint8_t volume; // Mid-volume, marked volatile as it's accessed in ISR and main loop
uint8_t EEMEM storedVolume = 127; // EEPROM variable to store volume level

void spi_init() {
	PORTA.DIRSET = SCK_PIN_bm | SI_PIN_bm | LPOT_CS_bm | RPOT_CS_bm | LED_PIN_bm; // Set as output
	// Configure interrupt for volume buttons
	PORTA.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc; // Enable pull-up, interrupt on falling edge for VOL_UP
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc; // Enable pull-up, interrupt on falling edge for VOL_DOWN
	sei(); // Enable global interrupts
}

void spi_send_byte(uint8_t byte) {
	for (int i = 7; i >= 0; i--) {
		if (byte & (1 << i)) {
			PORTA.OUT |= SI_PIN_bm;
			} else {
			PORTA.OUT &= ~SI_PIN_bm;
		}
		PORTA.OUT |= SCK_PIN_bm;
		_delay_us(10); // Adjust based on SPI requirements
		PORTA.OUT &= ~SCK_PIN_bm;
	}
}

void pot_write(uint8_t cs_bm, uint8_t value) {
	PORTA.OUT &= ~cs_bm; // Select potentiometer
	spi_send_byte(0x11); // Command to write
	spi_send_byte(value); // Value
	PORTA.OUT |= cs_bm; // Deselect potentiometer
}

ISR(PORTA_PORT_vect) {
	// Immediate volume adjustment
	if (!(PORTA.IN & VOL_UP_bm) && volume < 255) {
		volume++;
		pot_write(LPOT_CS_bm, volume);
		pot_write(RPOT_CS_bm, volume);
		eeprom_write_byte(&storedVolume, volume); // Update EEPROM
		} else if (!(PORTA.IN & VOL_DOWN_bm) && volume > 0) {
		volume--;
		pot_write(LPOT_CS_bm, volume);
		pot_write(RPOT_CS_bm, volume);
		eeprom_write_byte(&storedVolume, volume); // Update EEPROM
	}
	// Debounce (simple method)
	_delay_ms(50);
	// Clear interrupt flags for PORTA to prevent repeated triggers
	PORTA.INTFLAGS = VOL_UP_bm | VOL_DOWN_bm;
}

int main(void) {
	spi_init();

	// Load the volume level from EEPROM at startup
	volume = eeprom_read_byte(&storedVolume);

	while (1) {
		// Toggle LED state
		PORTA.OUTTGL = LED_PIN_bm;
		_delay_ms(500); // Wait for 500 milliseconds
	}
}
