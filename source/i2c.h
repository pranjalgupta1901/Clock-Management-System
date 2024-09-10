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
 * @file    i2c.h
 * @brief   This file has function prototypes related to i2c communication like read, write intialise.
 *
 * @author  Pranjal Gupta
 * @date    10/01/2023
 *
 */

#ifndef I2C_H_
#define I2C_H_

#include"stdint.h"

void i2c_data_transmit(uint8_t device_addr, uint8_t *data, int length);
void i2c_read_bytes(uint8_t device_addr, uint8_t read_addr, uint8_t *rx_buffer, uint8_t length);
void i2c0_pins_init();
void i2c0_init(void);
#endif /* I2C_H_ */
