#include <avr/io.h>
#include <stdbool.h>

float Fun_AT30_ReadTemperature (void) {
	
	union {
		uint8_t
			Bytes[2];
		int16_t
			Int16;	
	} LocVAR_Data;
	
	TWI0.MADDR = (0b1001111 << 1) + 1; // address + read
	while (!(TWI0.MSTATUS & TWI_RIF_bm));
	LocVAR_Data.Bytes[1] = TWI0.MDATA;
	TWI0.MSTATUS |= TWI_RIF_bm;
	//continuation of transmission
	TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc | TWI_ACKACT_ACK_gc;
	while (!(TWI0.MSTATUS & TWI_RIF_bm)); 
	LocVAR_Data.Bytes[0] = TWI0.MDATA;
	TWI0.MSTATUS |= TWI_RIF_bm;
	//stop transmission
	TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_STOP_gc;
	
	//temperature update
	return ((float)LocVAR_Data.Int16 / 256.0);
	
	
}