#include "tm4c123gh6pm.h"
#include "random.h"

//------------Delay1ms-------------
// Input: number of msec to delay
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

//------------Blink_Red------------
// Turn off all LEDs.  Turn on only the red LED (PB6).
// Input: none
// Output: none
void Blink_Red(void) {
	GPIO_PORTB_DATA_R &= ~0X70;
	GPIO_PORTB_DATA_R |= 0X40;
}

//------------Blink_Green------------
// Turn off all LEDs.  Turn on only the green LED (PB5).
// Input: none
// Output: none
void Blink_Green(void) {
	GPIO_PORTB_DATA_R &= ~0X70;
	GPIO_PORTB_DATA_R |= 0X20;
}

//------------Blink_Blue------------
// Turn off all LEDs.  Turn on only the blue LED (PB4).
// Input: none
// Output: none
void Blink_Blue(void) {
	GPIO_PORTB_DATA_R &= ~0X70;
	GPIO_PORTB_DATA_R |= 0X10;
}

//------------Next_Level_Lights------------
// Display during transition to next level. Flash all LEDs on and off 10 times.
// Input: none
// Output: none
void Next_Level_Lights(void) {
	int i;
	for(i=0;i<10;i++) {
		GPIO_PORTB_DATA_R |= 0X70;
		Delay1ms(25);	// turn on all lights for 25 msec
		GPIO_PORTB_DATA_R &= ~0X70;
		Delay1ms(25);	// turn off all lights for 25 msec
	}
}

//------------Game_Over_Lights------------
// Display if incorrect switch pressed.  Turn all LEDs on for 3 secs
// Input: none
// Output: none
void Game_Over_Lights(void) {
	GPIO_PORTB_DATA_R |= 0X70;
	Delay1ms(3000);		// turn on all lights for 3 sec
	GPIO_PORTB_DATA_R &= ~0X70;
}

//------------Game_Complete_Lights------------
// Display if final level completed.
// Input: none
// Output: none
void Game_Complete_Lights(void) {
	int i;
	for(i=0;i<10;i++) {
		GPIO_PORTB_DATA_R |= 0X70;
		Delay1ms(50);
		GPIO_PORTB_DATA_R &= ~0X70;
		Delay1ms(50);
		GPIO_PORTB_DATA_R |= 0X70;
		Delay1ms(50);
		GPIO_PORTB_DATA_R &= ~0X70;
		Delay1ms(1000);
	}
}