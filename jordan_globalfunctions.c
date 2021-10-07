#include "main.h"

#include <stdio.h>
#include <string.h>


uint8_t config_SLAVE_registers();
uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);


int main(void)
{

  	config_SLAVE_registers();
  	HAL_Delay(10000);

  	while (1){
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

uint8_t config_SLAVE_registers(){
    char buf[16];
	uint8_t contents[1];
	/* for my gyro for example */
	uint8_t YOUR_REGISTER_ADDRESS = 0x11; // ctrl2 address
    uint8_t VALUE_TO_WRITE_INTO_REGISTER = 0xA2; // value to put into 0x11 (ctrl2), default is 0x00
	uint8_t SLAVE_ADDRESS_RW = 0xD4; // read write mode of gyro
	/* end of example */
	registerwrite(YOUR_REGISTER_ADDRESS, VALUE_TO_WRITE_INTO_REGISTER, SLAVE_ADDRESS_RW);
    *contents = registerread(YOUR REGISTER ADDRESS, SLAVE ADDRESS R/W);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "VALUE IN REGISTER: %p\n\r", *contents), HAL_MAX_DELAY);
}

uint8_t registerwrite(uint8_t addr, uint8_t data, uint16_t slave_addr){
	uint8_t ret=0;
	uint8_t buf[] = {addr, data};

	if(HAL_I2C_Master_Transmit(&hi2c2, slave_addr, buf, 2, 100) != HAL_OK){
    ret=0x00; //error return to default
	}
	return ret;
}

uint8_t registerread(uint8_t addr, uint16_t slave_addr){
	uint8_t contents[1];
	char buf[16];

	if(HAL_I2C_Master_Transmit(&hi2c2, slave_addr, &addr, 1, 100) != HAL_OK){
    contents[1] = 0x00;
	}

	if(HAL_I2C_Master_Receive(&hi2c2, slave_addr, &contents, 1, 100) != HAL_OK){
	contents[1] = 0x00;
	}

	return *contents;
}