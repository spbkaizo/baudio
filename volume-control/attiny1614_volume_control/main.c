#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#define LPOT_CS_bm (1 << 4)  // Use the correct bit mask for PA4
#define RPOT_CS_bm (1 << 3)  // Use the correct bit mask for PA3
#define SCK_PIN_bm (1 << 1)  // Use the correct bit mask for PA1
#define SI_PIN_bm  (1 << 2)  // Use the correct bit mask for PA2
#define VOL_UP_bm  (1 << 5)  // Use the correct bit mask for PA5
#define VOL_DOWN_bm (1 << 6) // Use the correct bit mask for PA6
#define LED_PIN_bm (1 << 7)  // LED pin PA7

volatile uint8_t volume; // Current volume, marked volatile as it's accessed in ISR and main loop
uint8_t EEMEM storedVolume = 127; // EEPROM variable to store volume level
uint8_t lastStoredVolume; // To track the last stored volume value for comparison
unsigned long writeDelayCounter = 0; // Counter to manage write delay

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
		_delay_us(10);
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
	if (!(PORTA.IN & VOL_UP_bm) && volume < 255) {
		volume++;
		pot_write(LPOT_CS_bm, volume);
		pot_write(RPOT_CS_bm, volume);
		} else if (!(PORTA.IN & VOL_DOWN_bm) && volume > 0) {
		volume--;
		pot_write(LPOT_CS_bm, volume);
		pot_write(RPOT_CS_bm, volume);
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
	lastStoredVolume = volume; // Initialize lastStoredVolume

	while (1) {
		// Toggle LED state
		PORTA.OUTTGL = LED_PIN_bm;
		_delay_ms(1000); // Approximate 1s delay

		// Increment write delay counter
		writeDelayCounter++;

		// Check if volume has changed and if sufficient delay has passed (approximating with counter)
		if ((volume != lastStoredVolume) && (writeDelayCounter >= 120)) { // 120 cycles * 1s delay = 2 minute
			eeprom_write_byte(&storedVolume, volume);
			lastStoredVolume = volume;
			writeDelayCounter = 0; // Reset counter after writing
		}
	}
}
