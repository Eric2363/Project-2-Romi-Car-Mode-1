/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: PortB configuration: Motors: B4 & B5 as output and PWM Alternate function. Left DIR : B3,B2, output, Right DIR: B6,B7, output, will function as normal GPIO.
*/
#include "PWM.h"


void PWM_Init(){
	
	SYSCTL_RCGCPWM_R |= 0x01;
	SYSCTL_RCGCGPIO_R |= PORTB; // Enable Port B Clk
	while((SYSCTL_RCGCGPIO_R & PORTB) != PORTB){ // Wait for PortB clk to activate
		
	}
	
		// DIR Confiuration
	GPIO_PORTB_DIR_R |= DIR_MASK; // Set B7 & B3 as output
	GPIO_PORTB_DEN_R |= DIR_MASK;// Enable B7 & B3
	GPIO_PORTB_AMSEL_R &=~ DIR_MASK;// Disable Analog mode
	GPIO_PORTB_AFSEL_R &=~ DIR_MASK;// Disable ALternate function mode
	
	// MOTOR Configuration
	GPIO_PORTB_DIR_R |= MOTORS; // Set B4 & B5 as output
	GPIO_PORTB_DEN_R |= MOTORS; // Enable B4 & B5
	GPIO_PORTB_AMSEL_R &=~ MOTORS;// Disable Analog mode
	GPIO_PORTB_AFSEL_R |= MOTORS; // Enable Alternate function mode
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & (~0x00FF0000) ) | PCTL_PWM_CODE; // Set B4 & B5 with PWM function
			
	//PWM Configuration
	PWM0_1_CTL_R = 0; // Stop generator 1
	PWM0_1_GENA_R = 0xC8; // B4
	PWM0_1_GENB_R = 0xC08; // B5
	PWM0_1_LOAD_R = 16000 - 1;
	PWM0_1_CMPA_R = 0; // duty B4
	PWM0_1_CMPB_R = 0;// duty B5
	PWM0_1_CTL_R |= 0x01; // enable geneator 1
	PWM0_ENABLE_R |= 0x0C; // enable PWM2, PWM3

	
}
void Duty(unsigned long duty_L, unsigned long duty_R){
	PWM0_1_CMPA_R = duty_L -1; // B4 count value when output rises
	PWM0_1_CMPB_R = duty_R - 1; // B5 count value when output rises
}

void Forward(){
	
	GPIO_PORTB_DATA_R |= FORWARD;
	Duty(DUTY50,DUTY50);
}

void Backward(){
	GPIO_PORTB_DATA_R &=~FORWARD;
	Duty(DUTY50,DUTY50);
}
void MoveLeft(){
	GPIO_PORTB_DATA_R |= FORWARD;
	Duty(DUTY30,DUTY20);
}
void MoveRight(){
	GPIO_PORTB_DATA_R |= FORWARD;
	Duty(DUTY20,DUTY30);
}
void Stop(){
	Duty(OFF,OFF);
}

	