#define __SFR_OFFSET	0
#include <avr/io.h>

.global Fun_ASM_LedInitialization,Fun_ASM_ClockInitialization

Fun_ASM_LedInitialization:

	sbi 0x04,4
	cbi 0x05,4

	ret


Fun_ASM_ClockInitialization:
	
	push R30
	push R31
	push R24
	push R25

	// address of CLKCTRL 0x0060 - R31:R30 -> Z-pointer register
	LDI R30, 0x60
	LDI R31, 0x00

	// configuration change protection CCp 0x0034, signature for I/O registers 0xD8
	LDI R24, 0xD8

	// settings for MCLKCTRLA
	LDI R25, (0<<7) | (0x00<<0)  //disable CLKOUT,  high freq

	// writing
	OUT 0x34, R24

	// writing to MCLKCTRLA base 0x0060 + shift 0x00
	STD Z+0, R25

	//settings for mclkctrlb
	LDI R25, (0<<0) //disable prescaler division
	OUT 0x34, r24 // writing protection signature
	STD Z+1, r25


	pop R25
	pop R24
	pop R31
	pop R30

	ret