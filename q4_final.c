#include<stdio.h>
#include "NUC1xx.h"
#include "Driver/DrvSYS.h"
#include "LCD_Driver.h"
#include "DrvADC.h"
/*
Potentiometer Nuvoton 
GND 						GND 
VCC 						VCC 
SIGNAL 					GPA6 //TURN THE POTENTIOMETER TO SEE THE CHANGE */
int main(){
	uint16_t value;
	char *text;
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	Initial_panel();
	clr_all_panel();
	DrvADC_Open(ADC_SINGLE_END,ADC_SINGLE_OP , 0x40,INTERNAL_HCLK , 1);
	while(1){
		DrvADC_StartConvert();   // start A/D conversion
		while(DrvADC_IsConversionDone()==FALSE);
		value = ADC->ADDR[6].RSLT & 0xFFF;    
		sprintf(text,"Value:%f",((value)*3.3)/4095);
		print_lcd(0,text);
	}
}
