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
	// SS1: sample0 = AIN3(PE0)=Front, sample1 = AIN2(PE1)=Left, sample2 = AIN1(PE2)=Right
	ADC1_SSMUX1_R = (3<<0) | (2<<4) | (1<<8);
	ADC1_SSCTL1_R = 0x0600;  // END on 3rd sample (IE2|END2)

	ADC1_SSCTL1_R = 0x0600;// SS1: Sample 3 is last
	ADC1_SSPRI_R = 0x3201;		// Sequencer 1 high priority
	ADC1_EMUX_R &=~ 0x00F0;		// Software trigger for SS1
	ADC1_IM_R &=~ 0x0002; // Disable SS1 interrupts
	ADC1_ACTSS_R |= 0x0002; // Enable SS1
	

}

// read three channels from ADC1 SS1 in order
// sample0 = AIN3 (PE0)  -> *ain3
// sample1 = AIN2 (PE1)  -> *ain2
// sample2 = AIN1 (PE2)  -> *ain1
void ADC1_In321(uint16_t *ain3, uint16_t *ain2, uint16_t *ain1){
  ADC1_PSSI_R = 0x0002;                // 1) initiate SS1
  while((ADC1_RIS_R & 0x0002) == 0){}  // 2) wait for conversion complete
  *ain3 = ADC1_SSFIFO1_R & 0x0FFF;     // 3A) read first result  (AIN3, PE0)
  *ain2 = ADC1_SSFIFO1_R & 0x0FFF;     // 3B) read second result (AIN2, PE1)
  *ain1 = ADC1_SSFIFO1_R & 0x0FFF;     // 3C) read third result  (AIN1, PE2)
  ADC1_ISC_R  = 0x0002;                // 4) acknowledge completion
}

//median function
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3){
  uint16_t result;
  if(u1 > u2)
    if(u2 > u3)       result = u2;   // u1>u2>u3
    else if(u1 > u3)  result = u3;   // u1>u3>u2
    else              result = u1;   // u3>u1>u2
  else
    if(u3 > u2)       result = u2;   // u3>u2>u1
    else if(u1 > u3)  result = u1;   // u2>u1>u3
    else              result = u3;   // u2>u3>u1
  return result;
}

// y(n) = median(x(n), x(n-1), x(n-2))   for each channel
// Uses ADC1_In321(&r3,&r2,&r1) r3=AIN3(PE0)=Front, r2=AIN2(PE1)=Left, r1=AIN1(PE2)=Right
void ReadADCMedianFilter321(uint16_t *front, uint16_t *left, uint16_t *right){
  //            x(n-2)        x(n-1)
  static uint16_t f_old=0, f_mid=0;   // Front history
  static uint16_t l_old=0, l_mid=0;   // Left  history
  static uint16_t r_old=0, r_mid=0;   // Right history

  //            x(n)
  uint16_t r3_now, r2_now, r1_now;

  // One SS1 burst: PE0->r3 (Front), PE1->r2 (Left), PE2->r1 (Right)
  ADC1_In321(&r3_now, &r2_now, &r1_now);

  // Median-of-3 per channel
  *front = median(r3_now, f_mid, f_old);
  *left  = median(r2_now, l_mid, l_old);
  *right = median(r1_now, r_mid, r_old);

  // Shift histories
  f_old = f_mid; f_mid = r3_now;
  l_old = l_mid; l_mid = r2_now;
  r_old = r_mid; r_mid = r1_now;
}



