#include "lcd.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>

#define TWI_WRITE 0
#define TWI_READ 1

// Function declarations
void lcd_write4bits(LCD_I2C *lcd, uint8_t value);
void lcd_expander_write(LCD_I2C *lcd, uint8_t _data);
void lcd_pulse_enable(LCD_I2C *lcd, uint8_t _data);
void lcd_begin(LCD_I2C *lcd, uint8_t cols, uint8_t lines);

void i2c_init() {
    // Initialize I2C (TWI)
    TWI0.MBAUD = (uint8_t)((F_CPU / (2 * 100000)) - 5);  // Set baud rate for 100kHz
    TWI0.MCTRLA = TWI_ENABLE_bm;  // Enable TWI
}

void i2c_start(uint8_t address) {
    TWI0.MADDR = (address << 1) | TWI_WRITE;  // Set slave address and write mode
    while (!(TWI0.MSTATUS & TWI_WIF_bm));  // Wait for write interrupt flag
}

void i2c_stop() {
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;  // Send stop condition
}

void i2c_write(uint8_t data) {
    TWI0.MDATA = data;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));  // Wait for write interrupt flag
}

void lcd_command(LCD_I2C *lcd, uint8_t value) {
    i2c_start(lcd->address);
    i2c_write(0x00);  // Control byte
    i2c_write(value);  // Command byte
    i2c_stop();
}

void lcd_write(LCD_I2C *lcd, uint8_t value) {
    i2c_start(lcd->address);
    i2c_write(Rs);  // Control byte for data
    i2c_write(value);
    i2c_stop();
}

void lcd_send(LCD_I2C *lcd, uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    lcd_write4bits(lcd, (highnib) | mode);
    lcd_write4bits(lcd, (lownib) | mode);
}

void lcd_write4bits(LCD_I2C *lcd, uint8_t value) {
    lcd_expander_write(lcd, value);
    lcd_pulse_enable(lcd, value);
}

void lcd_expander_write(LCD_I2C *lcd, uint8_t _data) {
    i2c_start(lcd->address);
    i2c_write(_data | lcd->backlightval);
    i2c_stop();
}

void lcd_pulse_enable(LCD_I2C *lcd, uint8_t _data) {
    lcd_expander_write(lcd, _data | En);  // En high
    _delay_us(1);  // enable pulse must be >450ns
    lcd_expander_write(lcd, _data & ~En);  // En low
    _delay_us(50);  // commands need > 37us to settle
}

void lcd_init(LCD_I2C *lcd, uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows) {
    lcd->address = lcd_addr;
    lcd->cols = lcd_cols;
    lcd->rows = lcd_rows;
    lcd->backlightval = LCD_BACKLIGHT;

    i2c_init();
    _delay_ms(50);  // Wait for LCD to power up

    lcd->displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    lcd_begin(lcd, lcd->cols, lcd->rows);
}

void lcd_begin(LCD_I2C *lcd, uint8_t cols, uint8_t lines) {
    if (lines > 1) {
        lcd->displayfunction |= LCD_2LINE;
    }
    lcd->numlines = lines;
    lcd->currline = 0;

    // Initialization sequence from the HD44780 datasheet
    _delay_ms(50);
    lcd_write4bits(lcd, 0x03 << 4);
    _delay_us(4500);
    lcd_write4bits(lcd, 0x03 << 4);
    _delay_us(4500);
    lcd_write4bits(lcd, 0x03 << 4);
    _delay_us(150);
    lcd_write4bits(lcd, 0x02 << 4);

    // Function set
    lcd_command(lcd, LCD_FUNCTIONSET | lcd->displayfunction);

    // Display control
    lcd->displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_display(lcd);

    // Clear display
    lcd_clear(lcd);

    // Entry mode set
    lcd->displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcd_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);

    // Home the cursor
    lcd_home(lcd);
}

void lcd_clear(LCD_I2C *lcd) {
    lcd_command(lcd, LCD_CLEARDISPLAY);
    _delay_ms(2);  // This command takes a long time
}

void lcd_home(LCD_I2C *lcd) {
    lcd_command(lcd, LCD_RETURNHOME);
    _delay_ms(2);  // This command takes a long time
}

void lcd_set_cursor(LCD_I2C *lcd, uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= lcd->numlines) {
        row = lcd->numlines - 1;
    }
    lcd_command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_no_display(LCD_I2C *lcd) {
    lcd->displaycontrol &= ~LCD_DISPLAYON;
    lcd_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void lcd_display(LCD_I2C *lcd) {
    lcd->displaycontrol |= LCD_DISPLAYON;
    lcd_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void lcd_no_cursor(LCD_I2C *lcd) {
    lcd->displaycontrol &= ~LCD_CURSORON;
    lcd_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void lcd_cursor(LCD_I2C *lcd) {
    lcd->displaycontrol |= LCD_CURSORON;
    lcd_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void lcd_no_blink(LCD_I2C *lcd) {
    lcd->displaycontrol &= ~LCD_BLINKON;
    lcd_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void lcd_blink(LCD_I2C *lcd) {
    lcd->displaycontrol |= LCD_BLINKON;
    lcd_command(lcd, LCD_DISPLAYCONTROL | lcd->displaycontrol);
}

void lcd_scroll_display_left(LCD_I2C *lcd) {
    lcd_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scroll_display_right(LCD_I2C *lcd) {
    lcd_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_left_to_right(LCD_I2C *lcd) {
    lcd->displaymode |= LCD_ENTRYLEFT;
    lcd_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

void lcd_right_to_left(LCD_I2C *lcd) {
    lcd->displaymode &= ~LCD_ENTRYLEFT;
    lcd_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

void lcd_autoscroll(LCD_I2C *lcd) {
    lcd->displaymode |= LCD_ENTRYSHIFTINCREMENT;
    lcd_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

void lcd_no_autoscroll(LCD_I2C *lcd) {
    lcd->displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcd_command(lcd, LCD_ENTRYMODESET | lcd->displaymode);
}

void lcd_create_char(LCD_I2C *lcd, uint8_t location, uint8_t charmap[]) {
    location &= 0x7;  // We only have 8 locations 0-7
    lcd_command(lcd, LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        lcd_write(lcd, charmap[i]);
    }
}

void lcd_backlight(LCD_I2C *lcd) {
    lcd->backlightval = LCD_BACKLIGHT;
    lcd_expander_write(lcd, 0);
}

void lcd_no_backlight(LCD_I2C *lcd) {
    lcd->backlightval = LCD_NOBACKLIGHT;
    lcd_expander_write(lcd, 0);
}

void lcd_print(LCD_I2C *lcd, const char *str) {
    while (*str) {
        lcd_write(lcd, *str++);
    }
}
