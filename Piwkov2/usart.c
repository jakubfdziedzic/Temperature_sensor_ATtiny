#define F_CPU 20000000L
#define DEFVAL_RS_BaudRate 9600L

#include <avr/io.h>
#include <stdbool.h>



void Fun_USART_Initialization (void){
	
	PORTB.DIRSET = PIN2_bm;
	USART0.CTRLA = (1 << USART_RXCIE_bp)    | (0 << USART_TXCIE_bp) 
				 | (0 << USART_DREIE_bp)    | (0 << USART_RXSIE_bp)
				 | (0 << USART_LBME_bp) 	| (0 << USART_ABEIE_bp)
				 | (0x00 << USART_RS4850_bp);
				 
	USART0.CTRLB =  (1 << USART_RXEN_bp)	| (1 << USART_TXEN_bp)
				 |	(0 << USART_SFDEN_bp)	| (1 << USART_ODME_bp)
				 |	USART_RXMODE_NORMAL_gc  | ( 0 << USART_MPCM_bp);
				 
	USART0.CTRLC =  USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc
				 |  USART_SBMODE_1BIT_gc		| USART_CHSIZE_8BIT_gc;
				 
				 
	USART0.BAUD = (uint16_t)((64L*F_CPU)/(16L*DEFVAL_RS_BaudRate));
	
			 
				 
	
	
	
	
	
}

