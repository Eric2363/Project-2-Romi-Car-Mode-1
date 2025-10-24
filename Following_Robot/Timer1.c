/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: Enable the general purpose timer 1A to produce a delay.
Timer 1A Config: 16bit mode, periodic, down counter
*/

#include "Timer1.h"

void Timer1_Init(){
	SYSCTL_RCGCTIMER_R |= TIMER1;      	           // Activate TIMER1
	while((SYSCTL_RCGCTIMER_R & TIMER1) != TIMER1);// wait to until timer1 is active
	TIMER1_CFG_R = MODE_16bit;                     //configure for 16 bit mode
	Timer1A_Init();                                //initialize for countdown mode
}
//Timer 1 Setup
//===========================================================================
void Timer1A_Init(void){
	

  TIMER1_CTL_R &=~ TIMER1A_MASK;      // Disable TIMER1 A during setup
  TIMER1_TAMR_R = PERIODIC_MODE;      // Periodic Mode, down-counter
  TIMER1_TAILR_R = MAX_RELOAD;        // Reload value (16-bit max)
  TIMER1_TAPR_R = PRESCALE;           // prescale = 0. Withing resoultion of 16 bits.
	TIMER1_ICR_R = TIMER1A_MASK;				// Clear TIMER1A timeout flag

}
// 
void gpio_delay(uint32_t ticks, uint32_t pvalue){
    
		TIMER1_TAILR_R = ticks - 1;				//load Timer1A with ticks and subtract 1
	  TIMER1_TAPR_R = pvalue;           //load prescale for longer delays
    TIMER1_CTL_R |= TIMER1A_MASK;     // Start timer

    while(TIMER1_TAR_R != 0); // Wait for timer to run out
}