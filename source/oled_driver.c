/*******************************************************************************
 * Copyright (C) 2023 by PRANJAL GUPTA
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. PRANJAL GUPTA and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    oled_driver.c
 * @brief   This file contains the functions for communication with oled display which is having ssd1306 display controller
 *			Modified accordingly by taking reference from https://github.com/sdp8483/MSP430G2_SSD1306_OLED/blob/master/MSP430G2_SSD1306/ssd1306.c
 *
 * @author  Pranjal Gupta
 * @date    12/10/2023
 *
 */
#include "oled_driver.h"
#include "i2c.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "font_5x7.h"

static void set_column_start_end_addr();
static void set_page_start_end_addr();
static void send_command(uint8_t command, uint8_t *data, uint8_t length);
static void oled_set_position(uint8_t x, uint8_t y);

#define MULTIPLEX_VALUE 0x3F
#define COMPINS_FOR_128X64 0x12
#define CONTRAST_RESET_VALUE 0x7F
#define PRECHARGE_VALUE 0xC2
#define CLOCK_DIVISION_RATIO 0x80
#define CHARGEPUMP_VALUE 0x14
#define COM_OUTPUT_VOLTAGE_SEL_VALUE 0x20
#define PIXEL_SIZE_IN_BYTES 5
#define COLUMN_REG_ADDR 0x21
#define PAGE_REG_ADDR 0x22
#define SSD1306_MAX_PAGE_ADDR 7
#define DATA_IDENTIFIER_BYTE 0x40
#define COMMAND_IDETIFIER_BYTE 0x00
#define DATA_IDENTIFIER_END_BYTE 0x00

/*
 * Description: Intilaises the oled display by sending commands specifies in the datasheet
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */

void oled_init(void) {

	uint8_t data;
	send_command(OLED_DISPLAYOFF, NULL, 0);
	data = MULTIPLEX_VALUE;
	send_command(OLED_SETMULTIPLEX, &data, sizeof(data));

	send_command(OLED_SETSTARTLINE | 0x0, NULL, 0);
	send_command(OLED_SEGREMAP | 0x01, NULL, 0);
	send_command(OLED_COMSCANDEC, NULL, 0);
	data = COMPINS_FOR_128X64;
	send_command(OLED_SETCOMPINS, &data, sizeof(data));
	data = CONTRAST_RESET_VALUE;
	send_command(OLED_SETCONTRAST, &data, sizeof(data)); // resetting the contrast
	send_command(OLED_DISPLAYALLON_RESUME, NULL, 0);
	data = PRECHARGE_VALUE;
	send_command(OLED_SETPRECHARGE, &data, sizeof(data));             // 0xd9
	data = COM_OUTPUT_VOLTAGE_SEL_VALUE;
	send_command(OLED_SETVCOMDETECT, &data, sizeof(data));            // 0xDB
	send_command(OLED_NORMALDISPLAY, NULL, 0); // pixel with 0 is non illuminated and 1 is illuminated	data = 0x8D;
	data = CLOCK_DIVISION_RATIO;
	send_command(OLED_SETDISPLAYCLOCKDIV, &data, sizeof(data)); // clock division ratio is 1
	data = CHARGEPUMP_VALUE;
	send_command(OLED_CHARGEPUMP, &data, sizeof(data));

	send_command(OLED_DISPLAYON, NULL, 0);

}

/*
 * Description: Sends the data to the slave collectively according to the length
 * Parameters:
 * 		uint8_t device addr the device address of the slave
 * 		uint8_t *data the data which is to be sent
 * 		int length  the length of the data to be sent
 * Returns:
 *   		None
 */

static void send_command(uint8_t command, uint8_t *data, uint8_t length) {
	uint8_t command_byte = COMMAND_IDETIFIER_BYTE; // continuos bit set to 0 indicating the following byte is the data od the command
	uint8_t data_to_send[length + 2];
	data_to_send[0] = command_byte;
	data_to_send[1] = command;
	if (length == 0) {
		i2c_data_transmit(OLED_ADDRESS, data_to_send, sizeof(data_to_send));
		return;
	}

	memcpy(data_to_send + 2, data, length);
	i2c_data_transmit(OLED_ADDRESS, data_to_send, sizeof(data_to_send));
}

/*
 * Description: set the cursor at a specific column and page
 * Parameters:
 * 		uint8_t column value
 * 		uint8_t Page value
 * Returns:
 *   		None
 */
static void oled_set_position(uint8_t x, uint8_t y) {

	uint8_t data[2];
	if (x > OLED_LCDWIDTH - 1)
		x = 0;                 // as column range is 0-127

	if (y > SSD1306_MAX_PAGE_ADDR)
		y = 0;                // page range is 0-7

	data[0] = x;
	data[1] = OLED_LCDWIDTH - 1;

	send_command(OLED_COLUMNADDR, data, sizeof(data));

	data[0] = y;
	data[1] = SSD1306_MAX_PAGE_ADDR;

	send_command(OLED_PAGEADDR, data, sizeof(data));

}

/*
 * Description: clears a specific page
 * Parameters:
 * 		uint8_t page number to be cleared
 * Returns:
 *   		None
 */

void oled_clear_page(uint8_t page) {
	uint8_t data[OLED_LCDWIDTH + 1];
	oled_set_position(0, page);
	data[0] = DATA_IDENTIFIER_BYTE;
	memset(data + 1, 0, OLED_LCDWIDTH);
	i2c_data_transmit(OLED_ADDRESS, data, sizeof(data));

}

/*
 * Description: Write a string or a char onto the display
 * Parameters:
 * 		char *  the string to be displayed
 * 		uint8_t the column value
 * 		uint8_t the page value
 * Returns:
 *   		None
 */
void oled_printstring(char *string, uint8_t x, uint8_t y) {

	if (string == NULL)
		return;

	oled_set_position(x, y);

	uint8_t buffer_to_send[SIZE_OF_BYTE - 1];

	while (*string != '\0') {
		if ((x + PIXEL_SIZE_IN_BYTES) > (OLED_LCDWIDTH - 1)) { // since i am using font 5x7 and if x+5 bytes goes beyond the boundaries and then g to the next page
			x = 0;
			y++;
			oled_set_position(x, y);
		}
		buffer_to_send[0] = DATA_IDENTIFIER_BYTE;     // identifier for data

		for (int i = 0; i < PIXEL_SIZE_IN_BYTES; i++) {
			buffer_to_send[i + 1] = font_5x7[*string - ' '][i];
		}

		buffer_to_send[6] = DATA_IDENTIFIER_END_BYTE; // marking the end of the buffer
		i2c_data_transmit(OLED_ADDRESS, buffer_to_send, sizeof(buffer_to_send));
		string++;
		x = x + PIXEL_SIZE_IN_BYTES + 1;
	}
}

/*
 * Description: clear the complete display by writing 0 onto every pixel space
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */

void oled_clearDisplay(void) {
	uint8_t data = COMMAND_IDETIFIER_BYTE;
	send_command(OLED_MEMORYMODE, &data, sizeof(data));
	set_column_start_end_addr();
	set_page_start_end_addr();

	for (uint8_t i = 0; i < SSD1306_MAX_PAGE_ADDR + 1; i++) {
		oled_clear_page(i);
	}

}

/*
 * Description: set the start and end column address
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */

static void set_column_start_end_addr() {

	uint8_t data[2];
	data[0] = 0x00;
	data[1] = OLED_LCDWIDTH - 1;

	send_command(COLUMN_REG_ADDR, data, sizeof(data));

}

/*
 * Description: set the start and end page address
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */
static void set_page_start_end_addr() {

	uint8_t data[2];
	data[0] = 0x00;
	data[1] = SSD1306_MAX_PAGE_ADDR;

	send_command(PAGE_REG_ADDR, data, sizeof(data));

}
