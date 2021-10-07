
uint8_t config_gyro_registers();
uint8_t registerwrite(uint8_t, uint8_t, uint8_t);
uint8_t registerread(uint8_t, uint8_t);

int main(void)
    {
  	config_gyro_registers();
  	HAL_Delay(10000);

  	while (1){
  	registerread(0x22,0xD4); //reads address 0x22 of the 0xD4 device (OUTX_L of the gyro)
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

uint8_t config_gyro_registers(){
    registerwrite(0x11, 0xA2, 0xD4); //defaults 0x11 0x00 // control register address, data value to write
    registerwrite(0x14, 0x60, 0xD4); //defaults 0x14 0x00 // X , X , slave address (r/w for your device)
    registerread(0x11, 0xD4); //reads address 0x11 of 0xD4 device
    registerread(0x14, 0xD4);
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
	
	//if you want to print the value to screen
	//HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "Value: %p\n\r", *contents), HAL_MAX_DELAY);

	return contents;
}

