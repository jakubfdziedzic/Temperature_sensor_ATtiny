#include <avr/io.h>
#include <stdbool.h>

void Fun_SPI_Initialization(void) {
	
	PORTMUX.CTRLB = PORTMUX_SPI0_bm;
	PORTC.DIRSET = PIN3_bm | PIN2_bm | PIN0_bm | PIN4_bm;
	PORTC.OUTSET = PIN3_bm | PIN4_bm;
	
	SPI0.CTRLA = (0<<SPI_DORD_bp) |  (1<<SPI_MASTER_bp) | (0<<SPI_CLK2X_bp)
		| SPI_PRESC_DIV128_gc; 
	
	SPI0.CTRLB = (0<<SPI_BUFEN_bp) | (0<<SPI_BUFWR_bp) | (1<<SPI_SSD_bp) 
		| SPI_MODE_0_gc; 
				
		SPI0.CTRLA |= (1<<SPI_ENABLE_bp); 
	
}

void Fun_SPI_SendBytes (uint8_t * Par_BytesPtr, uint8_t Par_BytesNumber) {
	
	PORTC.OUTCLR = PIN3_bm | PIN4_bm;
	SPI0.INTFLAGS &= ~(SPI_IF_bm);
	while(Par_BytesNumber --) {
		SPI0.DATA = *Par_BytesPtr ++;
		while (!(SPI0.INTFLAGS & SPI_IF_bm));	
		
	}
	
	
	PORTC.OUTSET = PIN3_bm | PIN4_bm;
	
	
}