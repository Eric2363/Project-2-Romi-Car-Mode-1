/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: Enable the general purpose timer 1A to produce a 10us delay.
Timer 1A Config: 16bit mode, One shot, down counter
*/

#include "tm4c123gh6pm.h"
#include <stdint.h>


// Timer1 Setup
#define TIMER1 0x02              // Timer 1 bit
#define MODE_16bit 0x00000004    //16 bit mode 
#define PERIODIC_MODE 0x00000002 // Periodic mode


// Timer A : 10us delay
//====================================================
#define TIMER1A_MASK 0x00000001
#define MAX_RELOAD 0xFFFF
#define PRESCALE 0


void Timer1_Init(void);    //Initialize Timer1
void Timer1A_Init(void);   //Initialize Timer1A for periodic cowntdown mode
void gpio_delay(uint32_t ticks, uint32_t pvalue);  //create function that takes load and prescale values to create a delay