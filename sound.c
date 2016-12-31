#include "tm4c123gh6pm.h"
#include "sound.h"

unsigned long Index = 0;
const unsigned char *Wave;
unsigned long Counter = 0;
	
// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){
	unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0x0F;      // no analog
  GPIO_PORTB_PCTL_R &= ~0x0000FFFF; // regular GPIO function
  GPIO_PORTB_DIR_R |= 0x0F;      // make PB3-0 out
  GPIO_PORTB_AFSEL_R &= ~0x0F;   // disable alt funct on PB3-0
  GPIO_PORTB_DEN_R |= 0x0F;      // enable digital I/O on PB3-0
	GPIO_PORTB_DR8R_R |= 0x0F;		 // drive up to 8mA out
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;
}

// **************Timer2_Init*********************
// 
// Input: none
// Output: none
// period of 7272 = 11 kHz
void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
}

void Sound_Init(void){
  DAC_Init();               // initialize simple 4-bit DAC
  Timer2_Init(80000000/11025);     // 11.025 kHz
  Index = 0;
  Counter = 0;
}

void Timer2A_Stop(void){ 
  TIMER2_CTL_R &= ~0x00000001; // disable
}
	
void Timer2A_Start(void){ 
  TIMER2_CTL_R |= 0x00000001;   // enable
}

void Timer2A_Handler(void){ 
TIMER2_ICR_R = 0x00000001;  // acknowledge
  if(Counter){
    DAC_Out(Wave[Index]>>4); 
    Index = Index + 1;
    Counter = Counter - 1;
  }else{
    Timer2A_Stop();
  }
}

void Sound_Play(const unsigned char *pt, unsigned long count){
  Wave = pt;
  Index = 0;
  Counter = count;
  Timer2A_Start();
}

//void Sound_Shoot(void){
//  Sound_Play(shoot,4080);
//}