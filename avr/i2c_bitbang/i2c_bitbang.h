//
// Created by pimi on 15.03.18.
//

#ifndef OLED_I2C_BITBANG_H
#define OLED_I2C_BITBANG_H

// ============================================================================

//// -----(+)--------------->	// Vcc,	Pin 1 on SSD1306 Board
//// -----(-)--------------->	// GND,	Pin 2 on SSD1306 Board
#ifndef SSD1306_SCL
#define SSD1306_SCL		PB2	// SCL,	Pin 3 on SSD1306 Board
#endif
#ifndef SSD1306_SDA
#define SSD1306_SDA		PB0	// SDA,	Pin 4 on SSD1306 Board
#endif
#ifndef SSD1306_SA
#define SSD1306_SA		0x78	// Slave address
#endif


void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(uint8_t byte);
void ssd1306_send_command_start(void);
void ssd1306_send_command_stop(void);
//void ssd1306_send_command(uint8_t command);
void ssd1306_send_command(const uint8_t command[], uint8_t anz);
void ssd1306_send_data(const uint8_t data[], uint8_t anz);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);

/*
void ssd1306_send_data(uint8_t byte)
{
	ssd1306_send_data_start();
	ssd1306_send_byte(byte);
	ssd1306_send_data_stop();
}
*/
#endif //OLED_I2C_BITBANG_H
