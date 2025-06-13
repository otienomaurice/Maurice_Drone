

#include "stdint.h"
#include "stdbool.h"
#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_PRESSED_MS 1000

struct Button
{
    bool pressed;
    bool cooldown;
    uint32_t x, y, w, h;
    uint32_t time;
    char c;
    // unsigned short unpressed_color, pressed_color;
    // void (*on_press)();
};

struct Button createButton(int16_t x, int16_t y, int16_t w, int16_t h, char c);

bool getPressed(struct Button *button);

void tick_button(struct Button *button, int16_t x, int16_t y, bool isTouched);

#endif
