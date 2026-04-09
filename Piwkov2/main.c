#ifndef F_CPU
#define F_CPU 3333333UL
#endif

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "asm.h"
#include "spi.h"
#include "utility.h"
#include "max7219.h"
#include "twi.h"
#include "at30.h"
#include "adc.h"
#include "WaitTicks.h"
#include "display.h"


volatile uint8_t  VAR_Flag_Preceed = false;
volatile uint16_t VAR_ADC_Resoult  = 0;
float             VAR_Temperature  = 0.0f;


int main(void) {
	Fun_Util_PinInitialization();
	Fun_ASM_LedInitialization();
	Fun_ASM_ClockInitialization();
	Fun_Util_PIT_Initialization();
	Fun_ADC_Initialization();
	Fun_SPI_Initialization();
	Fun_MAX7219_Initialization();
	Fun_MAX7219_clearscreen();
	Fun_TWI_Initialization();

	sei();

	Fun_ADC_StartMeasuremementt();

	while (true) {
		Fun_Util_LedToggle();

		VAR_Temperature = Fun_AT30_Read_Temperature();
		AnimateTemperatureWithUnit(VAR_Temperature);

		if (VAR_ADC_Resoult < 100) {
			AnimateSun();
			} else {
			AnimateMoon();
		}

		Fun_ADC_StartMeasuremementt();
	}
}

ISR(RTC_PIT_vect) {
	RTC.PITINTFLAGS = RTC_PI_bm;
	VAR_Flag_Preceed = true;
}

ISR(ADC0_RESRDY_vect) {
	VAR_ADC_Resoult = ADC0.RES / 64;
}