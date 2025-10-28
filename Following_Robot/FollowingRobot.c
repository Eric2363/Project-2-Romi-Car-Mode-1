// FollowingRobot.c
// Runs TM4C123
// Starter program CECS 347 project 2 - A Follwoing Robot
// by Min He, 03/17/2024

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS1.h"  
#include "PWM.h"
#include "PortF.h"
#include "PLL.h"

#define TOO_FAR 1100
#define FOLLOW_DIST 1675
#define TOO_CLOSE 1850
#define OUT_OF_RANGE 500

enum robot_modes {INACTIVE, OBJECT_FOLLOWER, WALL_FOLLOWER};

uint16_t FrontMedian, LeftMedian,RightMedian;

// Function prototypes
// external functions
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);

enum robot_modes mode=INACTIVE;

int main(void){	
	System_Init();
	
  while(1){
		object_follower();
		switch (mode) {
			case OBJECT_FOLLOWER:
				object_follower();
			  break;
			case WALL_FOLLOWER:
				wall_follower();
			  break;
			default:
				WaitForInterrupt();
			  break;				
		}
  }
}

void System_Init(void){
	DisableInterrupts();
	PLL_Init();	               // set system clock to 16 MHz
	ADC1_SS1_Init();       // Initialize ADC1 Sample sequencer 1 
	PWM_Init();
  PortF_Init();          // inititlize switches and LEDs in SW_LED module
  EnableInterrupts();	
}

void object_follower(void)
{
 
	uint8_t i;
	for(i=0;i<10;i++){
				ReadADCMedianFilter(&FrontMedian,&LeftMedian,&RightMedian);
	}
	
	while(1){
		ReadADCMedianFilter(&FrontMedian,&LeftMedian,&RightMedian);
		
		uint16_t Front = FrontMedian;
		uint16_t Left = LeftMedian;
		uint16_t Right = RightMedian;
		

		
	if (Front >= TOO_CLOSE) {
			// Too close ? RED LED
			GPIO_PORTF_DATA_R = 0x02; // RED
			Backward();
	}
	else if (Front > FOLLOW_DIST) {
			// In range (not too close, not too far) ? GREEN LED
			GPIO_PORTF_DATA_R = 0x08; // GREEN
			Stop();
	}
	else if (Front <= TOO_FAR) {
			// Too far ? BLUE LED
			GPIO_PORTF_DATA_R = 0x04; // BLUE
			Forward();
	}
	else {
			// Optional: Between FOLLOW_DIST and TOO_CLOSE range
			GPIO_PORTF_DATA_R = 0x0A; // maybe yellowish (red+green)
	}


		int done = 0;
	}
	
}



void wall_follower(void){
}

