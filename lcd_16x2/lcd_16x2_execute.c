#include "pico/stdlib.h"
#include "lcd_16x2/lcd_16x2.h"
#include "global/global.h"
#include "stdlib.h"
#include "stdio.h"
void lcd_execute() {
    char line1[17];
    char line2[17];

    snprintf(line1, sizeof(line1), "X_1: %-12lu", horizontal1);
    snprintf(line2, sizeof(line2), "Y_1: %-12lu", vertical1);

    lcd_write_line(0, line1);
    lcd_write_line(1, line2);
}