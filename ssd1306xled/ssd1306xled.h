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

#ifndef SSD1306XLED_H
#define SSD1306XLED_H

// ============================================================================

typedef struct oled_struct{
    void (*ssd1306_init) (void);
    void (*ssd1306_setpos) (uint8_t x, uint8_t y);
    void (*ssd1306_fill4) (uint8_t, uint8_t, uint8_t, uint8_t);
    void (*ssd1306_fill) (uint8_t p);
    void (*put_string) (uint8_t posx, uint8_t posy, char *s);
    void (*put_char) (uint8_t posx, uint8_t posy, char s);
} ssd1306_oled;

void font_init(ssd1306_oled* f);

#define ssd1306_clear() ssd1306_fill(0);
#define font_string(font, x, y, s) font->put_string(x, y, s)

#define ssd1306_char(c) ssd1306_char_font6x8(c)
#define ssd1306_string(s) ssd1306_string_font6x8(s)

// ----------------------------------------------------------------------------
uint8_t expand(uint8_t buffer);
uint8_t expand_14(uint8_t buffer);

void ssd1306_init(void);

void ssd1306_setpos(uint8_t x, uint8_t y);

void ssd1306_fill4(uint8_t, uint8_t, uint8_t, uint8_t);

void ssd1306_fill2(uint8_t p1, uint8_t p2);

void ssd1306_fill(uint8_t p);



// ============================================================================

#endif
