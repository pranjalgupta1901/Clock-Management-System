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
 * @file    DS3231.c
 * @brief   This file contains the functions for communication with DS3231 using I2C library.
 *
 * @author  Pranjal Gupta
 * @date    12/10/2023
 *
 */

#include "DS3231.h"
#include "i2c.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

static uint8_t decimal_to_bcd_conversion(uint8_t dec_num);
static int bcd_to_decimal_conversion(uint8_t bcd_num);


#define DS3231_CONTROL_STATUS 0x0F
#define DS3231_DAY_REF_ADDR 0x03
#define CURRENT_CENTURY_OFFSET 2000
#define CENTURY_FACTOR 100
#define CENTURY_BIT_IN_MONTH_REG 7
#define EXTRACTING_SEVEN_BIT_MASK 0x7F
#define DS3231_SEC_REG_ADDR 0


/*
 * Description: Converts decimal to bcd number
 *
 * Parameters:
 *    			uint8_t  a decimal number
 *
 * Returns:
 *   		uint8_t the converted bcd number
 */


static uint8_t decimal_to_bcd_conversion(uint8_t dec_num){

	return (uint8_t)((dec_num/10*16) + (dec_num % 10));

}


/*
 * Description: Converts bcd to decimal number
 *
 * Parameters:
 *    			uint8_t  a bcd number
 *
 * Returns:
 *   		int the converted decimal number
 */


static int bcd_to_decimal_conversion(uint8_t bcd_num){
	return (int)((bcd_num/16*10) + (bcd_num%16));
}


/*
 * Description: Sets the time in RTC DS3231 by converting them into bcd numbers first and then writing using i2c
 *
 * Parameters:
 *    		ds3231_time_t a pointer which contains the time data t be write in the RTC
 *
 * Returns:
 *   		NULL
 */


void ds3231_set_time(ds3231_time_t *time){

	uint8_t data_to_send[4];

	data_to_send[0] = DS3231_SEC_REG_ADDR;
	data_to_send[1] = decimal_to_bcd_conversion(time->sec);
	data_to_send[2] = decimal_to_bcd_conversion(time->min);
	data_to_send[3] = decimal_to_bcd_conversion(time->hour);


	i2c_data_transmit(DS3231_ADDRESS, data_to_send, sizeof(data_to_send));

}

/*
 * Description: It reads the time back using i2c from the RTC DS3231 and converts them back into decimal form
 *
 * Parameters:
 *    		ds3231_time_t a pointer which contains the read data from the RTC
 *
 * Returns:
 *   		NULL
 */


void ds3231_read_time(ds3231_time_t *time){

	i2c_read_bytes(DS3231_ADDRESS, DS3231_SEC_REG_ADDR ,(uint8_t*)time, sizeof(ds3231_time_t));

	time->sec = bcd_to_decimal_conversion(time->sec);
	time->min = bcd_to_decimal_conversion(time->min);
	time->hour = bcd_to_decimal_conversion(time->hour);

}



/*
 * Description: Sets the date in RTC DS3231 by converting them into bcd numbers first and then writing using i2c
 *
 * Parameters:
 *    		ds3231_date_t a pointer which contains the date to be written in the RTC
 *
 * Returns:
 *   		NULL
 */
void ds3231_set_date(ds3231_date_t *date){

	uint8_t data_to_send[5];
	uint8_t century = (date->year / CENTURY_FACTOR) % 20; // extracting the century from the current year
	date->month = ((date->month & EXTRACTING_SEVEN_BIT_MASK) | (century << CENTURY_BIT_IN_MONTH_REG));   // setting the century bit in month register
	uint8_t year = (date->year % CENTURY_FACTOR);        // Extracting the current year

	data_to_send[0] = DS3231_DAY_REF_ADDR;
	data_to_send[1] = decimal_to_bcd_conversion(date->dow);
	data_to_send[2] = decimal_to_bcd_conversion(date->date);
	data_to_send[3] = decimal_to_bcd_conversion(date->month);
	data_to_send[4] = decimal_to_bcd_conversion(year);


	i2c_data_transmit(DS3231_ADDRESS, data_to_send, sizeof(data_to_send));
}

/*
 * Description: It reads the date back using i2c from the RTC DS3231 and converts them back into decimal form
 *
 * Parameters:
 *    		ds3231_date_t a pointer which contains the read data from the RTC
 *
 * Returns:
 *   		NULL
 */
void ds3231_read_date(ds3231_date_t *date){

	uint8_t data_to_read[4], year;
	uint16_t century;
	i2c_read_bytes(DS3231_ADDRESS, DS3231_DAY_REF_ADDR, data_to_read, sizeof(data_to_read));

	date->dow = bcd_to_decimal_conversion(data_to_read[0]);
	date->date = bcd_to_decimal_conversion(data_to_read[1]);
	date->month = bcd_to_decimal_conversion(data_to_read[2]);
	year = bcd_to_decimal_conversion(data_to_read[3]);
	century = (data_to_read[2] >> CENTURY_BIT_IN_MONTH_REG )*CENTURY_FACTOR + CURRENT_CENTURY_OFFSET; // calculating the century
	date->month = date->month & EXTRACTING_SEVEN_BIT_MASK;  // removing the century bit from month register
	date->year = century + year;

}

/*
 * Description: It reads the control status back using i2c from the RTC DS3231 and converts them back into decimal form
 *
 * Parameters:
 *    		ds3231_time_t a pointer which contains the read data from the RTC
 *
 * Returns:
 *   		NULL
 */
void ds3231_error_status(uint8_t *status){

	i2c_read_bytes(DS3231_ADDRESS, DS3231_CONTROL_STATUS, status, 1);
	*status = bcd_to_decimal_conversion(*status);

}


/*
 * Description: it tells the day of week by comapring the data read from rtc
 *
 * Parameters:
 *    		uint8_t a number which corresponds to the day of week
 *
 * Returns:
 *   	char *    a car pointer which contains a string
 */
char *ds3231_get_day_of_week(uint8_t dow){
	switch(dow){

	case 0:
		return "SUNDAY";
		break;

	case 1:
		return "MONDAY";
		break;

	case 2:
		return "TUESDAY";
		break;

	case 3:
		return "WEDNESDAY";
		break;

	case 4:
		return "THURSDAY";
		break;

	case 5:
		return "FRIDAY";
		break;

	case 6:
		return "SATURDAY";
		break;

	default:
		break;
	}

	return NULL;
}
