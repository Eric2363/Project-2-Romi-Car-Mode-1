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


// ---- Left sensor ----
#define LEFT_TOO_CLOSE   3081   // ~10 cm
#define LEFT_FOLLOW      2314   // mid between 10–20 cm
#define LEFT_TOO_FAR     1546   // ~20 cm

// ---- Front sensor ----
#define FRONT_TOO_CLOSE  3397
#define FRONT_FOLLOW     2556
#define FRONT_TOO_FAR    1716

// ---- Right sensor ----
#define RIGHT_TOO_CLOSE  2856
#define RIGHT_FOLLOW     2220
#define RIGHT_TOO_FAR    1583


enum robot_modes {INACTIVE, OBJECT_FOLLOWER, WALL_FOLLOWER};

typedef enum{
	
	SENSOR_LEFT = 0,
	SENSOR_FRONT = 1,
	SENSOR_RIGHT = 2
}sensor_id_t;


// Function prototypes
// external functions
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);
void BigSensor(void);

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

//// Implement a simple algorithm to follow an object 
//// to move forward/backward/stop/turn.
void object_follower(void)
{
 
	// Hold sensor  values
	uint16_t FrontMedian, LeftMedian,RightMedian;
	
	//calibrate ADC
	uint8_t i;
	for(i=0;i<20;i++){
	ReadADCMedianFilter(&FrontMedian,&RightMedian,&LeftMedian);
	}
	
	// wait for a object
	do{
		ReadADCMedianFilter(&FrontMedian,&RightMedian,&LeftMedian);
	}while((FrontMedian > FRONT_TOO_CLOSE)||(FrontMedian < FRONT_TOO_FAR));
	
	//Object following loop
	while(1){
		
		//Get current distance value for each sensor
		ReadADCMedianFilter(&FrontMedian,&RightMedian,&LeftMedian);
		
		//Watchwindow view
		uint16_t Front = FrontMedian;
		uint16_t Left = LeftMedian;
		uint16_t Right = RightMedian;
		
		
		if(FrontMedian > FRONT_TOO_CLOSE){
			GPIO_PORTF_DATA_R = RED;
		}
		else if(LeftMedian > LEFT_TOO_CLOSE){
			GPIO_PORTF_DATA_R = BLUE;
		}
		else if(RightMedian > RIGHT_TOO_CLOSE){
			GPIO_PORTF_DATA_R = GREEN;
		}
	
		
		int done = 0;
	}
	
}




void wall_follower(void){
}



