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
 * @file    project_tasks.c
 * @brief   This file is based on FreeRTOS and contains task for the application.
 * @author  Pranjal Gupta
 * @date    12/10/2023
 *
 */
#include "project_tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "oled_driver.h"
#include "DS3231.h"
#include "i2c.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "semphr.h"
#include "MKL25Z4.h"

TaskHandle_t rtc_set_handle;
TaskHandle_t rtc_read_handle;
TaskHandle_t init_handle;
TaskHandle_t monitor_failure_handle;

static void rtc_set_handler(void *parameters);
static void rtc_read_handler_and_display(void *parameters);
static void init_handler(void *parameters);
static void print_time_and_date(ds3231_date_t *date, ds3231_time_t *time);
static void monitor_failure_handler(void *parameters);

BaseType_t status;
uint8_t current_day = 0;

#define DEFAULT_STACK_SIZE 200
#define DEFAULT_PRIORITY 1
#define READ_TASK_STACK_SIZE 500
#define DEFAULT_COLUMN_POSITION 30
#define DEFAULT_BUFFER_SIZE 12
#define ERROR_PAGE_INDEX 6
#define DAY_PAGE_INDEX 4
#define DATE_PAGE_INDEX 2
#define TIME_PAGE_INDEX 0
#define OSC_BIT_EXTRACTION_MASK 0x80

/*
 * Description: initialises all the task required for the application
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */

void project_task_run(void) {

	status = xTaskCreate(init_handler, "INIT_TASK", DEFAULT_STACK_SIZE, NULL,
	DEFAULT_PRIORITY, &init_handle);

	configASSERT(status == pdPASS);

	status = xTaskCreate(rtc_set_handler, "RTC_SET_TASK1", DEFAULT_STACK_SIZE,
	NULL, DEFAULT_PRIORITY, &rtc_set_handle);

	configASSERT(status == pdPASS);

	status = xTaskCreate(rtc_read_handler_and_display, "RTC_READ_TASK2",
	READ_TASK_STACK_SIZE,
	NULL, DEFAULT_PRIORITY, &rtc_read_handle);

	configASSERT(status == pdPASS);

	status = xTaskCreate(monitor_failure_handler, "MONITOR_FAILURE",
	DEFAULT_STACK_SIZE,
	NULL, DEFAULT_PRIORITY, &monitor_failure_handle);

	configASSERT(status == pdPASS);

	vTaskStartScheduler();

}

/*
 * Description: task which takes care if there is any clock or power lost in the RTC by looking onto the OSC bit in the control register
 * Parameters:
 * 		void* parameters    a void pointer
 * Returns:
 *   		None
 */
static void monitor_failure_handler(void *parameters) {
	uint8_t status = 0;
	char buffer[DEFAULT_BUFFER_SIZE];

	while (1) {
		ds3231_error_status(&status);
		strcpy(buffer, "CLOCK LOST");
		if (status & (OSC_BIT_EXTRACTION_MASK)) {
			oled_clear_page(ERROR_PAGE_INDEX);
			oled_printstring(buffer, DEFAULT_COLUMN_POSITION, ERROR_PAGE_INDEX);
		}
	}
}

/*
 * Description: task which sets the date an time in the RTC
 * Parameters:
 * 		void* parameters
 * Returns:
 *   		None
 */
static void rtc_set_handler(void *parameters) {

	ds3231_date_t set_date;
	ds3231_time_t set_time;

	set_date.date = 13;
	set_date.dow = 3;
	set_date.month = 12;
	set_date.year = 2023;

	set_time.sec = 20;
	set_time.min = 10;
	set_time.hour = 23;

	while (1) {

		ds3231_set_date(&set_date);
		ds3231_set_time(&set_time);
		vTaskSuspend(NULL);           // suspending itself

	}

}

/*
 * Description: read the time and date from the rtc and prints it on the display
 * Parameters:
 * 		void *parameters
 * Returns:
 *   		None
 */

static void rtc_read_handler_and_display(void *parameters) {

	ds3231_date_t read_date;
	ds3231_time_t read_time;

	while (1) {

		ds3231_read_date(&read_date);
		ds3231_read_time(&read_time);

		print_time_and_date(&read_date, &read_time);

		}

	}


/*
 * Description: initialises all the peripherals and devices required for the application
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */
static void init_handler(void *parameters) {

	while (1) {
		i2c0_init();
		i2c0_pins_init();
		oled_init();
		oled_clearDisplay();
		vTaskSuspend(NULL);   // suspending itself after done initialisation

	}
}

/*
 * Description: prints the data on the display by calling oled drivers
 * Parameters:
 * 	ds3231_date_t *data  it contains the read date data from the RTC
 * 	ds3231_time_t *time   it contains the read time from the RTC
 * Returns:
 *   		None
 */
static void print_time_and_date(ds3231_date_t *date, ds3231_time_t *time) {

	char time_buffer[DEFAULT_BUFFER_SIZE], date_buffer[DEFAULT_BUFFER_SIZE],
			day[DEFAULT_BUFFER_SIZE];

	if (date == NULL)
		return;

	if (time == NULL)
		return;

	oled_clear_page(TIME_PAGE_INDEX);

	sprintf(time_buffer, "%02d:%02d:%02d", time->hour, time->min, time->sec);
	oled_printstring(time_buffer, DEFAULT_COLUMN_POSITION, TIME_PAGE_INDEX);

	if (current_day != date->dow) {
		sprintf(date_buffer, "%02d/%02d/%02d", date->date, date->month,
				date->year);
		strcpy(day, ds3231_get_day_of_week(date->dow));
		oled_clear_page(DATE_PAGE_INDEX);
		oled_printstring(date_buffer, DEFAULT_COLUMN_POSITION, DATE_PAGE_INDEX);
		oled_clear_page(DAY_PAGE_INDEX);
		oled_printstring(day, DEFAULT_COLUMN_POSITION, DAY_PAGE_INDEX);
		current_day = date->dow;
	}

}

