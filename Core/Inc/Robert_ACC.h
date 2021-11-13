/*
 * Robert_ACC.h
 *
 *  Created on: Oct 14, 2021
 *      Author: rober
 */

#ifndef INC_ROBERT_ACC_H_
#define INC_ROBERT_ACC_H_

#include <stdio.h>

#define DEV_ADDR (uint8_t)0xD4

/* WHO_AM_I address*/

#define ACC_ID (uint8_t)0x0F

/* Register initialisation */
#define ACC_CTRL1 0x10
#define ACC_CTRL3 0x12
#define ACC_CTRL4 0x13
#define ACC_CTRL5 0x14
#define ACC_CTRL6 0x15
#define ACC_CTRL7 0x16

#define ACC_CTRL1_VAL 0x10
#define ACC_CTRL1_Reset 0x00


/* Page numbers: 61,62,63,64,65,66,68 */

/* Data Register definitions */

/*FIFO Data Registers*/
#define FIFO_DATA_L    0x3E
#define FIFO_DATA_H    0x3F

/*accelerometer Data */
#define ACC_X_L           0x28
#define ACC_X_H           0x29
#define ACC_Y_L           0x2A
#define ACC_Y_H           0x2B
#define ACC_Z_L           0x2C
#define ACC_Z_H           0x2D

uint8_t ACC_Init(UART_HandleTypeDef);
uint8_t ACC_DeInit(UART_HandleTypeDef);
uint8_t ACC_getxyz(UART_HandleTypeDef);
uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);
uint8_t memregisterread(uint8_t, uint16_t);

/////////Functions/////////

uint8_t config_ACC_registers(UART_HandleTypeDef huart1){
    char buf[16];
    uint8_t contents[1];

    registerwrite(ACC_CTRL1, ACC_CTRL1_VAL, DEV_ADDR);

    *contents = registerread(ACC_CTRL1, DEV_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "ACC_CTRL1: %p\n\r", *contents), HAL_MAX_DELAY);
}


uint8_t ACC_DeIinit(UART_HandleTypeDef huart1){
    char buf[16];
    uint8_t contents[1];

    registerwrite(ACC_CTRL1, ACC_CTRL1_Reset, DEV_ADDR);

    *contents = registerread(ACC_CTRL1, DEV_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "ACC_CTRL1: %p\n\r", *contents), HAL_MAX_DELAY);
}

uint8_t ACC_Read(UART_HandleTypeDef huart1){ //put into while(1)
	char buf[32];
	int16_t ACCX, ACCY, ACCZ;

	/*************************************HOW IT WORKS*****************************************************/
	/*LOWBIT_X = memregisterread(ACC_X_L, DEV_ADDR);
	HIGHBIT_X = memregisterread(ACC_X_H, DEV_ADDR);

	A_X_H= HIGHBIT_X;
	A_X_H = A_X_H << 8;
	A_X_H |= LOWBIT_X;

	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n ACC X: %d\n\r", ACCX), HAL_MAX_DELAY);*/
	/******************************************************************************************************/

	/*note: registerread and memregisterread are interchangable here*/
	ACCX = memregisterread(ACC_X_H, DEV_ADDR) << 8 | memregisterread(ACC_X_L, DEV_ADDR);
	/*note: +-2^15 is max value, at +30000 the ACC will be moving quite drastically positively*/
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "ACC +X: %d\n\r", ACCX), HAL_MAX_DELAY);

	ACCY = memregisterread(ACC_Y_H, DEV_ADDR) << 8 | memregisterread(ACC_Y_L, DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "ACC +Y: %d\n\r", ACCY), HAL_MAX_DELAY);

	ACCZ = memregisterread(ACC_Z_H, DEV_ADDR) << 8 | memregisterread(ACC_Z_L, DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "ACC +Z: %d\n\r", ACCZ), HAL_MAX_DELAY);

}
#endif /* INC_ROBERT_ACC_H_ */
