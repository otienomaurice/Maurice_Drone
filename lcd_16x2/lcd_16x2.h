

#include  "inttypes.h"
#define LCD_I2C_ADDR 0x27  // Default I2C address of LCD backpack
#define LCD_COLS 16
#define LCD_ROWS 2
#ifndef LCD_I2C_H
#define LCD_I2C_H
// I2C pins (adjust as needed)
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

void lcd_init();
void lcd_clear();
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_write_char(char c);
void lcd_write_string(const char *str);
void lcd_write_line(uint8_t line, const char *text);

#endif