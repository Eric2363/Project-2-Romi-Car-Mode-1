#include "ADC1SS1.h"



void ADC1_SS1_Init(){
	
	//Clock setup
	SYSCTL_RCGCADC_R |= ADC1;// Enable ADC1
	while((SYSCTL_RCGCADC_R & ADC1) != ADC1);// Wait for ADC clock
	SYSCTL_RCGCGPIO_R |= PORTE; // Enable Port E
	while((SYSCTL_RCGCGPIO_R & PORTE) != PORTE); // wait for GPIO clock
	
	// Configure GPIO
	GPIO_PORTE_DIR_R &=~ SENSOR_PINS; // E0,E1,E2 input
	GPIO_PORTE_DEN_R &=~ SENSOR_PINS; // Disable digital
	GPIO_PORTE_AMSEL_R |= SENSOR_PINS;// Enable Analog mode
	GPIO_PORTE_AFSEL_R |= SENSOR_PINS;// Enable Alternate function
	
	// Configure ADC settings
	ADC1_ACTSS_R &=~ 0x0002;	//Disable SS1 during setup
	ADC1_PC_R &=~ 0xF; // Clear rate field
	ADC1_PC_R |= SAMPLE_RATE; // Set sample rate to 125k
	// Channel order setup
	// Sample 1: AIN3 (E0) Front Sensor
	// Sample 2: AIN2 (E1) Left Sensor
	// Sample 3: AIN1 (E2) Right Sensor
	ADC1_SSMUX1_R = (ADC1_SSMUX1_R & ~0x00000FFF) + (Ain3<<0) + (Ain2<<4) + (Ain1<<8);
	ADC1_SSCTL1_R = 0x0600;// SS1: Sample 3 is last
	ADC1_SSPRI_R = 0x3201;		// Sequencer 1 high priority
	ADC1_EMUX_R &=~ 0x00F0;		// Software trigger for SS1
	ADC1_IM_R &=~ 0x0002; // Disable SS1 interrupts
	ADC1_ACTSS_R |= 0x0002; // Enable SS1
}

// Reads three samples from ADC1 SS1: AIN3(PE0), AIN2(PE1), AIN1(PE2)
void ADC1_In321(uint16_t *ain3, uint16_t *ain2, uint16_t *ain1){
  ADC1_PSSI_R = 0x0002;                 // 1) initiate SS1
  while((ADC1_RIS_R & 0x02) == 0){};    // 2) wait for conversion done

  *ain3 = ADC1_SSFIFO1_R & 0xFFF;       // 3) first result (AIN3 / PE0)
  *ain2 = ADC1_SSFIFO1_R & 0xFFF;       // 4) second result (AIN2 / PE1)
  *ain1 = ADC1_SSFIFO1_R & 0xFFF;       // 5) third result (AIN1 / PE2)

  ADC1_ISC_R = 0x0002;                  // 6) acknowledge completion
}


//Get the median
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3){
  uint16_t result;
  if(u1 > u2)
    if(u2 > u3)       result = u2;
    else if(u1 > u3)  result = u3;
    else              result = u1;
  else
    if(u3 > u2)       result = u2;
    else if(u1 > u3)  result = u1;
    else              result = u3;
  return result;
}


void ReadADCMedianFilter(uint16_t *m3, uint16_t *m2, uint16_t *m1){
  static uint16_t oldest3=0, middle3=0; // Front (AIN3)
  static uint16_t oldest2=0, middle2=0; // Left  (AIN2)
  static uint16_t oldest1=0, middle1=0; // Right (AIN1)
  uint16_t r3, r2, r1;

  ADC1_In321(&r3, &r2, &r1);

  *m3 = median(r3, middle3, oldest3);
  oldest3 = middle3; middle3 = r3;

  *m2 = median(r2, middle2, oldest2);
  oldest2 = middle2; middle2 = r2;

  *m1 = median(r1, middle1, oldest1);
  oldest1 = middle1; middle1 = r1;
}

