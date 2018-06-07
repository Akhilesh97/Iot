#include<stdio.h>
#include "NUC1xx.h"
#include "LCD_Driver.h"
#include "Seven_Segment.h"
#include "Driver/DrvADC.h"
#include "Driver/DrvGPIO.h"
#include "Driver/DrvSYS.h"
#include "Driver/DrvGPIO.h"
/*
Potentiometer Nuvoton 
GND 						GND 
VCC 						VCC 
SIGNAL 					GPA6 //TURN THE POTENTIOMETER TO SEE THE CHANGE */
void sevensegment(int16_t value){
	int8_t digit;
	digit = value/1000;
	close_seven_segment();
	show_seven_segment(3,digit);
	DrvSYS_Delay(100);
	
	value = value - digit*1000;
	digit = value/100;
	close_seven_segment();
	show_seven_segment(2,digit);
	DrvSYS_Delay(100);
	
	value = value - digit*100;
	digit = value/10;
	close_seven_segment();
	show_seven_segment(1,digit);
	DrvSYS_Delay(100);
	
	value = value - digit*10;
	digit = value;
	close_seven_segment();
	show_seven_segment(0,digit);
}
int main(){
	//char *text;
	int32_t value ;
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	DrvADC_Open(ADC_SINGLE_END,ADC_SINGLE_OP , 0x01,INTERNAL_HCLK , 1);
	while(1){
		DrvADC_StartConvert();  
		while(DrvADC_IsConversionDone()==FALSE);
		value = ADC->ADDR[0].RSLT & 0xFFF;    
		sevensegment(value);
	}
}
