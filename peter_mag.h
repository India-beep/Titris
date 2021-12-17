#ifndef INC_PETER_MAG_H_
#define INC_PETER_MAG_H_

uint8_t registerwrite(uint8_t, uint8_t, uint16_t);
uint8_t registerread(uint8_t, uint16_t);
uint8_t memregisterread(uint8_t, uint16_t);
void Magnet_Init(void);
void Magnet_Collect(void);
void Magnet_Disable(void);

void Magnet_Init(void){
		registerwrite(0x20, 0x08, 0x3D); //Initiates Control Register 1 (Reg_Adr, Data, Chip_Adr)
		registerwrite(0x21, 0x70, 0x3D); //Initiates Control Register 2 (Reg_Adr, Data, Chip_Adr)
		registerwrite(0x22, 0x01, 0x3D); //Initiates Control Register 3 (Reg_Adr, Data, Chip_Adr)
		registerwrite(0x23, 0x00, 0x3D); //Initiates Control Register 4 (Reg_Adr, Data, Chip_Adr)
		registerwrite(0x24, 0x00, 0x3D); //Initiates Control Register 5 (Reg_Adr, Data, Chip_Adr)
}
void Magnet_Collect(void){
	uint8_t Low_X[1], High_X[1];  //Variables to hold X-axis data
	uint8_t Low_Y[1], High_Y[1];  //Variables to hold Y-axis data
	uint8_t Low_Z[1], High_Z[1];  //Variables to hold Z-axis data
	int x, y, z;  //Variables to hold numerical value of 3 axis

	*Low_X = registerread(0x28, 0x3D);  //Read Lower byte of X-axis from register
	*High_X = registerread(0x29, 0x3D);  //Read Upper byte of X-axis from register
	*Low_Y = registerread(0x2A, 0x3D);  //Read Lower byte of Y-axis from register
	*High_Y = registerread(0x2B, 0x3D);  //Read Upper byte of Y-axis from register
	*Low_Z = registerread(0x2C, 0x3D);  //Read Lower byte of Z-axis from register
	*High_Z = registerread(0x2D, 0x3D);  //Read Upper byte of Z-axis from register

	x = ((*Low_X<<8)|(*High_X))/1000;  //Stitches both X-axis bytes together
	y = ((*Low_Y<<8)|(*High_Y))/1000;  //Stitches both Y-axis bytes together
	z = ((*Low_Z<<8)|(*High_Z))/1000;  //Stitches both Z-axis bytes together
	uint16_t key =0;
	//key = getchar();
	//if(key != 0){
	printf("X-axis = %d G \n\r", x); //Prints X-axis value to terminal
	printf("--Y-axis = %d G \n\r", y); //Prints Y-axis value to terminal
	printf("----Z-axis = %d G \n\r", z); //Prints Z-axis value to terminal
	//}
}
void Magnet_Disable(void){
	registerwrite(0x20, 0x00, 0x3D); //Disables Control Register 1 (Reg_Adr, Data, Chip_Adr)
	registerwrite(0x21, 0x00, 0x3D); //Disables Control Register 2 (Reg_Adr, Data, Chip_Adr)
	registerwrite(0x22, 0x00, 0x3D); //Disables Control Register 3 (Reg_Adr, Data, Chip_Adr)
	registerwrite(0x23, 0x00, 0x3D); //Disables Control Register 4 (Reg_Adr, Data, Chip_Adr)
	registerwrite(0x24, 0x00, 0x3D); //Disables Control Register 5 (Reg_Adr, Data, Chip_Adr)
}

#endif /* INC_PETER_MAG_H_ */
