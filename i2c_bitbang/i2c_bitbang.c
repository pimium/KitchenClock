//
// Created by pimi on 15.03.18.
//
// Convenience definitions for PORTB
#include <avr/io.h>
#include <stdlib.h>

#include "i2c_bitbang.h"


#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)

void ssd1306_xfer_start(void) {
    DIGITAL_WRITE_HIGH(SSD1306_SCL);    // Set to HIGH
    DIGITAL_WRITE_HIGH(SSD1306_SDA);    // Set to HIGH
    DIGITAL_WRITE_LOW(SSD1306_SDA);        // Set to LOW
    DIGITAL_WRITE_LOW(SSD1306_SCL);        // Set to LOW
}

void ssd1306_xfer_stop(void) {
    DIGITAL_WRITE_LOW(SSD1306_SCL);        // Set to LOW
    DIGITAL_WRITE_LOW(SSD1306_SDA);        // Set to LOW
    DIGITAL_WRITE_HIGH(SSD1306_SCL);    // Set to HIGH
    DIGITAL_WRITE_HIGH(SSD1306_SDA);    // Set to HIGH
}

void ssd1306_send_byte(uint8_t byte) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if ((byte << i) & 0x80)
            DIGITAL_WRITE_HIGH(SSD1306_SDA);
        else
            DIGITAL_WRITE_LOW(SSD1306_SDA);

        DIGITAL_WRITE_HIGH(SSD1306_SCL);
        DIGITAL_WRITE_LOW(SSD1306_SCL);
    }
    DIGITAL_WRITE_HIGH(SSD1306_SDA);
    DIGITAL_WRITE_HIGH(SSD1306_SCL);
    DIGITAL_WRITE_LOW(SSD1306_SCL);
}

void ssd1306_send_command_start(void) {
    ssd1306_xfer_start();
    ssd1306_send_byte(SSD1306_SA);  // Slave address, SA0=0
    ssd1306_send_byte(0x00);    // write command
}

void ssd1306_send_command_stop(void) {
    ssd1306_xfer_stop();
}

void ssd1306_send_command(const uint8_t *command, uint8_t anz) {
    ssd1306_send_command_start();
    for (int i = 0; i < anz; ++i) {
        ssd1306_send_byte(*command++);                        // The second byte is used for the data.
    }

    ssd1306_send_data_stop();
}

void ssd1306_send_data(const uint8_t *data, uint8_t anz) {
    ssd1306_send_data_start();

    for (int i = 0; i < anz; ++i) {
        ssd1306_send_byte(*data++);                        // The second byte is used for the data.
    }

    ssd1306_send_data_stop();
}

void ssd1306_send_data_start(void) {
    ssd1306_xfer_start();
    ssd1306_send_byte(SSD1306_SA);
    ssd1306_send_byte(0x40);    //write data
}

void ssd1306_send_data_stop(void) {
    ssd1306_xfer_stop();
}
