#include<stdio.h>
#include "NUC1xx.h"
#include "Driver/DrvSYS.h"
#include "Driver/DrvGPIO.h"
#include "LCD_Driver.h"
int main(){
	char *text;
	int32_t a;
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	Initial_panel();
	clr_all_panel();
	while(1){
		DrvGPIO_SetPortBits(E_GPA,0);
		a = DrvGPIO_GetPortBits(E_GPA);
		sprintf(text, "Port NO:%d",a);//sprintf(text, "Port NO:%x",a-128)
		print_lcd(0,text);
	}}
		