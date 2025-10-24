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

#define TOO_FAR 400
#define FOLLOW_DIST 1400
#define TOO_CLOSE 1900

enum robot_modes {INACTIVE, OBJECT_FOLLOWER, WALL_FOLLOWER};

uint16_t FrontSensor; // Front Sensor: E0
uint16_t LeftSensor; // Left Sensor: E1
uint16_t RightSensor; // Right Sensor: E2


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
	ADC1_SS1_Init();       // Initialize ADC1 Sample sequencer 1 
	PLL_Init();               // set system clock to 16 MHz
	PWM_Init();
  PortF_Init();          // inititlize switches and LEDs in SW_LED module
  EnableInterrupts();	
}

void object_follower(void)
{
	uint8_t i;
	
	//calibrate the sensor
	for(i =0; i <10;i++){
		//Read the value of each sensor and save 
		ADC1_In298(&LeftSensor,&FrontSensor,&RightSensor);
	}
	
	do{
		// Read ADC values for each sensor and save
		ADC1_In298(&LeftSensor,&FrontSensor,&RightSensor);

	}while((FrontSensor < TOO_CLOSE) || (FrontSensor < TOO_FAR));
	
	//Move Back if too close
	if(FrontSensor > TOO_CLOSE){
		Backward();
	}
	else if(FrontSensor < TOO_FAR && LeftSensor < TOO_FAR && RightSensor <TOO_FAR){
		Forward();
	}
	else{
		// Turn left 
		if((LeftSensor < FOLLOW_DIST) && (RightSensor > FOLLOW_DIST)){
			MoveLeft();
		}
		//turn right
		else if((RightSensor < FOLLOW_DIST) && (LeftSensor > FOLLOW_DIST)){
			MoveRight();
		}
		else if((LeftSensor > FOLLOW_DIST) && (LeftSensor < FOLLOW_DIST) && (RightSensor > FOLLOW_DIST) && (RightSensor < FOLLOW_DIST)){
								Stop();
						}
		else{
			Forward();
		}
		
	
}
	
	}


void wall_follower(void){
}

