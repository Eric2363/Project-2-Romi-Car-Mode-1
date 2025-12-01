/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: Object following & Wall following robot.
*/

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS1.h"  
#include "PWM.h"
#include "PortF.h"
#include "PLL.h"


// ---- Left sensor ----
#define LEFT_FOLLOW     1800   // ~20 cm

// ---- Front sensor ----
#define FRONT_STOP      4000   // ~10 cm
#define FRONT_FOLLOW    1800   // ~20 cm

// ---- Right sensor ----
#define RIGHT_FOLLOW    1800   // ~20 cm

#define MAXDIST         1000   // max distance before robot should stop
#define WALL 1500
#define FWALL 2700
#define PIVOTSTOP 1300  //reading in the front to stop pivoting
#define TURNSTOP 2200   //reading to stop turning once the inside sensor finds the wall again 
#define WALLDISTFOLLOW 2900 // optimal reading for wall following at this distance ~15cm
enum robot_modes {INACTIVE, OBJECT_FOLLOWER, WALL_FOLLOWER};

// Function prototypes
// external functions
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);


volatile enum robot_modes mode=INACTIVE;

int main(void){	
	System_Init();
	
  while(1){

		switch (mode) {
			case OBJECT_FOLLOWER:
				GPIO_PORTF_DATA_R &=~ LEDS;
				GPIO_PORTF_DATA_R |= BLUE;
				object_follower();
			  break;
			case WALL_FOLLOWER:
				GPIO_PORTF_DATA_R &=~ LEDS;
				GPIO_PORTF_DATA_R |= GREEN;
				wall_follower();
			  break;
			default:
				GPIO_PORTF_DATA_R &=~ LEDS;
				GPIO_PORTF_DATA_R |= RED;
				Stop();
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
		uint8_t i;
		uint16_t front, left, right;
		//calibrate sensors
	for(i=0;i<20;i++){
        ReadADCMedianFilter321(&front, &left, &right);
    }
    
		// TEMP: raw single-channel reads every loop
    mode = OBJECT_FOLLOWER;
		
		//ADC1_FilterConfig(32); 

    while (mode == OBJECT_FOLLOWER) {
        // Read each channel       
				ReadADCMedianFilter321(&front, &left, &right);

				
			  // go inactive once the object is within ~6cm
				if (front > FRONT_STOP) {
            Stop();
            mode = INACTIVE;
            break;
        }

        // bias toward the side that reads stronger
        if (left > LEFT_FOLLOW && right < RIGHT_FOLLOW && left > MAXDIST ) {
            // Object on left side -> turn left
            MoveLeft();
            continue;
        } else if (right > RIGHT_FOLLOW && left < LEFT_FOLLOW && right > MAXDIST) {
            // Object on right side -> turn right
            MoveRight();
            continue;
        }

        // control by front distance
        if (front < FRONT_FOLLOW && front > MAXDIST) {
            // Maintain Distance go forward
            Forward();
        } else if (front > FRONT_FOLLOW && front < FRONT_STOP) {
            // Maintain distance go backward
            Backward();
        } else {
            //no target -> stop
            Stop();
					for(int j=0;j<5000;j++){
       
    }
        }
    }
}



void wall_follower(void){
	uint8_t i;
	uint16_t front, left, right;
	
	for(i=0;i<20;i++){
        ReadADCMedianFilter321(&front, &left, &right);
    }
	
	uint16_t firstleft  = left; //keep track of last reading to make the next turn once there is no wall to follow
	uint16_t firstright = right;
	uint16_t nextturn;
		
	mode = WALL_FOLLOWER;
	
	
	while (mode == WALL_FOLLOWER) {								
		  ReadADCMedianFilter321(&front, &left, &right);
		
		if (front >= FWALL){ //if the front sensor detects an objec

			if (right >= WALL && left < WALL){ //check if the right is sensing a wall to turn left
			
				do{PivotLeft(); ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;}
				while(front > PIVOTSTOP);
			}
			else if (left >= WALL && right < WALL){ //check if the left is sensing a wall to turn right
			
				do{PivotRight(); ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;}
				while (front > PIVOTSTOP);
			}
			else if (front > 3100) //move backwares until the front sensor is no longer in front of a wall
			{do {SlowBackward(); ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;}
			
			while(front > 2300);  //stop moving backwards once far enough from wall
			  
			} //set next turn in backwards and front movements
			
		}
		//all additions are used to make sure there is enough difference between the sensor readings to determine a direction
		else if (front < FWALL){  //if there is no obstacle ahead
			if      (right > (left + 1000) && (right < WALLDISTFOLLOW)){Forward(); ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;} //set next turn just as a saftey to decide turn along with first variables
			else if (left > (right + 1000) && (left < WALLDISTFOLLOW)){Forward(); ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;}
			else if (right > (left + 800) && (right > WALLDISTFOLLOW)){SlightLeft();ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;} 
			else if (left > (right + 800) && (left > WALLDISTFOLLOW)){SlightRight();ReadADCMedianFilter321(&front, &left, &right); firstright = right; firstleft  = left;}
			

			
			else if (firstright > firstleft + 180) //make sure there is a large enough difference between the two sensors
			{
				do{MoveRight(); ReadADCMedianFilter321(&front, &left, &right);} // turn until right senses A WALL
				while (right < TURNSTOP); 
			
			
			}
			else if (firstleft > firstright + 400) //make sure there is a large enough difference between the two sensors
			{
				do {MoveLeft(); ReadADCMedianFilter321(&front, &left, &right);} // turn until left senses A WALL
				while (left < TURNSTOP);
			
			}
		}	
	} 
}






void GPIOPortF_Handler(void){
    uint32_t s = GPIO_PORTF_RIS_R;              // latched edges
    GPIO_PORTF_ICR_R = s & (SW1 | SW2);         // clear flags we handle

    // SW1: activate/deactivate
    if (s & SW1){
        if (mode == INACTIVE){
            mode = OBJECT_FOLLOWER;             // default to mode 1 on activation
        } else {
            mode = INACTIVE;                    // deactivate
        }
    }

    // SW2: toggle modes only when active
    if (s & SW2){
        if (mode != INACTIVE){
            mode = (mode == OBJECT_FOLLOWER) ? WALL_FOLLOWER : OBJECT_FOLLOWER;
        }
    }
}