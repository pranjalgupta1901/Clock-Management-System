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
 * @file    DS3231.h
 * @brief   This file has function prototypes for DS3231 RTC operation
 *
 * @author  Pranjal Gupta
 * @date    10/01/2023
 *
 */

#ifndef DS3231_H_
#define DS3231_H_

#include "stdint.h"

#define DS3231_ADDRESS 0x68

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
}ds3231_time_t;

typedef struct {
	uint8_t dow;
	uint8_t date;
	uint8_t month;
	uint16_t year;
}ds3231_date_t;


void ds3231_set_time(ds3231_time_t *time);
void ds3231_read_time(ds3231_time_t *time);
void ds3231_set_date(ds3231_date_t *date);
void ds3231_read_date(ds3231_date_t *date);
char *ds3231_get_day_of_week(uint8_t dow);
void ds3231_error_status(uint8_t *status);





#endif /* DS3231_H_ */
