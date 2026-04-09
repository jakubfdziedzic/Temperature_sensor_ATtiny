#include <avr/io.h>
#include <stdbool.h>
#include "WaitTicks.h"

extern volatile uint8_t VAR_Flag_Preceed;

void WaitTicks(uint16_t ticks) {
	for (uint16_t i = 0; i < ticks; i++) {
		while (!VAR_Flag_Preceed);   
		VAR_Flag_Preceed = false;   
	}
}