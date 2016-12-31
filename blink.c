#include "tm4c123gh6pm.h"
#include "random.h"

// **************Delay1ms*********************
// Input: none
// Output: none
void Delay1ms(unsigned long msec){
	unsigned long i;
  while(msec > 0){
    i = 13333;  // approximately 1 ms at 80 MHz (1ms/12.5ns/6)
    while(i > 0){
      i = i - 1;
    }
    msec = msec - 1;
  }
}

//------------LED_Init------------
// Initialize the three LEDs on on PB6-PB4.
// Red LED connected to PB6
// Green LED connected to PB5
// Blue LED connected to PB4
// Input: none
// Output: none
void LED_Init(void){ 
	unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
	GPIO_PORTB_PCTL_R &= ~0x0FFF0000; // 2) regular GPIO function
  GPIO_PORTB_DIR_R |= 0x70;      // 3) make PB6-PB4 output
  GPIO_PORTB_AFSEL_R &= ~0x70;     // 4) disable alternate function on PB6-PB4
  GPIO_PORTB_DEN_R |= 0x70;      // 5) enable digital I/O on PB6-PB4
  GPIO_PORTB_AMSEL_R &= ~0x70;     // 6) disable analog function on PB6-PB4
	GPIO_PORTB_DATA_R &= ~0X70;			//7) clear the data register for PB6-PB4
}

void Blink_Red(void) {
	GPIO_PORTB_DATA_R &= ~0X70;
	GPIO_PORTB_DATA_R |= 0X40;
}

void Blink_Green(void) {
	GPIO_PORTB_DATA_R &= ~0X70;
	GPIO_PORTB_DATA_R |= 0X20;
}

void Blink_Blue(void) {
	GPIO_PORTB_DATA_R &= ~0X70;
	GPIO_PORTB_DATA_R |= 0X10;
}

void Next_Level_Lights(void) {
	int i;
	for(i=0;i<10;i++) {
		GPIO_PORTB_DATA_R |= 0X70;
		Delay1ms(25);
		GPIO_PORTB_DATA_R &= ~0X70;
		Delay1ms(25);
	}
}

void Game_Over_Lights(void) {
	GPIO_PORTB_DATA_R |= 0X70;
	Delay1ms(50);
}

void Game_Complete_Lights(void) {
	int i;
	for(i=0;i<10;i++) {
		GPIO_PORTB_DATA_R |= 0X70;
		Delay1ms(200);
		GPIO_PORTB_DATA_R &= ~0X70;
		Delay1ms(200);
		GPIO_PORTB_DATA_R |= 0X70;
		Delay1ms(200);
		GPIO_PORTB_DATA_R &= ~0X70;
		Delay1ms(1000);
	}
}