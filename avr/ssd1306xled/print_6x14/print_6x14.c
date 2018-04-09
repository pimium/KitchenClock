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
#include "print_6x14.h"
#include "font3x7.h"


ssd1306_oled* font6x14_init(){
    font6x14 *f = malloc(sizeof(font6x14));
    font_init(&(f->super));
    f->super.put_string = ssd1306_string_font6x14;
    f->super.put_char = ssd1306_char_font6x14;
    return (ssd1306_oled*) f;
}

#define FONT_WIDTH 6

void ssd1306_char_font6x14(uint8_t pos_x, uint8_t pos_y, char ch) {
    uint8_t c = ch - 0x20;
    uint8_t message_up[FONT_WIDTH];
    uint8_t message_down[FONT_WIDTH];
    uint8_t message_up_[FONT_WIDTH];
    uint8_t message_down_[FONT_WIDTH];

    for (uint8_t i = 0; i < FONT_WIDTH/2; i++) {
        uint8_t buffer = pgm_read_byte(&ssd1306xled_font3x7[c][i]);
        uint8_t buffer_down = expand_14(buffer >> 6);
        uint8_t buffer_down_ = expand_14((buffer >> 4)&0x3);
        uint8_t buffer_up = expand_14((buffer >> 2)&0x3);
        uint8_t buffer_up_ = expand_14(buffer & 0x3);

        uint8_t value = expand(buffer_up);
        message_up[i*2] = value;
        message_up[i*2+1] = value;
        value = expand(buffer_up_);
        message_up_[i*2] = value;
        message_up_[i*2+1] = value;
        value = expand(buffer_down);
        message_down[i*2] = value;
        message_down[i*2+1] = value;
        value = expand(buffer_down_);
        message_down_[i*2] = value;
        message_down_[i*2+1] = value;

    }

    ssd1306_setpos(pos_x, pos_y);
    ssd1306_send_data(message_up_, sizeof(message_up_));
    ssd1306_setpos(pos_x, pos_y + 1);
    ssd1306_send_data(message_up, sizeof(message_up));
    ssd1306_setpos(pos_x, pos_y + 2);
    ssd1306_send_data(message_down_, sizeof(message_down_));
    ssd1306_setpos(pos_x, pos_y + 3);
    ssd1306_send_data(message_down, sizeof(message_down));

}

void ssd1306_string_font6x14(uint8_t posx, uint8_t posy, char *s) {
    while (*s) {
        if (posx + 6 > 128) {
            posx = 0;
            posy += 4;
        }
        ssd1306_char_font6x14(posx, posy, *s++);
        posx += 7;
    }
}

