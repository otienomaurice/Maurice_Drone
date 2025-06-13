#include "lcd_16x2.h"
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "stdbool.h"
static void lcd_send_cmd(uint8_t cmd);
static void lcd_send_data(uint8_t data);
static void lcd_send_byte(uint8_t byte, bool is_data);
static void lcd_pulse_enable(uint8_t data);

static i2c_inst_t *i2c_port = i2c1;

void lcd_init() {
    i2c_init(i2c_port, 100 * 1000); // 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    sleep_ms(50);

    lcd_send_cmd(0x33); // Init
    lcd_send_cmd(0x32); // Set 4-bit mode
    lcd_send_cmd(0x28); // 2 lines, 5x8 font
    lcd_send_cmd(0x0C); // Display on, cursor off
    lcd_send_cmd(0x06); // Entry mode
    lcd_send_cmd(0x01); // Clear display
    sleep_ms(5);
}

void lcd_clear() {
    lcd_send_cmd(0x01);
    sleep_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40};
    lcd_send_cmd(0x80 | (col + row_offsets[row % LCD_ROWS]));
}

void lcd_write_char(char c) {
    lcd_send_data(c);
}

void lcd_write_string(const char *str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}

void lcd_write_line(uint8_t line, const char *text) {
    lcd_set_cursor(0, line);
    for (int i = 0; i < LCD_COLS; i++) {
        if (text[i]) {
            lcd_write_char(text[i]);
        } else {
            lcd_write_char(' ');
        }
    }
}

// ---------- Internal Helpers ----------

#define LCD_BACKLIGHT 0x08
#define ENABLE_BIT    0x04
#define RS_BIT        0x01

static void lcd_send_cmd(uint8_t cmd) {
    lcd_send_byte(cmd, false);
}

static void lcd_send_data(uint8_t data) {
    lcd_send_byte(data, true);
}

static void lcd_send_byte(uint8_t byte, bool is_data) {
    uint8_t high = (byte & 0xF0);
    uint8_t low  = ((byte << 4) & 0xF0);

    lcd_pulse_enable(high | (is_data ? RS_BIT : 0x00));
    lcd_pulse_enable(low  | (is_data ? RS_BIT : 0x00));
}

static void lcd_pulse_enable(uint8_t data) {
    uint8_t buf[1];

    buf[0] = data | LCD_BACKLIGHT | ENABLE_BIT;
    i2c_write_blocking(i2c_port, LCD_I2C_ADDR, buf, 1, false);
    sleep_us(1);

    buf[0] = data | LCD_BACKLIGHT;
    i2c_write_blocking(i2c_port, LCD_I2C_ADDR, buf, 1, false);
    sleep_us(50);
}