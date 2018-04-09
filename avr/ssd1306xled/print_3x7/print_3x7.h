//
// Created by pimi on 29.03.18.
//

#ifndef OLED_PRINT_3X7_H
#define OLED_PRINT_3X7_H

#include <avr/io.h>
#include "ssd1306xled.h"

typedef struct font3x7_st{
    ssd1306_oled super;
} font3x7;

ssd1306_oled* font3x7_init();

void ssd1306_char_font3x7(uint8_t pos_x, uint8_t pos_y, char ch);
void ssd1306_string_font3x7(uint8_t posx, uint8_t posy, char *s);

#endif //OLED_PRINT_3X7_H
