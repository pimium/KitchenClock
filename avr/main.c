/**
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2014-08-08
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

// #define F_CPU 1000000UL
// NOTE: The F_CPU (CPU frequency) should not be defined in the source code.
//       It should be defined in either (1) Makefile; or (2) in the IDE. 

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                ATtiny
//               25/45/85
//              +----------+   (-)-------
//      (RST)---+ PB5  Vcc +---(+)-------
// --[OWOWOD]---+ PB3  PB2 +---[TWI/SCL]-
//           ---+ PB4  PB1 +---
// -------(-)---+ GND  PB0 +---[TWI/SDA]-
//              +----------+
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// -----(+)--------------->	// Vcc,	Pin 1 on SSD1306 Board
// -----(-)--------------->	// GND,	Pin 2 on SSD1306 Board
#define SSD1306_SCL        PB2    // SCL,	Pin 7A on SSD1306 Board
#define SSD1306_SDA        PB0    // SDA,	Pin 5 on SSD1306 Board

#define SSD1306_SA        0x78    // Slave address

// ----------------------------------------------------------------------------

#include "ssd1306xled.h"
//#include "print_3x7.h"
#include "print_5x7.h"
#include "print_10x14.h"
#include "num2str.h"
//#include "cpufreq.h"

// ----------------------------------------------------------------------------

#define IDLE 0
#define ARMED 1
#define COUNTING 2
#define WAIT 3

#include <util/delay.h>

volatile uint8_t sec = 50, display_time = 1;
uint8_t display_date = 1, days = 8, month = 4;
uint16_t year = 2018;
volatile uint8_t hour = 23, min = 59;
volatile uint16_t counter_interrupt = 0;
volatile uint8_t state = IDLE;
// ----------------------------------------------------------------------------

static char daytab[2][13] =
        {
                {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
        };

static char weekdaytab[7][3] =
        {
                {'M', 'O', 0},
                {'D', 'I', 0},
                {'M', 'I', 0},
                {'D', 'O', 0},
                {'F', 'R', 0},
                {'S', 'A', 0},
                {'S', 'O', 0}
        };

char ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS + 1];

#if 1

ISR(PCINT0_vect) {
//    if((PIND & (1 << MY_SWITCH)) == 0){}
    _delay_ms(10);
    if ((PINB & (1 << PIN4)) == 0) {
        hour++;
    } else if ((PINB & (1 << PIN3)) == 0) {
        min++;
    }
}

ISR(TIMER1_OVF_vect) {
    if (counter_interrupt == 15625) {
        counter_interrupt = 0;
        sec++;
        if (sec > 59) {
            min++;
            sec = 0;
            if (min > 59) {
                hour++;
                min = 0;
                if (hour > 23) {
                    hour = 0;
                    display_date = 1;
                    days++;
                }
            }
        }
        display_time = 1;
    }
    counter_interrupt++;
}


static inline void initTimer1(void) {
//    TCCR1 |= (0 << CTC1);  // clear timer on compare match
    TCCR1 |= (0 << CS13) | (0 << CS12) | (1 << CS11) | (0 << CS10);
    OCR1C = 0xff; // compare match value
    TIMSK |= (1 << TOIE1); // enable overflow interrupt
}

#endif

static inline void init_PCINT_Interrupt(void) {
    GIMSK |= (1 << PCIE);   // pin change interrupt enable
    PCMSK |= (1 << PCINT4) | (1 << PCINT3) | (1 << PCINT1); // pin change interrupt enabled for PCINT4

    DDRB &= ~((1 << PB4) | (1 << PB3) | (1 << PB1));        // see comment #1
    PORTB |= (1 << PB4) | (1 << PB3) | (1 << PB1); // enable pullup on pushbutton output and PCINT4 interrupt

//    sei();                  // enable interrupts
}

uint8_t is_leap_year(uint16_t year) {
    uint8_t leap;
    if (year % 4 == 0)
        leap = 1;
    else
        leap = 0;
    if (year % 100 == 0 && year % 400 != 0)
        leap = 0;
    return leap;
}

// https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
uint8_t daystotal(uint16_t y, uint8_t m, uint8_t d) {
    uint16_t daystotal = d;
    for (uint16_t year = 1; year <= y; year++) {
        int max_month = (year < y ? 12 : m - 1);
        int leap = is_leap_year(year);
        for (int month = 1; month <= max_month; month++) {
            daystotal += daytab[leap][month];
        }
    }
    return (uint8_t) (daystotal % 7);
}

int main(void) {
    uint8_t week_days = daystotal(year, month, days);

    ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
    // ---- Initialization ----

    // Small delay is necessary if ssd1306_init is the first operation in the application.
    _delay_ms(40);
    ssd1306_oled *f10x14 = font10x14_init();
    _delay_ms(40);
    ssd1306_oled *f5x7 = font5x7_init();

    _delay_ms(100);

    f5x7->ssd1306_init();
    _delay_ms(100);
//    f10x14->ssd1306_init();
    f5x7->ssd1306_clear();

    init_PCINT_Interrupt();
    initTimer1();
    sei();



//    f10x14->put_char(0,3,'I');
//    f10x14->put_char(12,3,'A');
//    f10x14->put_char(24,3,'C');
//    f10x14->put_char(38,3,'8');

    // ---- Main Loop ----

#if 1
    for (;;) {
        if (display_date) {
//            days++;
            if (days == daytab[is_leap_year(year)][month]) {
                days = 0;
                month++;
                if (month == 13) {
                    month = 1;
                    year++;
                }

            }

//            usint2decascii(, ssd1306_numdec_buffer);
            f5x7->put_string(30, 0, weekdaytab[(week_days++) % 7]);

            usint2decascii(days, ssd1306_numdec_buffer);

            f5x7->put_string(50, 0, ssd1306_numdec_buffer);
            f5x7->put_char(62, 0, '.');

            usint2decascii(month, ssd1306_numdec_buffer);
            f5x7->put_string(68, 0, ssd1306_numdec_buffer);
            f5x7->put_char(80, 0, '.');

            usint2decascii(year % 100, ssd1306_numdec_buffer);
            f5x7->put_string(86, 0, ssd1306_numdec_buffer);

            display_date = 0;
        }

        if (display_time) {
            f10x14->put_char(44, 4, ' ');
            f10x14->put_char(80, 4, ' ');
            usint2decascii(hour, ssd1306_numdec_buffer);
            f10x14->put_string(20, 4, ssd1306_numdec_buffer);
            usint2decascii(min, ssd1306_numdec_buffer);
            f10x14->put_string(56, 4, ssd1306_numdec_buffer);
            usint2decascii(sec, ssd1306_numdec_buffer);
            f10x14->put_string(92, 4, ssd1306_numdec_buffer);
            f10x14->put_char(44, 4, ':');
            f10x14->put_char(80, 4, ':');
            display_time = 0;
        }
    }
#endif

    return 0;
}

// ============================================================================
