#ifndef jordan_gyro
#define jordan_gyro

#define CTRL2_G 0x11
#define CTRL5_G 0x14
#define CTRL2_G_VAL 0xA2
#define CTRL5_G_VAL 0x60
#define SensorLGX 0x22 //lowest bits of gyroscope x sensor
#define SensorLGY 0x24
#define SensorLGZ 0x26
#define SensorHGX 0x23 //highest bits of gyroscope x sensor
#define SensorHGY 0x25
#define SensorHGZ 0x27
#define GYRO_SLAVE_ADDR 0xD4

uint8_t config_gyro_registers(UART_HandleTypeDef);
uint8_t reset_gyro_registers(UART_HandleTypeDef);
uint8_t gyro_getxyz(UART_HandleTypeDef);
uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);

/*for main.c
add config_gyro_registers before the while(1)
add gyro_getxyz into while(1)

note i could put registerwrite/read into a header file but they should be in main
because all functions either read or write to a register at some point
*/

uint8_t config_gyro_registers(UART_HandleTypeDef huart1){
    char buf[16];
    uint8_t contents[1];

    registerwrite(CTRL2_G, CTRL2_G_VAL, GYRO_SLAVE_ADDR);
    registerwrite(CTRL5_G, CTRL5_G_VAL, GYRO_SLAVE_ADDR);

    *contents = registerread(CTRL2_G, GYRO_SLAVE_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL2_G: %p\n\r", *contents), HAL_MAX_DELAY);

    *contents = registerread(CTRL5_G, GYRO_SLAVE_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL5_G: %p\n\r", *contents), HAL_MAX_DELAY);
}

uint8_t reset_gyro_registers(UART_HandleTypeDef huart1){
    char buf[16];
    uint8_t contents[1];

    registerwrite(CTRL2_G, 0x00, GYRO_SLAVE_ADDR);
    registerwrite(CTRL5_G, 0x00, GYRO_SLAVE_ADDR);

    *contents = registerread(CTRL2_G, GYRO_SLAVE_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL2_G: %p\n\r", *contents), HAL_MAX_DELAY);

    *contents = registerread(CTRL5_G, GYRO_SLAVE_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL5_G: %p\n\r", *contents), HAL_MAX_DELAY);
}

uint8_t gyro_getxyz(UART_HandleTypeDef huart1){ //put into while(1)
	char buf[32];
	uint16_t contents[1];

	*contents = (registerread(SensorLGX, GYRO_SLAVE_ADDR)*256 + registerread(SensorHGX, GYRO_SLAVE_ADDR))-1152;
	if (*contents == 0){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "GYRO X MOVING NEGATIVE: %d\n\r", *contents), HAL_MAX_DELAY);
	}
	if (*contents == 63487){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "GYRO X MOVING POSITIVE: %d\n\r", *contents), HAL_MAX_DELAY);
	}


	*contents = (registerread(SensorLGY, GYRO_SLAVE_ADDR)*256 + registerread(SensorHGY, GYRO_SLAVE_ADDR))-1152;
	if (*contents == 0){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "GYRO Y MOVING NEGATIVE: %d\n\r", *contents), HAL_MAX_DELAY);
	}
	if (*contents == 63487){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "GYRO Y MOVING POSITIVE: %d\n\r", *contents), HAL_MAX_DELAY);
	}


	*contents = (registerread(SensorLGZ, GYRO_SLAVE_ADDR)*256 + registerread(SensorHGZ, GYRO_SLAVE_ADDR))-1152;
	if (*contents == 0){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "GYRO Z MOVING NEGATIVE: %d\n\r", *contents), HAL_MAX_DELAY);
	}
	if (*contents == 63487){
	HAL_UART_Transmit(&huart1, (uint16_t*)buf, sprintf(buf, "GYRO Z MOVING POSITIVE: %d\n\r", *contents), HAL_MAX_DELAY);
	}

	return 0;
}

#endif
