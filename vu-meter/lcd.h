#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

typedef struct {
    uint8_t address;
    uint8_t cols;
    uint8_t rows;
    uint8_t backlightval;
    uint8_t displayfunction;
    uint8_t displaycontrol;
    uint8_t displaymode;
    uint8_t numlines;
    uint8_t currline;
} LCD_I2C;

void lcd_init(LCD_I2C *lcd, uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
void lcd_clear(LCD_I2C *lcd);
void lcd_home(LCD_I2C *lcd);
void lcd_set_cursor(LCD_I2C *lcd, uint8_t col, uint8_t row);
void lcd_no_display(LCD_I2C *lcd);
void lcd_display(LCD_I2C *lcd);
void lcd_no_cursor(LCD_I2C *lcd);
void lcd_cursor(LCD_I2C *lcd);
void lcd_no_blink(LCD_I2C *lcd);
void lcd_blink(LCD_I2C *lcd);
void lcd_scroll_display_left(LCD_I2C *lcd);
void lcd_scroll_display_right(LCD_I2C *lcd);
void lcd_left_to_right(LCD_I2C *lcd);
void lcd_right_to_left(LCD_I2C *lcd);
void lcd_autoscroll(LCD_I2C *lcd);
void lcd_no_autoscroll(LCD_I2C *lcd);
void lcd_create_char(LCD_I2C *lcd, uint8_t location, uint8_t charmap[]);
void lcd_backlight(LCD_I2C *lcd);
void lcd_no_backlight(LCD_I2C *lcd);
void lcd_command(LCD_I2C *lcd, uint8_t value);
void lcd_write(LCD_I2C *lcd, uint8_t value);
void lcd_print(LCD_I2C *lcd, const char *str);

#endif // LCD_H
