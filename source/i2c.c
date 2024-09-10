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
 * @file    i2c.c
 * @brief   This file contains the functions related to i2c drivers for communicating with various devices.
 *
 * @author  Pranjal Gupta
 * @date    12/3/2023
 *
 *
 *
 */
#include "MKL25Z4.h"
#include "i2c.h"
#include "stdbool.h"

static void i2c_start(uint8_t device_addr, uint8_t write_or_read);
static uint8_t read_single_byte(bool is_reapeated_read, uint8_t ack_or_nack);
static void i2c_stop(void);
static void i2c_delay(void);

#define ICR_FACTOR 0x1E
#define I2C0_SDA_PIN 9
#define I2C0_SCL_PIN 8
#define I2C0_ALT_FUNC_NUM 2
#define READ 1
#define WRITE 0
#define NACK 1
#define ACK 0


/*
 * Description: initialises the i2c0 and enables the clock
 *
 * Parameters:
 *    		None
 *
 * Returns:
 *   		None
 */

void i2c0_init(void) {

	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;            // ENABLING CLOCK FOR I2C0
	I2C0->C1 = 0;                         // clearing all the bits and resetting
	I2C0->F = I2C_F_ICR(ICR_FACTOR);              // setting the baud rate       // 186KHZ FREQ,
	I2C0->C1 |= I2C_C1_IICEN_MASK;                // enabling the i2c module
}


/*
 * Description: initialises the i2c0 apins
 *
 * Parameters:
 *    		None
 *
 * Returns:
 *   		None
 */


void i2c0_pins_init() {

	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;              // enabling clock for PORTC
	PORTC->PCR[I2C0_SCL_PIN] = PORT_PCR_MUX(I2C0_ALT_FUNC_NUM);                     // I2C_SCL
	PORTC->PCR[I2C0_SDA_PIN] = PORT_PCR_MUX(I2C0_ALT_FUNC_NUM);

}


/*
 * Description: Send the start condition during write and read operation
 * Parameters:
 *    		uint8_t device address of the slave
 *    		uint8_t flag which states that the start is for read or write
 *
 * Returns:
 *   		None
 */

static void i2c_start(uint8_t device_addr, uint8_t write_or_read) {

	device_addr = (device_addr << 1 | write_or_read);
	if (write_or_read == READ)
		I2C0->C1 &= ~I2C_C1_TX_MASK;
	else
		I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->D = device_addr;
	while (!(I2C0->S & I2C_S_IICIF_MASK))
		;
	I2C0->S |= I2C_S_IICIF_MASK;

}

/*
 * Description: Send the stop condition followed by a small delay to account for transaction complete
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */


static void i2c_stop(void) {

	I2C0->C1 &= ~(I2C_C1_MST_MASK);
	I2C0->C1 &= ~(I2C_C1_TX_MASK);
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

void i2c_data_transmit(uint8_t device_addr, uint8_t *data, int length) {
	i2c_start(device_addr, WRITE);

	for (int i = 0; i < length; i++) {

		I2C0->D = data[i];
		while (!(I2C0->S & I2C_S_IICIF_MASK)) {

		}

		I2C0->S |= I2C_S_IICIF_MASK;  // Clear the interrupt flag

	}

	i2c_stop();

	i2c_delay();
}



/*
 * Description: produces a small delay to give time for transaction to complete
 * Parameters:
 * 		None
 * Returns:
 *   		None
 */
static void i2c_delay(void){
	for (int i = 0; i < 1000; i++)
		;

}


/*
 * Description: reads the data from the slave collectively according to the length asked
 * Parameters:
 * 		uint8_t device addr the device address of the slave
 * 		uint8_t read_addr  address of the register to be read from the slave
 * 		uint8_t *data the data buffer to store the read data
 * 		uint8_t length  the length of the data to be read
 * Returns:
 *   		None
 */

void i2c_read_bytes(uint8_t device_addr, uint8_t read_addr, uint8_t *rx_buffer,
		uint8_t length) {

	i2c_start(device_addr, WRITE);

	while (!(I2C0->S & I2C_S_TCF_MASK)) {

	}

	I2C0->D = read_addr;        // sending the register address

	while (!(I2C0->S & I2C_S_IICIF_MASK)) {

	}
	I2C0->S |= I2C_S_IICIF_MASK;  // Clear the interrupt flag

	I2C0->C1 |= I2C_C1_RSTA_MASK;         // repeated start flag set

	rx_buffer[0] = I2C0->D;
	i2c_start(device_addr, READ);

	while (!(I2C0->S & I2C_S_TCF_MASK))
		;

	i2c_delay();

	for (int i = 0; i < length; i++) {
		rx_buffer[i] = read_single_byte(true, i == (length - 1));

	}

	i2c_stop();

}
/*
 * Description: reads a single data from the slave
 * Parameters:
 * 		bool  specifies is it a repeated read or read after sending the stop
 * 		uint8_t tells is it the last read byte, if not then send ack otherwise send nack
 *
 * Returns:
 *   		uint8_t the read byte
 */

static uint8_t read_single_byte(bool is_repeated_read, uint8_t ack_or_nack) {
	uint8_t return_data;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	if (ack_or_nack)
		I2C0->C1 |= I2C_C1_TXAK_MASK;
	else
		I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	if (is_repeated_read == true)
		return_data = I2C0->D;

	while (!(I2C0->S & I2C_S_IICIF_MASK))
		;
	I2C0->S |= I2C_S_IICIF_MASK;

	if (ack_or_nack == NACK)
		I2C0->C1 &= ~(I2C_C1_MST_MASK);

	return_data = I2C0->D;

	return return_data;

}


