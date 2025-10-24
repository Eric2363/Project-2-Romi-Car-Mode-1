
/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: PortF switch 1 configuration
*/

#include "PortF.h"

void PortF_Init(){

	SYSCTL_RCGCGPIO_R |= PORTF;	//Enable Clk for PortF
	while((SYSCTL_RCGCGPIO_R & PORTF) != PORTF){} // Wait for Clk to turn on

	GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock Port F
	GPIO_PORTF_CR_R |= 0x1F;        // allow changes to PF4–PF0

		
	
	GPIO_PORTF_DIR_R |= LEDS; // Set PortF LEds as output
	GPIO_PORTF_DEN_R |= LEDS; // Enable LEDS
	GPIO_PORTF_AMSEL_R &=~ LEDS; // Disable Analog Mode
	GPIO_PORTF_AFSEL_R &=~ LEDS; // Disable Alternate Function mode
	
	GPIO_PORTF_DIR_R &=~ SW1; // Set PortF F4 sw1 as input
	GPIO_PORTF_DEN_R |= SW1; // Enable SW1
	GPIO_PORTF_AMSEL_R &=~ SW1;// Disable Analog Mode
	GPIO_PORTF_AFSEL_R &=~ SW1;// Disable Alternate function mode
	GPIO_PORTF_PUR_R |= SW1;
	
	// Interupt SW1
		
	GPIO_PORTF_IS_R &=~ SW1; // Edge interupt
	GPIO_PORTF_IBE_R &=~ SW1; // Noth both edges
	GPIO_PORTF_IEV_R &=~ SW1; // Falling edge
	GPIO_PORTF_ICR_R = SW1; // Clear and previouse interupt flags
	GPIO_PORTF_IM_R |= SW1;// Arm interupt
		
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | LEVEL1;	// Set priority level 1
	NVIC_EN0_R |= 0x40000000; // Enable PortF Handler interupt
	
	
	
	
		
	
}