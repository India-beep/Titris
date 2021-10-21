/*
 * Charlie_humid.h
 *
 *  Created on: 14 Oct 2021
 *      Author: Charlie Ward
 */

#ifndef INC_CHARLIE_HUMID_H_
#define INC_CHARLIE_HUMID_H_


#define H_DEV_ADDR			0xBE
/* Sensor address on I2C bus */


#define H_WHO_AM_I			0x0F
/* Device ID Register */


#define H_AV_CONF			0x10
/* Average Configuration Register */
	/* Bits 3, 4, 5 set averaged temperature samples
	 * Bits 0, 1, 2 set averaged humidity samples */


#define H_CTRL_REG1			0x20
/* Control Register 1 */
	/* Bit 7 sets power-down mode
	 * Bit 2 sets block data update
	 * Bits 0, 1 set output data rate*/


#define H_CTRL_REG2			0x21
/* Control Register 2 */
	/* Bit 7 sets BOOT mode
	 * Bit 1 sets Heater
	 * Bit 0 sets ONE_SHOT*/


#define H_CTRL_REG3			0x22
/* Control Register 3 */
	/* Bit 7 sets Data Ready signal active high / low
	 * Bit 6 sets Push-pull / Open Drain on pin 3
	 * Bit 2 sets Data Ready Enable on pin 3 */


#define H_STATUS_REG		0x27
/* Status Register */
	/* Bit 1 for humidity data available
	 * Bit 0 for temperature data available */


#define H_HUMIDITY_OUT_L	0x28
#define H_HUMIDITY_OUT_H	0x29
#define H_TEMP_OUT_L		0x2A
#define H_TEMP_OUT_H		0x2B
/* Output Registers */
	/* L for LSB, H for MSB */


/// Private function prototypes ///

uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);
uint8_t memregisterread(uint8_t, uint16_t);


/// Functions	///


uint8_t H_ReadID(UART_HandleTypeDef huart1){
	char buf[32];
	uint8_t contents[1];

	*contents = registerread(H_WHO_AM_I, H_DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumidity DevID: %p\n\r", *contents), HAL_MAX_DELAY);
}

/// WTF is going on with the arrays / pointers / variables?
/// .... thinking
/// couldn't figure out why the line " *contents = ..." is necessary
/// but then thinking about it 'registerread' is a function defined to return a uint8_t data type
/// so previous approaches which relied on the back to back hand-over of 'HAL_I2C_Mem_Read' and 'HAL_UART_Transmit' ...
/// probably won't work any more.


uint8_t H_Init(UART_HandleTypeDef huart1){
	char buf[32];
	uint8_t contents[1];

    registerwrite(H_AV_CONF, 0x3F, H_DEV_ADDR);
    registerwrite(H_CTRL_REG1, 0x83, H_DEV_ADDR);
    registerwrite(H_CTRL_REG2, 0x00, H_DEV_ADDR);
    registerwrite(H_CTRL_REG3, 0x00, H_DEV_ADDR);

	*contents = registerread(H_AV_CONF, H_DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumidity Config: %p\n\r", *contents), HAL_MAX_DELAY);

	*contents = registerread(H_CTRL_REG1, H_DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumidity CTRL_1: %p\n\r", *contents), HAL_MAX_DELAY);

	*contents = registerread(H_CTRL_REG2, H_DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumidity CTRL_2: %p\n\r", *contents), HAL_MAX_DELAY);

	*contents = registerread(H_CTRL_REG3, H_DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumidity CTRL_3: %p\n\r", *contents), HAL_MAX_DELAY);

	*contents = registerread(H_STATUS_REG, H_DEV_ADDR);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rStatus: %p\n\r", *contents), HAL_MAX_DELAY);

}

uint8_t H_Read(UART_HandleTypeDef huart1){
	char buf[32];
	uint8_t H_H, H_L, T_H, T_L;
	int16_t T_Out, H_Out;

	T_H = memregisterread(H_TEMP_OUT_H, H_DEV_ADDR);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rTemp High: %d\n\r", T_H), HAL_MAX_DELAY);

	T_L = memregisterread(H_TEMP_OUT_L, H_DEV_ADDR);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Temp Low: %d\n\r", T_L), HAL_MAX_DELAY);

	T_Out = T_H <<8 | T_L;
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rTemp: %d\n\r", T_Out), HAL_MAX_DELAY);


	H_H = memregisterread(H_HUMIDITY_OUT_H, H_DEV_ADDR);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumid High: %d\n\r", H_H), HAL_MAX_DELAY);

	H_L = memregisterread(H_HUMIDITY_OUT_L, H_DEV_ADDR);
//	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Humid Low: %d\n\r", H_L), HAL_MAX_DELAY);

	H_Out = H_H <<8 | H_L;
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n\rHumid: %d\n\r", H_Out), HAL_MAX_DELAY);


}

#endif /* INC_CHARLIE_HUMID_H_ */
