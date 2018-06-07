#include<stdio.h>
#include "NUC1xx.h"
#include "DrvGPIO.h"
#include "DrvSYS.h"
#include "LCD_Driver.h"
#include "DrvADC.h"
/*
//Ldr  has gnd,vcc,signal next to each other  
LDR Nuvoton 
GND GND 
VCC VCC 
SIGNAL GPA6 
//note gpa6:0040(set in code) 
//Procedure:Cover the LDR ,THE READING SHOWS LESS THAN 4K AND LED IS OFF. //WHEN WE REMOVE OUR HANDS FROM LDR,LED TURNS ON
*/
void InitADC(void)
{
	/* Step 1. GPIO initial */ 
	GPIOA->OFFD|=0x00400000; 	//Disable digital input path
	SYS->GPAMFP.ADC7_SS21_AD6=1; 		//Set ADC function 
				
	/* Step 2. Enable and Select ADC clock source, and then enable ADC module */          
	SYSCLK->CLKSEL1.ADC_S = 2;	//Select 22Mhz for ADC
	SYSCLK->CLKDIV.ADC_N = 1;	//ADC clock source = 22Mhz/2 =11Mhz;
	SYSCLK->APBCLK.ADC_EN = 1;	//Enable clock source
	ADC->ADCR.ADEN = 1;			//Enable ADC module

	/* Step 3. Select Operation mode */
	ADC->ADCR.DIFFEN = 0;     	//single end input
	ADC->ADCR.ADMD   = 0;     	//single mode
		
	/* Step 4. Select ADC channel */
	ADC->ADCHER.CHEN = 0x40;
	
	/* Step 5. Enable ADC interrupt */
	ADC->ADSR.ADF =1;     		//clear the A/D interrupt flags for safe 
	ADC->ADCR.ADIE = 1;
	//NVIC_EnableIRQ(ADC_IRQn);
	
	/* Step 6. Enable WDT module */
	ADC->ADCR.ADST=1;
}
void delay(int count){
	while(count--){}
	}
int main(){
	char text[16] = "ADC Value:";
	UNLOCKREG();
	SYSCLK->PWRCON.XTL12M_EN = 1; 	//Enable 12Mhz and set HCLK->12Mhz
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();
	InitADC();
	Initial_panel();
	clr_all_panel();
	DrvGPIO_Open(E_GPC,15,E_IO_OUTPUT);
	while(1){
		while(ADC->ADSR.ADF==0);	 // ADC Flag, wait till 1 (A/DC conversion done)
		ADC->ADSR.ADF=1;	 // write 1 to ADF is to clear the flag
		ADC->ADDR[7].RSLT;
		sprintf(text+10,"%d",ADC->ADDR[6].RSLT);
		if(ADC->ADDR[6].RSLT > 4000)
			DrvGPIO_ClrBit(E_GPC,15);
		else
			DrvGPIO_SetBit(E_GPC,15);
		print_lcd(1,text);
		DrvSYS_Delay(40000);
		ADC->ADCR.ADST=1;
	}
}
