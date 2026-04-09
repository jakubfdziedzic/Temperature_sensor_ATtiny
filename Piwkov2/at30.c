#include <avr/io.h>
#include <stdbool.h>

float Fun_AT30_Read_Temperature (void){
	
	union {
		uint8_t
			Bytes[2];
		uint16_t
			Int16;
		} LocVar_Data;
		
		//pointer register for 5
		
	TWI0.MADDR = (0b1001111 << 1) + 1; //address + read
	while (!(TWI0.MSTATUS & TWI_RIF_bm));
	LocVar_Data.Bytes[1] = TWI0.MDATA;
	TWI0.MSTATUS |= TWI_RIF_bm;
	
	//transmission continue
	TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
	while (!(TWI0.MSTATUS & TWI_RIF_bm));
	LocVar_Data.Bytes[0] = TWI0.MDATA;
	TWI0.MSTATUS |= TWI_RIF_bm;
	
	//transmission end
	TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
	
	//temp update
	return ((float)LocVar_Data.Int16 /256.0);
}