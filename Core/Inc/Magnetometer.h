/*
 * Magnetometer.h
 *
 *  Created on: 1 Dec 2020
 *      Author: ckw
 */

#ifndef INC_MAGNETOMETER_H_
#define INC_MAGNETOMETER_H_

#define WHOAM 			0x0F //Default 0x0F

#define CTRL1  			0x20 //Default 0x20
#define CTRL2 			0x21 //Default 0x21
#define CTRL3 			0x22 //Default 0x22
#define CTRL4 			0x23 //Default 0x23
#define CTRL5 			0x24 //Default 0x24

#define MAG_OUT_X_L		0x28 //Default 0x28
#define MAG_OUT_X_H		0x29 //Default 0x29
#define MAG_OUT_Y_L		0x2A //Default 0x2A
#define MAG_OUT_Y_H		0x2B //Default 0x2B
#define MAG_OUT_Z_L		0x2C //Default 0x2C
#define MAG_OUT_Z_H		0x2D //Default 0x2D

#endif /* INC_MAGNETOMETER_H_ */
