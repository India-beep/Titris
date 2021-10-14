#ifndef jordan_gyro
#define jordan_gyro

#define CTRL2_G 0x11
#define CTRL3_G 0x12
#define CTRL4_G 0x13
#define CTRL5_G 0x14
#define CTRL6_G 0x15
#define CTRL7_G 0x16
#define CTRL2_G_VAL 0x10
#define SensorLGX 0x22 //lowest bits of gyroscope x sensor
#define SensorHGX 0x23 //highest bits of gyroscope x sensor
#define SensorLGY 0x24
#define SensorHGY 0x25
#define SensorLGZ 0x26
#define SensorHGZ 0x27
#define GYRO_SLAVE_ADDR 0xD4

uint8_t config_gyro_registers(UART_HandleTypeDef);
uint8_t reset_gyro_registers(UART_HandleTypeDef);
uint8_t gyro_getxyz(UART_HandleTypeDef);
uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);
uint8_t memregisterread(uint8_t, uint16_t);

/****************for main.c*******************************************************
add config_gyro_registers before the while(1)
add gyro_getxyz into while(1)

note i could put registerwrite/read into a header file but they should be in main
because all functions either read or write to a register at some point
*********************************************************************************/

uint8_t config_gyro_registers(UART_HandleTypeDef huart1){
    char buf[16];
    uint8_t contents[1];

    registerwrite(CTRL2_G, CTRL2_G_VAL, GYRO_SLAVE_ADDR);

    *contents = registerread(CTRL2_G, GYRO_SLAVE_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL2_G: %p\n\r", *contents), HAL_MAX_DELAY);
}

uint8_t reset_gyro_registers(UART_HandleTypeDef huart1){
    char buf[16];
    uint8_t contents[1];

    registerwrite(CTRL2_G, 0x00, GYRO_SLAVE_ADDR);

    *contents = registerread(CTRL2_G, GYRO_SLAVE_ADDR);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL2_G: %p\n\r", *contents), HAL_MAX_DELAY);
}

uint8_t gyro_getxyz(UART_HandleTypeDef huart1){ //put into while(1)
	char buf[32];
	int16_t VALUE_GX, VALUE_GY, VALUE_GZ;

	/*************************************HOW IT WORKS*****************************************************/
	/*LOWBIT_X = memregisterread(SensorLGX, GYRO_SLAVE_ADDR);
	HIGHBIT_X = memregisterread(SensorHGX, GYRO_SLAVE_ADDR);

	VALUE_GX = HIGHBIT_X;
	VALUE_GX =  VALUE_GX << 8;
	VALUE_GX |= LOWBIT_X;

	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "\n Gyro X: %d\n\r", VALUE_GX), HAL_MAX_DELAY);*/
	/******************************************************************************************************/

	/*note: registerread and memregisterread are interchangable here*/
	VALUE_GX = memregisterread(SensorHGX, GYRO_SLAVE_ADDR) << 8 | memregisterread(SensorLGX, GYRO_SLAVE_ADDR);
	/*note: +-2^15 is max value, at +30000 the gyro will be moving quite drastically positively*/
	if(VALUE_GX>30000){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Gyro +X: %d\n\r", VALUE_GX), HAL_MAX_DELAY);
	}
	else if(VALUE_GX<-30000){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Gyro -X: %d\n\r", VALUE_GX), HAL_MAX_DELAY);
	}

	VALUE_GY = memregisterread(SensorHGY, GYRO_SLAVE_ADDR) << 8 | memregisterread(SensorLGY, GYRO_SLAVE_ADDR);
	if(VALUE_GY>30000){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Gyro +Y: %d\n\r", VALUE_GY), HAL_MAX_DELAY);
	}
	else if(VALUE_GY<-30000){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Gyro -Y: %d\n\r", VALUE_GY), HAL_MAX_DELAY);
	}

	VALUE_GZ = memregisterread(SensorHGZ, GYRO_SLAVE_ADDR) << 8 | memregisterread(SensorLGZ, GYRO_SLAVE_ADDR);
	if(VALUE_GZ>30000){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Gyro +Z: %d\n\r", VALUE_GZ), HAL_MAX_DELAY);
	}
	else if(VALUE_GZ<-30000){
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Gyro -Z: %d\n\r", VALUE_GZ), HAL_MAX_DELAY);
	}
	
	return 0;
}
#endif
