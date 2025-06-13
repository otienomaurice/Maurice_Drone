#include "button.h"
#include "stdint.h"
#include "stdbool.h"

struct Button createButton(int16_t x, int16_t y, int16_t w, int16_t h, char c)
{
    struct Button button;
    button.pressed = false;
    button.cooldown = false;
    button.time = 0;
    button.x = x;
    button.y = y;
    button.w = w;
    button.h = h;
    button.c = c;

    return button;
}

bool getPressed(struct Button *button)
{
    return button->pressed;
}
char getButtonChar(struct Button *button){
    return button->c;
}
bool is_in_area(struct Button *button, int16_t x, int16_t y)
{
    return (x > button->x && x < button->x + button->w && y > button->y && y < button->y + button->h);
}

void tick_button(struct Button *button, int16_t x, int16_t y, bool isTouched)
{

    if (button->pressed)
    {
        button->pressed = false;
        button->cooldown = true;
        return;
    }

    if (button->cooldown && (!(isTouched && is_in_area(button, x, y))))
    {
        button->cooldown = false;
        return;
    }

    if (isTouched && is_in_area(button, x, y))
    {
        button->pressed = true;
        return;
    }
}
