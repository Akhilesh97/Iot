#include<stdio.h>
#include "NUC1xx.h"
#include "DrvGPIO.h"
#include "DrvSYS.h"
#define d360 512
#define d180 512/2
#define d90 512/4
#define d45 512/8
#define d2 51
/*
//STEPPER MOTOR 
Stepper motor: Nuvoton board 
IN1 GPA3 
IN2 GPA2 
IN3 GPA1 
IN4 GPA0 
GND GND 
VCC VCC(5 OR 12)(DONT CONNECT TO VCC 3.3) 
*/
char cw[8] = {0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};
char ccw[8] = {0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
void cw_m(uint16_t deg){
	int i,j;
	for(i = 0;i < (deg);i++){
		for(j = 0;j < 8; j++){
			GPIOA->DOUT = cw[j];
		}
	}
}
void ccw_m(uint16_t deg){
	int i,j;
	for(i = 0;i < (deg);i++){
		for(j = 0;j < 8; j++){
			GPIOA->DOUT = ccw[j];
		}
	}
}
int main(){
	cw_m(d2/2);
	ccw_m(d2/2);
}