//===================Headers===================================
#include "tm4c123gh6pm.h"
#include <stdint.h>
//================Periperhal Setup=============================
#define ADC1 0x00000002 // Enable ADC1
#define PORTE 0x10 // Enabale PORT E Clock

//=====================PINS====================================
#define SENSOR_PINS 0x07 // Sensor input:E0(Front),E1(Left),E2(Right)

#define SAMPLE_RATE 0x01 // 125ksps


//===================Functions=================================
void ADC1_SS1_Init(void);
void ADC1_In298(uint16_t *ain2, uint16_t *ain3, uint16_t *ain1);
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3);