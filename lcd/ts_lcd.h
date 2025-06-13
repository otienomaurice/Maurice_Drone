#include "stdint.h"

#define NORMAL_COLOR 0x07E0
#define PRESSED_COLOR 0x07FF

bool get_ts_lcd(uint16_t *px, uint16_t *py);

void ts_lcd_init();

void ts_test();

void render_button();

void render_text_number(int32_t *i);

void render_text(char *c);
