/*
Group:4
Eric Santana
Hector Polanco
Kero Samaan
Mason Doan
ORG: CSULB
Class: CECS 347 Embedded System II
--------------------------
Description: PortF switch 1 configuration
*/

#include "tm4c123gh6pm.h"


#define PORTF 0x20

// Interupt Level
#define LEVEL0 0x00000000
#define LEVEL1 0x00200000

// Port F LEDs for debugging
#define LEDS 0x0E
#define RED 0x02
#define BLUE 0x04
#define PURPLE 0x06
#define GREEN 0x08
#define YELLOW 0x0A
#define TEAL 0x0C
#define WHITE 0x0E


#define SW1 0x10

void PortF_Init(void);
#include "tm4c123gh6pm.h"

void PortF_Init(void);
