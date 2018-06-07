#include<stdio.h>
#include "NUC1xx.h"
#include "Driver/DrvSYS.h"
#include "Driver/DrvGPIO.h"
#include "Driver/DrvUART.h"
/*For buzzer connect gpb15 to gnd and then remove. For led connect gpb14 to gnd and then remove and repeat. */
void EINT1CallBack(void){
	DrvGPIO_ClrBit(E_GPB,15);
	DrvSYS_Delay(30000);
	DrvGPIO_SetBit(E_GPB,15);
	DrvSYS_Delay(30000);
}
void EINT0CallBack(void){
	
	DrvGPIO_ClrBit(E_GPA,11);
	DrvSYS_Delay(30000);
	DrvGPIO_SetBit(E_GPA,11);
	DrvSYS_Delay(300000);
}
int main(){
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	DrvGPIO_Open(E_GPB,11,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPA,12,E_IO_OUTPUT);
	DrvGPIO_Open(E_GPB,4,E_IO_INPUT);
	DrvGPIO_EnableEINT1(E_IO_BOTH_EDGE,E_MODE_EDGE,EINT1CallBack);
	DrvGPIO_Open(E_GPB,15,E_IO_INPUT);
	DrvGPIO_EnableEINT0(E_IO_BOTH_EDGE,E_MODE_EDGE,EINT0CallBack);
	while(1){}
	}
