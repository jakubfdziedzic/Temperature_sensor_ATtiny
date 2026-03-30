#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "asm.h"
#include "utility.h"
#include "spi.h"
#include "max7219.h"
#include "usart.h"
#include "twi.h"
#include "at30.h"

#define DEFCMD_IDN			"*IDN?"
#define DEFSTR_AnswerIDN	"We're EiT"
#define DEFSTR_AnswerUnkownCmd "Unknown command"

volatile uint8_t
	VAR_Flag_Proceed = false,
	VAR_Flag_ByteReceived = false,
	VAR_Received_Data = '?';
	
uint16_t
	VAR_Timer = 0;
char
	VAR_String[4];
	
volatile uint8_t
	VAR_ReceivedBuforIndex = 0,
	VAR_Flag_AnalizeCommand = false,
	VAR_ReceiverBufor[32],
	VAR_TransmitterBufer[32],
	VAR_TransmitterBuferIndex = 0,
	VAR_LenghtOfAnswer = 0;
	
float
	VAR_Temperature;
	
void Fun_SendAnswerIDN (void) {
	
	VAR_TransmitterBuferIndex = 0;
	VAR_LenghtOfAnswer = 0;
	
	do {
		VAR_TransmitterBufer[VAR_LenghtOfAnswer] = *((uint8_t*)&DEFSTR_AnswerIDN + VAR_LenghtOfAnswer);
	} while (++VAR_LenghtOfAnswer < (sizeof(DEFSTR_AnswerIDN)));
	VAR_TransmitterBufer[VAR_LenghtOfAnswer] = ';';
	USART0.TXDATAL = ':';
	USART0.CTRLA |= (1<<USART_DREIE_bp);
}	

void Fun_SendAnswerUnknownCmd (void) {
		
	VAR_TransmitterBuferIndex = 0;
	VAR_LenghtOfAnswer = 0;
		
	do {
		VAR_TransmitterBufer[VAR_LenghtOfAnswer] = *((uint8_t*)&DEFSTR_AnswerUnkownCmd + VAR_LenghtOfAnswer);
	} while (++VAR_LenghtOfAnswer < (sizeof(DEFSTR_AnswerUnkownCmd)));
	VAR_TransmitterBufer[VAR_LenghtOfAnswer++] = ';';
	USART0.TXDATAL = ':';
	USART0.CTRLA |= (1<<USART_DREIE_bp);
}
	
		
	

int main(void)
{
	uint8_t
		LocVAR_ComparisionResult,
		LocVAR_ComparisionIndex;

	Fun_Util_PINInitialization();
	Fun_ASM_LedInitialization ();
	Fun_ASM_ClockInitialization ();
	Fun_Util_PIT_Initialization();
	Fun_SPI_Initialization();
	FUN_MAX7219_Initialization();
	Fun_MAX7219_Clear_Screen ();
	Fun_USART_Initialization();
	Fun_TWI_Initialization();
	
	sei ();
	
    while (true) {

		if (VAR_Flag_Proceed) {
		
			Fun_util_LEDToggle ();
			//VAR_Temperature = Fun_AT30_ReadTemperature();
			Fun_Util_IntegerToStringSize4(VAR_Timer++, VAR_String);
			Fun_MAX7219_DisplayString (VAR_String);
			VAR_Flag_Proceed = false;
		}
	
	
		if (VAR_Flag_AnalizeCommand){
			// command "*IDN?"
			if (VAR_ReceivedBuforIndex == (sizeof(DEFCMD_IDN)-1)) {
					LocVAR_ComparisionResult = true;
					LocVAR_ComparisionIndex = 0;
					do {
						if (VAR_ReceiverBufor [LocVAR_ComparisionIndex] != *((uint8_t*)&DEFCMD_IDN + LocVAR_ComparisionIndex))
						LocVAR_ComparisionResult = false;
					} while (LocVAR_ComparisionResult && (++LocVAR_ComparisionIndex < (sizeof(DEFCMD_IDN)-1)));
					if (LocVAR_ComparisionResult) {
						Fun_SendAnswerIDN ();
					} else Fun_SendAnswerUnknownCmd ();
			} else Fun_SendAnswerUnknownCmd ();
			VAR_Flag_AnalizeCommand = false;
		}
	}
}

ISR (RTC_PIT_vect){
	
	RTC.PITINTFLAGS = RTC_PI_bm;
	VAR_Flag_Proceed = true;
	
}

ISR (USART0_RXC_vect) {
	
	uint8_t
		LocVAR_ReceivedData;
	
	LocVAR_ReceivedData = USART0.RXDATAL;
	
	if (LocVAR_ReceivedData == ':'){
		// start of frame
		VAR_ReceivedBuforIndex = 0;
	} else{
		if(LocVAR_ReceivedData == ';'){
			// end of frame
			VAR_Flag_AnalizeCommand = true;
		} else{
			// content of frame
			if (VAR_ReceivedBuforIndex < (sizeof(VAR_ReceiverBufor)-1)){
				VAR_ReceiverBufor[VAR_ReceivedBuforIndex++] = LocVAR_ReceivedData;
				
			} else{
				VAR_ReceivedBuforIndex = 0;
			}
		}
		
	}
	
	//VAR_Flag_ByteReceived = true;
	//VAR_Received_Data = USART0.RXDATAL;
}

ISR (USART0_DRE_vect){
	
	if (VAR_TransmitterBuferIndex < VAR_LenghtOfAnswer) {
		USART0.TXDATAL = VAR_TransmitterBufer[VAR_TransmitterBuferIndex++];
	} else {
		USART0.CTRLA &= ~(1<<USART_DREIE_bp);
	}
	
}

