#include<stdio.h>
#include "NUC1xx.h"
#include "Driver/DrvGPIO.h"
#include "Driver/DrvSYS.h"
#include "Driver/DrvUART.h"
#include "LCD_Driver.h"
/*
Switch Nuvoton 
GND 		GND 
VCC 		VCC 
SIGNAL GPA15 //Press switch,led glows. 
*/
void GPIOAB_Int_CallBack(){
	DrvGPIO_ClrBit(E_GPC,15);
	DrvSYS_Delay(3000);
	DrvGPIO_SetBit(E_GPC,15);
	DrvSYS_Delay(4000);
}
int main(){
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	DrvGPIO_Open(E_GPA,15,E_IO_INPUT);
	DrvGPIO_Open(E_GPC,15,E_IO_OUTPUT);
	DrvGPIO_EnableInt(E_GPA,15,E_IO_RISING,E_MODE_EDGE);
	DrvGPIO_SetDebounceTime(5,1);
	DrvGPIO_EnableDebounce(E_GPA,15);
	DrvGPIO_SetIntCallback(GPIOAB_Int_CallBack,NULL);
	while(1){}
	}