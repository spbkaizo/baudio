#include <avr/io.h>
#include <util/delay.h>

#define LPOT_CS_bm (1 << 4) // Use the correct bit mask for PA4
#define RPOT_CS_bm (1 << 3) // Use the correct bit mask for PA3
#define SCK_PIN_bm (1 << 1) // Use the correct bit mask for PA1
#define SI_PIN_bm  (1 << 2) // Use the correct bit mask for PA2
#define VOL_UP_bm  (1 << 5) // Use the correct bit mask for PA5
#define VOL_DOWN_bm (1 << 6) // Use the correct bit mask for PA6

void spi_init() {
    PORTA.DIRSET |= SCK_PIN_bm | SI_PIN_bm | LPOT_CS_bm | RPOT_CS_bm; // Set as output
    PORTA.DIRSET &= ~(VOL_UP_bm | VOL_DOWN_bm); // Set as input
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm; // Enable pull-up for VOL_UP
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm; // Enable pull-up for VOL_DOWN
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

int main(void) {
    spi_init();
    uint8_t volume = 127; // Mid-volume

    while(1) {
        if (!(PORTA.IN & VOL_UP_bm)) {
            if (volume < 255) volume++;
            pot_write(LPOT_CS_bm, volume);
            pot_write(RPOT_CS_bm, volume);
            _delay_ms(100); // Simple debounce
        }
        if (!(PORTA.IN & VOL_DOWN_bm)) {
            if (volume > 0) volume--;
            pot_write(LPOT_CS_bm, volume);
            pot_write(RPOT_CS_bm, volume);
            _delay_ms(100); // Simple debounce
        }
    }
}

