#include<stdio.h>
#include "NUC1xx.h"
#include "LCD_Driver.h"
/*
NO POTENTIOMETER REQUIRED SHORTER END OF LED TO GPA12 
ON NUVOTON LONGER END OF LED TO VCC ON NUVOTON (SHORTER END OF THE LED IS THE ONE WITHOUT THE RESISTOR) 
Here,automatically once the external LED is connected ,intensity increases and decreases. 
*/
void InitPWM(void)
{
 	/* Step 1. GPIO initial */ 
	SYS->GPAMFP.PWM0_AD13=1;
				
	/* Step 2. Enable and Select PWM clock source*/		
	SYSCLK->APBCLK.PWM01_EN = 1;//Enable PWM clock
	SYSCLK->CLKSEL1.PWM01_S = 3;//Select 22.1184Mhz for PWM clock source

	PWMA->PPR.CP01=1;			//Prescaler 0~255, Setting 0 to stop output clock
	PWMA->CSR.CSR0=0;			// PWM clock = clock source/(Prescaler + 1)/divider
				         
	/* Step 3. Select PWM Operation mode */
	//PWM0
	PWMA->PCR.CH0MOD=1;			//0:One-shot mode, 1:Auto-load mode
	//CNR and CMR will be auto-cleared after setting CH0MOD form 0 to 1.

	PWMA->CNR0=0xFFFF;
	PWMA->CMR0=0xFFFF;

	PWMA->PCR.CH0INV=0;			//Inverter->0:off, 1:on
	PWMA->PCR.CH0EN=1;			//PWM function->0:Disable, 1:Enable
 	PWMA->POE.PWM0=1;			//Output to pin->0:Diasble, 1:Enable
}
void delay(int value){
	while(value--){}
	}
int main(){
	int val=0000;
	UNLOCKREG();
	SYSCLK->PWRCON.XTL12M_EN = 1;
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();
	InitPWM();
	while(1){
		PWMA->CMR0=val;
		delay(200);
		val++;
	}
}