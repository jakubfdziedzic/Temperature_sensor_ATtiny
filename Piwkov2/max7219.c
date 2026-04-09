#include <avr/io.h>
#include <stdbool.h>
#include "spi.h"

void Fun_MAX7219_Initialization (void) {
	
	uint8_t 
		LocVAR_Array[8];
		
	LocVAR_Array[0] = LocVAR_Array[2] = LocVAR_Array[4] = LocVAR_Array[6] = 0x09;
	LocVAR_Array[1] = LocVAR_Array[3] = LocVAR_Array[5] = LocVAR_Array[7] = 0x00;
	Fun_SPI_SendNudes(LocVAR_Array, 8);
	
	LocVAR_Array[0] = LocVAR_Array[2] = LocVAR_Array[4] = LocVAR_Array[6] = 0x0a;
	LocVAR_Array[1] = LocVAR_Array[3] = LocVAR_Array[5] = LocVAR_Array[7] = 0x08;
	Fun_SPI_SendNudes(LocVAR_Array, 8);
	
	LocVAR_Array[0] = LocVAR_Array[2] = LocVAR_Array[4] = LocVAR_Array[6] = 0x0b;
	LocVAR_Array[1] = LocVAR_Array[3] = LocVAR_Array[5] = LocVAR_Array[7] = 0x07;
	Fun_SPI_SendNudes(LocVAR_Array, 8);
	
	LocVAR_Array[0] = LocVAR_Array[2] = LocVAR_Array[4] = LocVAR_Array[6] = 0x0c;
	LocVAR_Array[1] = LocVAR_Array[3] = LocVAR_Array[5] = LocVAR_Array[7] = 0x01;
	Fun_SPI_SendNudes(LocVAR_Array, 8);
	
	
}

void Fun_MAX7219_clearscreen (void){
	uint8_t
		LocVAR_Array[8],
		LocVAR_WhichRow;
		
	for (LocVAR_WhichRow = 1;LocVAR_WhichRow < 9; LocVAR_WhichRow++){
	LocVAR_Array[0] = LocVAR_Array[2] = LocVAR_Array[4] = LocVAR_Array[6] = LocVAR_WhichRow;
	LocVAR_Array[1] = LocVAR_Array[3] = LocVAR_Array[5] = LocVAR_Array[7] = 0x00;
	Fun_SPI_SendNudes(LocVAR_Array, 8);
	}
	
	}
	
	
