#include <avr/io.h>
#include <stdbool.h>

volatile uint16_t lightValue = 0;

void Fun_ADC_Initialization (void){
	
ADC0.CTRLA = (0 << ADC_RUNSTBY_bp) | ADC_RESSEL_10BIT_gc | (0 << ADC_FREERUN_bp) | (1 << ADC_ENABLE_bp);
ADC0.CTRLB = ADC_SAMPNUM_ACC64_gc;
ADC0.CTRLC = (1 << ADC_SAMPCAP_bp) | ADC_REFSEL_VDDREF_gc | ADC_PRESC_DIV256_gc;
ADC0.CTRLD = ADC_INITDLY_DLY256_gc | (1 << ADC_ASDV_bp) | 7;
ADC0.CTRLE = ADC_WINCM_NONE_gc;
ADC0.SAMPCTRL = 0;
ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
ADC0.INTCTRL = (0 << ADC_WCMP_bp) | (1 << ADC_RESRDY_bp);


}
void Fun_ADC_StartMeasuremementt (void){
	ADC0.COMMAND = (1 << ADC_STCONV_bp);
	
}
