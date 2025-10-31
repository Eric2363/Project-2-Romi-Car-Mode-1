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
#define FOLLOW_DIST 2000
#define TOO_CLOSE 3045
#define OUT_OF_RANGE 500

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
	ReadADCMedianFilter(&FrontMedian,&LeftMedian,&RightMedian);
	}
	
	// wait for a object
	do{
		ReadADCMedianFilter(&FrontMedian,&LeftMedian,&RightMedian);
	}while((FrontMedian > TOO_CLOSE)||(FrontMedian < TOO_FAR));
	
	//Object following loop
	while(1){
		
		//Get current distance value for each sensor
		ReadADCMedianFilter(&FrontMedian,&LeftMedian,&RightMedian);
		
		//Watchwindow view
		uint16_t Front = FrontMedian;
		uint16_t Left = LeftMedian;
		uint16_t Right = RightMedian;
		
	
		//Distance Control with Front Sensor
		if(FrontMedian > FOLLOW_DIST){
			Backward(); // Move backwards
			
		
		}
		else if(FrontMedian < FOLLOW_DIST){
			//Too far so move forward
			Forward();
			
		}
		else{
			// Perfect Distance
			Stop();
			
		}
		//======================Direction Control: Left & Right Sensor===============================
		
		if((LeftMedian > TOO_CLOSE)){
			//Object is closer to right sensor so turn right
//			if(LefttMedian > FOLLOW_DIST){
//				
//				Backward();
//				
//			}
//			else if(FrontMedian < FOLLOW_DIST){
//				Forward();
//			}
			
			BackLeft();
		
			GPIO_PORTF_DATA_R = RED;
		}
//		else if((RightMedian  > TOO_CLOSE)){
//			//Object is closer to left sensor so turn left
//			BackRight();

//			GPIO_PORTF_DATA_R = GREEN;
//		}
		
		int done = 0;
	}
	
}




void wall_follower(void){
}

sensor_id_t BigSensor(uint16_t Left, uint16_t Front, uint16_t Right){

	ReadADCMedianFilter(&FrontMedian,&LeftMedian,&RightMedian);
	
	sensor_id_t owner = SENSOR_LEFT;
	uint16_t best = Lm;
	
	if( Fm >= best){
		best = Fm;
		owner = SENSOR_FONT;
	}
	
}

