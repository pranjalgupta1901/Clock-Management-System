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
 * @file    oled_driver.h
 * @brief   This file has function prototypes for oled display, its write and clear operation.
 *			Modified accordingly by taking reference from https://github.com/sdp8483/MSP430G2_SSD1306_OLED/blob/master/MSP430G2_SSD1306/ssd1306.h
 * @author  Pranjal Gupta
 * @date    10/01/2023
 *
 */


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include "stdint.h"

void oled_init(void);

#define OLED_ADDRESS            	  0x3C

#define OLED_LCDWIDTH                128
#define OLED_LCDHEIGHT   			64
#define OVERALL_SIZE (OLED_LCDHEIGHT * OLED_LCDHEIGHT)/SIZE_OF_BYTE
#define SIZE_OF_BYTE 8

#define OLED_PAGEADDR				0x22
#define OLED_SETCONTRAST             0x81
#define OLED_DISPLAYALLON_RESUME     0xA4
#define OLED_DISPLAYALLON            0xA5
#define OLED_NORMALDISPLAY           0xA6
#define OLED_INVERTDISPLAY           0xA7
#define OLED_DISPLAYOFF              0xAE
#define OLED_DISPLAYON               0xAF

#define OLED_SETDISPLAYOFFSET        0xD3
#define OLED_SETCOMPINS              0xDA

#define OLED_SETVCOMDETECT           0xDB

#define OLED_SETDISPLAYCLOCKDIV      0xD5
#define OLED_SETPRECHARGE            0xD9

#define OLED_SETMULTIPLEX            0xA8


#define OLED_SETSTARTLINE            0x40

#define OLED_MEMORYMODE              0x20
#define OLED_COLUMNADDR              0x21
#define OLED_PAGEADDR                0x22

#define OLED_COMSCANINC              0xC0
#define OLED_COMSCANDEC              0xC8

#define OLED_SEGREMAP                0xA0

#define OLED_CHARGEPUMP              0x8D

#define SSD1306_EXTERNALVCC             0x1

void oled_clearDisplay(void);
void oled_clear_page(uint8_t y);
void oled_printstring(char *string, uint8_t x, uint8_t y);

#endif /* OLED_DRIVER_H_ */
