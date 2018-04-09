//
// Created by pimi on 29.03.18.
//

//
// Created by pimi on 29.03.18.
//
#include <stdlib.h>
#include <avr/io.h>

#include <avr/pgmspace.h>

#include "i2c_bitbang.h"
#include "print_3x7.h"
#include "font3x7.h"


ssd1306_oled* font3x7_init(){
    font3x7 *f = malloc(sizeof(font3x7));
    font_init(&(f->super));
    f->super.put_string = ssd1306_string_font3x7;
    f->super.put_char = ssd1306_char_font3x7;
    return (ssd1306_oled*) f;
}

void ssd1306_char_font3x7(uint8_t pos_x, uint8_t pos_y, char ch) {
    uint8_t c = ch - 0x20;
    uint8_t message_up[3];
    uint8_t message_down[3];

    for (uint8_t i = 0; i < sizeof(message_up); i++) {
        uint8_t buffer = pgm_read_byte(&ssd1306xled_font3x7[c][i]);
        uint8_t buffer_down = (buffer >> 4);
        uint8_t buffer_up = buffer & 0xF;

        message_up[i] = expand(buffer_up);
        message_down[i] = expand(buffer_down);
    }

    ssd1306_setpos(pos_x, pos_y);
    ssd1306_send_data(message_up, sizeof(message_up));

    ssd1306_setpos(pos_x, pos_y + 1);
    ssd1306_send_data(message_down, sizeof(message_down));
}

void ssd1306_string_font3x7(uint8_t posx, uint8_t posy, char *s) {
    while (*s) {
        if (posx + 6 > 128) {
            posx = 0;
            posy += 2;
        }
        ssd1306_char_font3x7(posx, posy, *s++);
        posx += 4;
    }
}

