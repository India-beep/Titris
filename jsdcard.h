/*
 * jsdcard.h
 *
 *  Created on: 4 Nov 2021
 *      Author: jordan
 */

/**************------------NOTE-------------*****************
 *
 * fatfs_sd.h library uses FATFS data and makes it compatible
 * to be transmitted/received via SPI. My code below uses the
 * inbuilt FATFS STM32 library to handle files which is then
 * pass to the fatfs_sd.h to transmit/receive via SPI.
 *
 * This is so that the SD card could be modified on a computer.
 * Otherwise, a homemade file system wouldn't be readily
 * recognised on windows.
 *
 * https://blog.naver.com/eziya76/221188701172
 ************************************************************/

#ifndef JSDCARD_H_
#define JSDCARD_H_

extern FATFS fs; //file system
extern FIL fil; //file object structure
extern UART_HandleTypeDef huart1; //uses global defined uart in main
extern FRESULT fresult; //file function return code

extern char buffer [2048];

extern UINT br, bw; //pointer to number of bites to read, pointer to number of bytes to write (if using fwrite)

extern FATFS *pfs; //file system pointer
extern DWORD fre_clust; //free current cluster of fptr
extern uint32_t total, free_space; //ints for free/total space

int sprintf(char *str, const char *string,...);
int jbufsize(char*);
void jclear_buffer(void*);
void jUARTSEND(char*);
void jmountsdcard();
void junmountsdcard();
void jchecktotalspace();
void jwritetofile(void*, void*);
void jreadfile(void*);
void jupdatefile(void*, void*);
void jdeletefile(void*);

int jbufsize (char *buf){
	int i=0;
	while (*buf++ !=  '\0') i++;
	return i;
}

void jclear_buffer (void *b){
	char *buffer = (char*)b;
	for (int i=0; i<2048; i++){
	buffer[i] = '\0';
	}
}

void jUARTSEND (char *string){
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *) string, len, 2000);
}

void jmountsdcard (){
	fresult = f_mount(&fs, "/", 1);
	if (fresult != FR_OK) jUARTSEND("ERROR MOUNTING SD CARD\r\n\r\r\n\r");
	else jUARTSEND("SD CARD mounted successfully\r\n\r\r\n\r");
}

void junmountsdcard(){
	fresult = f_mount(NULL, "/", 1);
	if (fresult == FR_OK) jUARTSEND ("\r\n\rSD CARD UNMOUNTED SUCCESS\r\n\r");
}

void jchecktotalspace(){
	f_getfree("", &fre_clust, &pfs);
	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	sprintf (buffer, "SD CARD Total Size: \t%lu\r\n\r",total);
	jUARTSEND(buffer);
	jclear_buffer(buffer);
}

void jcheckfreespace(){
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	sprintf (buffer, "SD CARD Free Space: \t%lu\r\n\r\r\n\r",free_space);
	jUARTSEND(buffer);
	jclear_buffer(buffer);
}

void jwritefile(void *vfilename, void *vcontents){
	char *cfilename = (char*)vfilename;
	char *contents = (char*)vcontents;
    fresult = f_open(&fil, cfilename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
  	f_puts(contents , &fil);
  	fresult = f_close(&fil);
  	if (fresult == FR_OK) {
  		jUARTSEND(cfilename);
  		jUARTSEND (" CREATED AND DATA WRITTEN\r\n\r");
  	}
}

void jreadfile(void *vfilename){
	jclear_buffer(buffer);
	char *cfilename = (char*)vfilename;
	fresult = f_open (&fil, cfilename, FA_READ);
	fresult = f_read (&fil, buffer, f_size(&fil), &br);
	char *string = "\r\n\rBELOW SHOWS DATA OF ";
	if (fresult == FR_OK) jUARTSEND (string);
	jUARTSEND(cfilename);
	jUARTSEND(": \r\n\r");
	jUARTSEND(buffer);
	jUARTSEND("\r\n\r\r\n\r");
	f_close(&fil);
    jclear_buffer(buffer);
}

void jupdatefile(void *vfilename, void *vcontents){
	char *cfilename = (char*)vfilename;
	char *contents = (char*)vcontents;
  	fresult = f_open(&fil, cfilename, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
  	fresult = f_lseek(&fil, f_size(&fil));
  	if (fresult == FR_OK){
  		jUARTSEND("\r\n\rUPDATING ");
  		jUARTSEND(cfilename);
  		jUARTSEND("\r\n\r");
  	}
  	fresult = f_puts(contents, &fil);
  	f_close (&fil);
  	jclear_buffer(buffer);
}

void jdeletefile(void *vfilename){
	char *cfilename = (char*)vfilename;
	if (cfilename[0] != '/'){ //if filename given isnt in a folder
		char *slash = "/";
		char *result = malloc(strlen(cfilename) + strlen(slash) + 1); // add 1 for terminator
		strcpy(result,slash);
		strcat(result,cfilename);
		cfilename = result;
	}
	fresult = f_unlink(cfilename);
	if (fresult == FR_OK) {
		jUARTSEND("\r\n\r");
		jUARTSEND(cfilename);
		jUARTSEND(" REMOVED SUCCESS\r\n\r");
	}
}

#endif /* JSDCARD_H_ */
