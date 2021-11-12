#ifndef india_bar
#define india_bar
//Control registers - turn sensor on and off
#define CTRL1_B 0x10 //CTRL_REG1
#define CTRL2_B 0x11 //CTRL_REG2
#define CTRL3_B 0x12 //CTRL_REG3
//Data registers - provide data from sensor
#define PRESS_XL 0x28 //PRESS_OUT_XL
#define PRESS_L 0x29 //PRESS_OUT_L
#define PRESS_H 0x2A //PRESS_OUT_H
//Initialising functions
uint8_t bar_on(UART_HandleTypeDef);
uint8_t bar_use(UART_HandleTypeDef);
uint8_t bar_off(UART_HandleTypeDef);
uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);
//Function for turning sensor on
uint8_t bar_on(){
  	uint8_t contents[1];
    registerwrite(0x10, 0x10, 0xBB); //CTRL_REG1 on continuous mode
    registerwrite(0x11, 0x10, 0xBB); //CTRL_REG2 on continuous mode
    registerwrite(0x12, 0x04, 0xBB); //CTRL_REG3 on continuous mode
    printf("\r\nBarometer on\n\r"); //Prints out sensor on
  }
//Function for collecting sensor data continuously
uint8_t bar_use(){
	  uint8_t contents[1];
      uint8_t PRESS_XL = registerread(0x28, 0xBB); //PRESS_OUT_XL get data
      uint8_t PRESS_L = registerread(0x29, 0xBB); //PRESS_OUT_L get data
      uint8_t PRESS_H = registerread(0x2A, 0xBB); //PRESS_OUT_H get data
      int div;
      div = (PRESS_H<<16)|(PRESS_L<<8)|(PRESS_XL); //Puts data bytes in order, refer to datasheet
      int PR = div/4096; //Converts to hPa, refer to datasheet
      printf("\r\nBarometer: '%d' hPa\n\r", PR); //Prints out data
    }
//Function for turning sensor off
uint8_t bar_off(){
	  uint8_t contents[1];
     registerwrite(0x10, 0x00, 0xBB); //CTRL_REG1 set to off/reset
     registerwrite(0x11, 0x03, 0xBB); //CTRL_REG2 set to off/reset
     registerwrite(0x12, 0x00, 0xBB); //CTRL_REG3 set to off/reset
     printf("\r\nBarometer off\n\r"); //Prints out sensor off
   }


#endif /* india_bar */
