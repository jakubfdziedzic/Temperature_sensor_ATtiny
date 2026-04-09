#include <avr/io.h>
#include <stdbool.h>


void Fun_Util_PinInitialization (void){
	
	PORTB.DIRCLR = PIN5_bm;
	PORTB.PIN5CTRL = PORT_PULLUPEN_bm;
}

void Fun_Util_LedToggle (void){
	
	PORTB.OUTTGL = PIN4_bm;
	
}

void Fun_Util_PIT_Initialization (void) {
	
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
	RTC.PITINTCTRL = RTC_PI_bm;
	RTC.PITCTRLA = (1<<RTC_PITEN_bp) | RTC_PERIOD_CYC32_gc; //CYC32768
}