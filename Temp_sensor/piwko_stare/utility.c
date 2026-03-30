#include <avr/io.h>
#include <stdbool.h>


void Fun_Util_PINInitialization(void)	{
	PORTB.DIRCLR=PIN5_bm;
	PORTB.PIN5CTRL=PORT_PULLUPEN_bm;
}


void Fun_util_LEDToggle (void){
	
	PORTB.OUTTGL = PIN4_bm;
}

void Fun_Util_PIT_Initialization(void){
	
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
	RTC.PITINTCTRL = RTC_PI_bm;
	RTC.PITCTRLA = (1<<RTC_PITEN_bp) | RTC_PERIOD_CYC32768_gc;
	
	
}

void Fun_Util_IntegerToStringSize4 (uint16_t Par_Integer, char Par_String[4]) {
	
	uint16_t
		LocVAR_UInt;
		
	if (Par_Integer > 9999) LocVAR_UInt = 9999;
	else LocVAR_UInt = Par_Integer;
	
	Par_String[0] = (LocVAR_UInt / 1000) + '0'; LocVAR_UInt %= 1000;
	Par_String[1] = (LocVAR_UInt / 100) + '0'; LocVAR_UInt %= 100;
	Par_String[2] = (LocVAR_UInt / 10) + '0'; LocVAR_UInt %= 10;
	Par_String[3] = LocVAR_UInt + '0';
}