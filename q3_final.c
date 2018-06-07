#include<stdio.h>
#include "Driver/DrvGPIO.h"
#include "Driver/DrvSYS.h"
#include "NUC1xx.h"
#include "LCD_Driver.h"
//GPA 15 to GND thenGPA15 to VCC repeat 
volatile uint32_t irqACounter = 0;
void GPIOAB_Int_CallBack(uint32_t GPIOA_Status,uint32_t GPIOB_Status){
		if((GPIOA_Status >>15)&&0x11)
			irqACounter++;
	}
int main(){
	char *text;
	char *TEXT;
	int32_t a;
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	DrvGPIO_Open(E_GPA,15,E_IO_INPUT);
	DrvGPIO_EnableInt(E_GPA,15,E_IO_RISING,E_MODE_EDGE);
	DrvGPIO_SetDebounceTime(5,1);
	DrvGPIO_EnableDebounce(E_GPA,15);
	DrvGPIO_SetIntCallback(GPIOAB_Int_CallBack,NULL);
	Initial_panel();
	clr_all_panel();
	while(1){
		DrvGPIO_SetPortBits(E_GPA,15);
		a = DrvGPIO_GetPortBits(E_GPA);
		sprintf(text,"Port NO;%d",a);
		print_lcd(1,text);
		sprintf(TEXT,"Counter:%d",irqACounter);
		print_lcd(2,TEXT);
	}
}