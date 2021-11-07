/*
 * dinoheader.h
 *
 *  Created on: 7 Nov 2021
 *      Author: jpjdo
 */

/* current main includes
#include "main.h"
#include "fatfs.h"
#include "fatfs_sd.h"
#include "string.h"
#include "jsdcard.h"
#include "dinoheader.h"
#include <stdio.h>
 */

#ifndef INC_DINOHEADER_H_
#define INC_DINOHEADER_H_

//screen size - oh heck dynamic map size
#define rows 32
#define cols 32

extern int dispm[rows][cols];

//oh heck dyanic dino sizes
const int dsize=8, hsize=4, hurdlesloc=32;
// NOTE IT DOES NOT LIKE DEFINES FOR THESE VALUES
// translation / runtime error

const uint8_t dino[] = { 0x1f, 0x3b, 0x3f, 0x3c, 0x7e, 0xfc, 0xa4, 0x36 }; //binary dino
const uint8_t hurdle[] = {0xF, 0x9, 0x9}; //binary hurdle

int (*ptr), i, j, speed, jval, dheight; // ptr only for UARTdrawdispm(); - DEBUGGING
				//speed of hurdle //jval is jump bool  //dheight pixel height of dino


void UARTdrawdispm(); //DEBUGGING
void dispmgen();
void dinogen(int,int);
void hurdlegen(int);
void hurdledel(int);
int jump(int); //temporarily bad
void game();

/* IF YOU DONT HAVE MY SD CARD LIBRARY ON UNCOMMENT THE BELOW*/
/*
void jUARTSEND (char*);

void jUARTSEND (char *string){
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *) string, len, 2000);
}
 */

void UARTdrawdispm(){
	for(i=0; i<rows; i++) {
	  if (i != 0) jUARTSEND("\n\r");
      for(j=0;j<cols;j++) {
    	  ptr = &dispm[i][j];
    	  jUARTSEND(ptr);
      }
   }
}
/*generate disp(m)lay origin*/
void dispmgen(){
  for(i=0; i<rows; i++) {
       for(j=0;j<cols;j++) {
          dispm[i][j] = ' '; // ' ' for debugging
       }
    }
}

void dinogen(int dheight, int cheight){
		int a[dsize];
		//dheight start pixel row - rows end pixel row - dsize width of image
		for (int di=dheight; di<rows; di++){ //begins 8 rows from bottom
			  for (j=0; j<dsize;j++){
				  //extract bits from dino to turn on and off pixel
				  // ie 0x1f is 00011111 so will look like '000#####'
				  int n=dino[(di+dsize-rows)];
				  for(i=0;n>0;i++){
					  a[i]=n%2;
					  n=n/2;
				  }
				  for(i=0;i<=dsize;i++) {
				  a[i] = ((a[i] != 1) ? ' ' : '1');
			  }
			  if (a[dsize-j-1] == '1') (dispm[di-cheight][j] = '#');
			  //-j-1 because i designed dino facing wrong way
		  }
	  }
}

void hurdlegen(int hurdleloc){
	  int hheight = rows-hsize;
	  int h[hsize];
	  for (int hi=hheight; hi<rows; hi++){ //begins 8 rows from bottom
		  for (j=0; j<hsize;j++){
			  //extract bits from dino to turn on and off pixel
			  // ie 0x1f is 00011111 so will look like '000#####'
			  int n=hurdle[(hi+hsize-rows)];
			  for(i=0;n>0;i++){
				  h[i]=n%2;
				  n=n/2;
			  }
			  for(i=0;i<=hsize;i++) {
				  h[i] = ((h[i] != 1) ? ' ' : '1');
			  }
			  if (hurdleloc < 4){
				  if (h[hsize-j-1] == '1') (dispm[hi+2][hurdleloc-j-1] = 'X');
			  }else{
				  if (h[hsize-j-1] == '1') (dispm[hi+1][hurdleloc-j-1] = 'X');
			  }
		  }
	  }
}
/* more efficient way for GPU but not CPU for changing matrix
 * only where the hurdle was last frame

void hurdledel(int hurdleloc){
	int hheight = rows-hsize;
			  int  h[hsize];
			  	  for (int hi=hheight; hi<rows; hi++){ //begins 8 rows from bottom
			  		  for (j=0; j<hsize;j++){
			  			  //extract bits from dino to turn on and off pixel
			  			  // ie 0x1f is 00011111 so will look like '000#####'
			  			  int n=hurdle[(hi+hsize-rows)];
			  			  for(i=0;n>0;i++){
			  				  h[i]=n%2;
			  				  n=n/2;
			  			  }
			  			  for(i=0;i<=hsize;i++) {
			  				  h[i] = ((h[i] != 1) ? ' ' : '1');
			  			  }
			  			  if (h[hsize-j-1] == '1' && hurdleloc != cols) (dispm[hi+1][hurdleloc-j] = ' ');
			  			  //-j-1 because i designed dino facing wrong way
			  		  }
			  	  }
}
*/
int jump(int cheight){
	if( jval == 1 ){
		if (cheight == 0) {return 12;}
		jval = 0;
	}
	if (cheight > 0) {return cheight-1;}
	return cheight;
}

void game(){
	/******jordans dinojump code***********/

	  dispmgen();
	  int hurdleloc=hurdlesloc;
	  dheight = rows-dsize;
	  int cheight = 0;
	  UARTdrawdispm();
	  int gameover = 0, speed = 200;
	  jval = 0;

	  while (gameover == 0){

			HAL_Delay (speed);
			if (hurdleloc == 11) jval = 1; //IMAGINE JUMP PRESSED HERE// current infin jump
			cheight = jump(cheight);
			/*LAZY WAY should probably use hurdledel and make a dinodel for it to be proper*/
			dispmgen();
			/* draw new hurdle */
			hurdlegen(hurdleloc);
			hurdleloc--;
			if (hurdleloc == -11) hurdleloc = 21;
			/*redraw dino*/
			dinogen(dheight, cheight);

			UARTdrawdispm();
			//hurdleloc 9 (maybe10) is collision
			if (((1<hurdleloc) && (hurdleloc < 11)) && (dheight-cheight >= 22)) {
			jUARTSEND("COLLISION");
			gameover = 1;
		}
    }
}



#endif /* INC_DINOHEADER_H_ */
