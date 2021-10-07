
//i added stdio and string to main header
#include "main.h"
 
 
 
//up with prototypes
uint8_t configregisters();
uint8_t registerwrite(uint8_t, uint8_t);
uint8_t registerread(uint8_t);
//
 
int main(void)
{
  HAL_Init();
 
  SystemClock_Config();
 
  MX_GPIO_Init();
  MX_DFSDM1_Init();
  MX_I2C2_Init();
  MX_QUADSPI_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
 
    configregisters();
    while (1)
    {
 
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
 
uint8_t configregisters(){
    registerwrite(0x11, 0xA2); //def 0x11 0x00
    registerwrite(0x14, 0x60); //def 0x14 0x00
    registerread(0x11);
    registerread(0x14);
}
 
uint8_t registerwrite(uint8_t addr, uint8_t data){
    uint8_t ret=0;
    uint8_t buf[] = {addr, data};
 
    uint16_t slave_addr = 0xD4; //SAD R/W addr
 
    if(HAL_I2C_Master_Transmit(&hi2c2, slave_addr, buf, 2, 100) != HAL_OK){
    ret=0x00; //error return to default
    }
    return ret;
}
 
uint8_t registerread(uint8_t addr){
    uint8_t contents[1];
    char buf[16];
    uint16_t slave_addr = 0xD4;
 
    if(HAL_I2C_Master_Transmit(&hi2c2, slave_addr, &addr, 1, 100) != HAL_OK){
    contents[1] = 0x00;
    }
 
    if(HAL_I2C_Master_Receive(&hi2c2, slave_addr, &contents, 1, 100) != HAL_OK){
    contents[1] = 0x00;
    }
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, sprintf(buf, "CTRL: %p\n\r", *contents), HAL_MAX_DELAY);
    return contents;
}
 