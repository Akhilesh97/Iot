#include<stdio.h>
#include "NUC1xx.h"
#include "LCD_Driver.h"
#include "DrvRTC.h"
static uint8_t Alarm_E=1;
void InitRTC(void)
{
	UNLOCKREG();		
	/* Step 1. Enable and Select RTC clock source */     
	SYSCLK->PWRCON.XTL32K_EN = 1;//Enable 32Khz for RTC clock source
	SYSCLK->APBCLK.RTC_EN =1;//Enable RTC clock source	

	/* Step 2. Initiate and unlock RTC module */
	 
	START_RTC();

	/* Step 3. Initiate Time and Calendar  setting */
	RTC->TSSR.HR24_HR12     =1;//Set 24hour mode
	//Set time and calendar for loading register, Calendar„³ 09/1/19, Time„³ 13:20:00
	set_CLR(0,9,0,1,1,9);
	set_TLR(1,3,2,0,0,0);

	/* Step 4. Set alarm interrupt */
	//Set time and calendar for alarm register , Calendar„³ 09/1/19, Time„³ 13:30:00
	set_CAR(0,9,0,1,1,9);
	set_TAR(1,3,2,0,2,0);		
	//Enable alarm interrupt
	RTC->RIER.AIER = 1;
	RTC->RIER.TIER = 1;
	NVIC_EnableIRQ(RTC_IRQn);
}
void RTC_IRQHandler(void)
{
	uint32_t clock,calender;
	char adc_value[15]="Clock:";
	char adc_value_cl[15]="Calender:";
	/* tick */
	if(inpw(&RTC->RIIR)&0x2)
 	{
		clock=inpw(&RTC->TLR)&0xFFFFFF;
		sprintf(adc_value+6,"%02x",(clock>>16)&0xFF);
		sprintf(adc_value+9,"%02x",((clock)>>8)&0xFF);
		sprintf(adc_value+12,"%02x",clock&0xFF);
		adc_value[8]=':';
		adc_value[11]=':';
		print_lcd(0, adc_value);
		clock=inpw(&RTC->CLR)&0xFFFFFF;
		sprintf(adc_value_cl+6,"%02x",(calender>>16)&0xFF);
		sprintf(adc_value_cl+9,"%02x",((calender)>>8)&0xFF);
		sprintf(adc_value_cl+12,"%02x",calender&0xFF);
		adc_value_cl[8]=':';
		adc_value_cl[11]=':';
		print_lcd(1, adc_value_cl);
		outpw(&RTC->RIIR,2);
	}
	/* alarm */
	if(inpw(&RTC->RIIR)&0x1) 
	{
		print_lcd(1, "Alarm!!!!");
		GPIOC->DOUT&=0xFF;
		Alarm_E=0;
		outpw(&RTC->RIIR,1);
	}
}
int32_t main (void)
{
	UNLOCKREG();
	SYSCLK->PWRCON.XTL32K_EN = 1;//Enable 32Khz for RTC clock source
	SYSCLK->PWRCON.XTL12M_EN = 1;
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();

	Initial_panel();  //call initial pannel function
	clr_all_panel();
	                        
	InitRTC();
	
		 	
	/* Synch field transmission & Request Identifier Field transmission*/
	while(Alarm_E)
	{
		UNLOCKREG();
		WDT->WTCR.WTR =1;//Write 1 to clear for safety
		LOCKREG();	
	}
	while(1)
	{
		__NOP();
	}
}
/*PROCEDURE: 1)FOR ADD FILES TO SOURCE GROUP ON THE LEFT HAND SIDE : 
a)Go to SRC>DRIVER>DrvRtc.h add it 
b)Go to Include>Driver>DrvRtc.h(make sure extension for it is all files and not .c) 
c)Over write the code in DrvRtc.c and DrvRTC.h with the code given (they will give rtc.zip)
d)Remove DrvRTC.h from source group 
ALSO GO TO LAST PART OF DRVRTC.C AND REMOVE FUNCTION DRVINITRTC()
e)Load the prog and run */