// flashinglights.c
// Runs on LM4F120/TM4C123
// Matthew Dias
// Jan 1, 2017

// ******* Required Hardware I/O connections*******************
// SW1 connected to PE0 (associated with Blue LED)
// SW2 connected to PE1 (associated with Green LED)
// SW3 connected to PE2 (associated with Red LED)
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// Blue LED on PB4
// Green LED on PB5
// Red LED on PB6

#include "tm4c123gh6pm.h"
#include "random.h"
#include "blink.h"
#include "sound.h"

//declare functions
//void Systick_Init(unsigned long period);	//initialize systick and pass in period to be used for reload value
void Delay1ms(unsigned long msec);
void LED_Init(void);		//initialize LEDs of PB6-PB4
void Blink_Red(void);		// turn on red LED
void Blink_Green(void);	// turn on green LED
void Blink_Blue(void);	// turn on blue LED
void Next_Level_Lights(void);	// flash all lights
void Game_Over_Lights(void);		// turn all lights on
void Game_Complete_Lights(void);	// flash all lights twice then delay

//values passed into SysTick_Init to determine frequency LEDs will blink at for each level
unsigned long levels[10] = 
 {500, 400, 300, 200, 100, 80, 60, 40, 20, 10};  // calculate correct values for this
unsigned int curLevel = 0;
unsigned int curLight;
unsigned int randomnum;


//------------Switch_Init------------
// Initialize the three switches used to choose an LED on  PE2-PE0
// Input: none
// Output: none
void Switch_Init(void){ 
	unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
	GPIO_PORTE_PCTL_R &= ~0x00000FFF; // 2) regular GPIO function
  GPIO_PORTE_DIR_R &= ~0x07;      // 3) make PE2-PE0 input
  GPIO_PORTE_AFSEL_R &= ~0x07;     // 4) disable alternate function on PE2-PE0
  GPIO_PORTE_DEN_R |= 0x07;      // 5) enable digital I/O on PE2-PE0
  GPIO_PORTE_AMSEL_R &= ~0x07;     // 6) disable analog function on PE2-PE0
	GPIO_PORTE_IS_R &= ~0x07;     // 7) PE2-PE0 is edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x07;    // 8)  PE2-PE0 is not both edges
  GPIO_PORTE_IEV_R |= 0x07;    // 9)  PE2-PE0 rising edge event
  GPIO_PORTE_ICR_R = 0x07;      // 10) clear flags 2,1,0
  GPIO_PORTE_IM_R |= 0x07;      // 11) arm interrupt on PE2-PE0
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF0F)|0x40; // 12) priority 2
  NVIC_EN0_R = 0x10;      // 13) enable interrupt 4 in NVIC
}

void Next_Level(void) {
	Next_Level_Lights();
}

void Game_Over(void) {
	Game_Over_Lights();
}

void Game_Complete(void) {
	Game_Complete_Lights();
}

void GPIOPortE_Handler(void){
	unsigned long switchPressed = GPIO_PORTE_DATA_R;
	GPIO_PORTE_ICR_R = 0x07; // ack, clear interrupt flag 2,1,0
	curLight = (GPIO_PORTB_DATA_R&0x70)>>4;
	if(curLight == switchPressed) {
		curLevel += 1;
		Next_Level();
	} else {
		Game_Over();
		curLevel = 0;
	}
	randomnum += 5;		// increase randomness of flashing by allowing user switch input to change random number
	GPIO_PORTE_DATA_R = 0;
}

int main(void) {
	Switch_Init();
	LED_Init();
	//Sound_Init();
	
	while(curLevel <= 10) {
		randomnum = (Random32()>>24)%60; // a number from 0 to 59
		if(randomnum < 20) {
			Blink_Red();
			curLight = 1; //red light on
			Delay1ms(levels[curLevel]);
		} else if(randomnum >=20 && randomnum < 40) {
			Blink_Green();
			curLight = 2; //green light on
			Delay1ms(levels[curLevel]);
		} else {
			Blink_Blue();
			curLight = 3; //blue light on
			Delay1ms(levels[curLevel]);
		}
	}
	Game_Complete();
}
