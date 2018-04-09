//
// Created by pimi on 29.03.18.
//

#ifndef OLED_PRINT_10X14_H
#define OLED_PRINT_10X14_H

#include <avr/io.h>
#include "ssd1306xled.h"

typedef struct font10x17_st{
    ssd1306_oled super;
} font10x14;

ssd1306_oled* font10x14_init();

void ssd1306_char_font10x14(uint8_t pos_x, uint8_t pos_y, char ch);
void ssd1306_string_font10x14(uint8_t posx, uint8_t posy, char *s);

#endif //OLED_PRINT_10X14_H
