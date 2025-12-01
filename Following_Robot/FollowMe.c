//// FollowMe.c
//// Runs on TM4C123
//// This the starter test program for the object follower that implements basic features: 
//// follow an object forward and backward, stop when the object is out of range.
//// By Min He
//// March 12, 2024

//// This is an example program to show how to use hardware PWM on TM4C123.
//#include <stdint.h>
//#include "PWM.h"
//#include "ADC0SS3.h"
//#include "PLL.h"

//// move the following constant definitions to ADC0SS3.h
//#define TOO_FAR 		(400)  // replace the zero with the ADC output value for maximum distance
//#define FOLLOW_DIST (1400)  // replace the zero with the ADC output value for object following distance
//#define TOO_CLOSE 	(3000)  // replace the zero with the ADC output value for minimum distance

//void follow_me(void);

//int main(void){	
//	PLL_Init();									// 16MHz system clock
//	ADC0_Seq3_Ch9_Init();       // ADC initialization PE4/AIN9
//	PWM_Init();              // initialize PB32: left direction pins, PB76: right direction pins
//															// PB4:Left wheel, PB5:Right wheel
//	while(1){
//	  Stop();
//		follow_me();
//  }
//}

//// Implement a simple algorithm to follow an object to move forward/backward/stop.
//void follow_me(void) {
//	uint8_t i;
//  volatile uint16_t ADCvalue;
//	
//	// Calibrate the sensor
//	for (i=0;i<10;i++) {
//    ADCvalue = ReadADCMedianFilter();  // read one value
//	} 
//	
//  // wait until an obstacle is in the right distant range.
// 	do {
//			ADCvalue = ReadADCMedianFilter();  // read one value
//	} while ((ADCvalue>TOO_CLOSE) || (ADCvalue<TOO_FAR));
//		
//  while ((ADCvalue<TOO_CLOSE) && (ADCvalue>TOO_FAR)) {								
//		ADCvalue = ReadADCMedianFilter();
//		
//		if (ADCvalue>FOLLOW_DIST) { // negative logic: too close, move back
//			Backward();
//		}
//		else if (ADCvalue<FOLLOW_DIST){ // negative logic: too far, move forward
//			Forward();
//		}
//		else { // right distance, stop
//			Stop();
//		}
//  }	
//}
