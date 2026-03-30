#include <avr/io.h>
#include <stdbool.h>
#include "spi.h"
#include <avr/pgmspace.h>

//vcc -> PWR (3)
//gnd -> PWR (2)
//din -> Ext1 (16)
//cs -> Ext (15)
//clk -> Ext (18)

const PROGMEM uint8_t DigitsFonts[][8]={
	{0x0e, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0e, 0x00}, //0
	{0x04, 0x0c, 0x14, 0x04, 0x04, 0x04, 0x1f, 0x00}, //1
	{0x0e, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1f, 0x00}, //2
	{0x1F, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0e, 0x00}, //3
	{0x02, 0x06, 0x0a, 0x12, 0x1f, 0x02, 0x02, 0x00}, //4
	{0x1f, 0x10, 0x1e, 0x01, 0x01, 0x11, 0x0e, 0x00}, //5
	{0x06, 0x08, 0x10, 0x1e, 0x11, 0x11, 0x0e, 0x00}, //6
	{0x1f, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08, 0x00}, //7
	{0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e, 0x00}, //8
	{0x0e, 0x11, 0x11, 0x0f, 0x01, 0x02, 0x0c, 0x00}, //9
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  //blank
};

void FUN_MAX7219_Initialization(void){
	uint8_t
		LocVAR_Array[8];
		//disable BCD,enable matrix
		LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=0x09;
		LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x00;
		Fun_SPI_SendBytes(LocVAR_Array,8);
		
		
		//illumination
		LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=0x0a;
		LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x08;
		Fun_SPI_SendBytes(LocVAR_Array,8);
		
		
		//row number
		LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=0x0b;
		LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x07;
		Fun_SPI_SendBytes(LocVAR_Array,8);
		
		
		//normal operation
		LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=0x0c;
		LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x01;
		Fun_SPI_SendBytes(LocVAR_Array,8);
		
		
		//test
		LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=0x0f;
		LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x00;
		Fun_SPI_SendBytes(LocVAR_Array,8);
		
		
}

void Fun_MAX7219_Clear_Screen (void) {
	uint8_t
		LocVAR_Array[8],
		LocVAR_WhichRow;
		
		for(LocVAR_WhichRow = 1; LocVAR_WhichRow < 9; LocVAR_WhichRow ++ ){
			LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=LocVAR_WhichRow;
			LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x55;
			Fun_SPI_SendBytes(LocVAR_Array,8);
		}
		
		//disable BCD,enable matrix
		LocVAR_Array[0]=LocVAR_Array[2]=LocVAR_Array[4]=LocVAR_Array[6]=0x09;
		LocVAR_Array[1]=LocVAR_Array[3]=LocVAR_Array[5]=LocVAR_Array[7]=0x00;
		Fun_SPI_SendBytes(LocVAR_Array,8);
}

void Fun_MAX7219_DisplayString (char Par_String[4]) {
	
	uint8_t
		LocVAR_Array[8],
		LocVAR_IndexMatrixRow,
		LocVAR_StringChar;
		
	for (LocVAR_IndexMatrixRow = 0; LocVAR_IndexMatrixRow < 8; LocVAR_IndexMatrixRow++) {
		LocVAR_Array[0] = LocVAR_Array[2] = LocVAR_Array[4] = LocVAR_Array[6] = LocVAR_IndexMatrixRow + 1;
		for (LocVAR_StringChar = 0; LocVAR_StringChar < 4; LocVAR_StringChar++){
			if ((Par_String[LocVAR_StringChar] >= '0') && (Par_String[LocVAR_StringChar] <= '9')){
				LocVAR_Array[(LocVAR_StringChar<<1)+1] = pgm_read_byte (&(DigitsFonts[Par_String[LocVAR_StringChar]-'0'][LocVAR_IndexMatrixRow]));	
				
			} else{
				LocVAR_Array[(LocVAR_StringChar<<1)+1] = pgm_read_byte (&(DigitsFonts[Par_String[10]][LocVAR_IndexMatrixRow]));
				
			}
			
		}
	
		Fun_SPI_SendBytes(LocVAR_Array, 8);	
	}
	}