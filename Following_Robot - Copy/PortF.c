
/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: PortF switch 1 & 0 configuration
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
	
	GPIO_PORTF_DIR_R &=~ SWITCHES; // Set PortF F4 sw1 & SW0 as input
	GPIO_PORTF_DEN_R |= SWITCHES; // Enable SW1 & SW0
	GPIO_PORTF_AMSEL_R &=~ SWITCHES;// Disable Analog Mode
	GPIO_PORTF_AFSEL_R &=~ SWITCHES;// Disable Alternate function mode
	GPIO_PORTF_PUR_R |= SWITCHES;
	
	// Interupt SW1 and SW0
		
	GPIO_PORTF_IS_R &=~ SWITCHES; // Edge interupt
	GPIO_PORTF_IBE_R &=~ SWITCHES; // Noth both edges
	GPIO_PORTF_IEV_R &=~ SWITCHES; // Falling edge
	GPIO_PORTF_ICR_R = SWITCHES; // Clear and previouse interupt flags
	GPIO_PORTF_IM_R |= SWITCHES;// Arm interupt
		
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | LEVEL1;	// Set priority level 1
	NVIC_EN0_R |= 0x40000000; // Enable PortF Handler interupt
	
	
	
	
		
	
}