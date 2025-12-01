//===================Headers===================================
#include "tm4c123gh6pm.h"
#include <stdint.h>
//================Periperhal Setup=============================
#define ADC1 0x00000002 // Enable ADC1
#define PORTE 0x10 // Enabale PORT E Clock

//=====================PINS====================================
//E0: Front : Ain3 | E1: Left: Ain2 | E2: Right: Ain1
#define SENSOR_PINS 0x07 
#define SAMPLE_RATE 0x01 // 125ksps
#define Ain1 1
#define Ain2 2
#define Ain3 3

//===================Functions=================================
void ADC1_SS1_Init(void);
void ADC1_In321(uint16_t *ain3,uint16_t *ain2, uint16_t *ain1);
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3);
void ReadADCMedianFilter(uint16_t *m3, uint16_t *m2, uint16_t *m1);
