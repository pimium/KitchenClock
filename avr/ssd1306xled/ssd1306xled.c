/**
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2014-08-12
 * @author: Neven Boyanov
 *
 * This is part of the Tinusaur/SSD1306xLED project.
 *
 * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */

// ============================================================================

#include <stdlib.h>
#include <avr/io.h>

#include <avr/pgmspace.h>

#include "ssd1306xled.h"
#include "i2c_bitbang.h"
#include "num2str.h"

// ----------------------------------------------------------------------------

// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358

const uint8_t ssd1306_init_sequence[] PROGMEM = {    // Initialization Sequence
        0xAE,            // Display OFF (sleep mode)
        0x20, 0x00,        // Set Memory Addressing Mode
        // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
        // 10=Page Addressing Mode (RESET); 11=Invalid
        0xB0,            // Set Page Start Address for Page Addressing Mode, 0-7
        0xC8,            // Set COM Output Scan Direction
        0x00,            // ---set low column address
        0x10,            // ---set high column address
        0x40,            // --set start line address
        0x81, 0x3F,        // Set contrast control register
        0xA1,            // Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
        0xA6,            // Set display_time mode. A6=Normal; A7=Inverse
        0xA8, 0x3F,        // Set multiplex ratio(1 to 64)
        0xA4,            // Output RAM to Display
        // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
        0xD3, 0x00,        // Set display_time offset. 00 = no offset
        0xD5,            // --set display_time clock divide ratio/oscillator frequency
        0xF0,            // --set divide ratio
        0xD9, 0x22,        // Set pre-charge period
        0xDA, 0x12,        // Set com pins hardware configuration
        0xDB,            // --set vcomh
        0x20,            // 0x20,0.77xVcc
        0x8D, 0x14,        // Set DC-DC enable
        0xAF            // Display ON in normal mode

};

char ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS + 1];

// ----------------------------------------------------------------------------

// These function should become separate library for handling I2C simplified output.

// ----------------------------------------------------------------------------
void font_init(ssd1306_oled *f){
//    ssd1306_oled *f = malloc(sizeof(ssd1306_oled));
    f->ssd1306_init = ssd1306_init;
    f->ssd1306_setpos = ssd1306_setpos;
    f->ssd1306_fill = ssd1306_fill;
    f->ssd1306_fill4 = ssd1306_fill4;
}

uint8_t expand(uint8_t buffer){

    return (((buffer >> 3) & 0x1) << 7) + (((buffer >> 2) & 0x1) << 5) + (((buffer >> 1) & 0x1) << 3) +
           (((buffer >> 0) & 0x1) << 1);
}

uint8_t expand_14(uint8_t buffer){

    return (((buffer >> 1) & 0x1) << 3) + (((buffer >> 1) & 0x1) << 2) +
           (((buffer >> 0) & 0x1) << 1) + (((buffer >> 0) & 0x1) << 0);
}

void ssd1306_init(void) {
    DDRB |= (1 << SSD1306_SDA);    // Set port as output
    DDRB |= (1 << SSD1306_SCL);    // Set port as output

    uint8_t message[sizeof(ssd1306_init_sequence)];
    for (uint8_t i = 0; i < sizeof(ssd1306_init_sequence); i++) {
        message[i] = pgm_read_byte(&ssd1306_init_sequence[i]);
    }
    ssd1306_send_command(message, sizeof(ssd1306_init_sequence));
}

void ssd1306_setpos(uint8_t x, uint8_t y) {
    uint8_t message[3] = {(0xb0 | (0x7 & y)), ((x & 0xf0) >> 4) | 0x10, (x & 0xF)};
    ssd1306_send_command(message, sizeof(message));
}

void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {

    uint8_t message[4] = {p1, p2, p3, p4};
    for (int j = 0; j < 8; ++j) {
        for (uint16_t i = 0; i < 128; i++) {
            ssd1306_setpos(i * 4, j);
            ssd1306_send_data(message, 4);
        }
    }
}

void ssd1306_fill2(uint8_t p1, uint8_t p2) {
    ssd1306_fill4(p1, p2, p1, p2);
}

void ssd1306_fill(uint8_t p) {
    ssd1306_fill4(p, p, p, p);
}

// ============================================================================
