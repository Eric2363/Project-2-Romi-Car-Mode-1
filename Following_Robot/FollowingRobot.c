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
#define LEFT_STOP       3081   // ~10 cm
#define LEFT_FOLLOW     1546   // ~20 cm

// ---- Front sensor ----
#define FRONT_STOP      3397   // ~10 cm
#define FRONT_FOLLOW    1716   // ~20 cm

// ---- Right sensor ----
#define RIGHT_STOP      2856   // ~10 cm
#define RIGHT_FOLLOW    1583   // ~20 cm


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


enum robot_modes mode=INACTIVE;

int main(void){	
	System_Init();
	
  while(1){

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
    uint16_t FrontMedian, LeftMedian, RightMedian;
    uint8_t i;

    // Calibrate ADC
    for(i=0;i<20;i++){
        ReadADCMedianFilter(&FrontMedian, &RightMedian, &LeftMedian);
    }

    // Wait until object appears in front (between 10–20 cm)
    do{
        ReadADCMedianFilter(&FrontMedian, &RightMedian, &LeftMedian);
    } while((FrontMedian > FRONT_STOP) || (FrontMedian < FRONT_FOLLOW));

    // Activate object-follow mode
    mode = OBJECT_FOLLOWER;

    // Main object-following loop
    while(mode == OBJECT_FOLLOWER){

        ReadADCMedianFilter(&FrontMedian, &RightMedian, &LeftMedian);
        uint16_t Front = FrontMedian;
        uint16_t Left  = LeftMedian;
        uint16_t Right = RightMedian;

        // Out-of-range check
        if (Front < 1000 && Left < 1000 && Right < 1000) {
            Stop();
            continue;
        }

        // Stop condition (<10cm)
        if (Front > FRONT_STOP) {
            Stop();
            mode = INACTIVE;   // go inactive
            break;              // exit loop, return to main
        }

        // Hold distance (10–20cm)
        else if (Front > FRONT_FOLLOW) {
            Stop();
        }

        // Move forward if object moves away (>20cm)
        else if (Front < FRONT_FOLLOW) {
            Forward();
        }

        // Steering adjustments
        if (Left > LEFT_FOLLOW && Right < RIGHT_FOLLOW) {
            MoveLeft();
        }
        else if (Right > RIGHT_FOLLOW && Left < LEFT_FOLLOW) {
            MoveRight();
        }
    }
}





void wall_follower(void){
}

void GPIOPortF_Handler(){
	
	// Switch 1 press: Set mode to object follower
	if(GPIO_PORTF_RIS_R & SW1){
		GPIO_PORTF_ICR_R = SW1;
		mode = OBJECT_FOLLOWER;
	}
	// Switch 0 press: Set mode to wall follower
	if(GPIO_PORTF_RIS_R & SW0){
		GPIO_PORTF_ICR_R = SW0;
		mode = WALL_FOLLOWER;
	}
}



