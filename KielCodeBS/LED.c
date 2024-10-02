/*

/////// LED Demo 
*/

#include "LED.h"
void LED_INIT(void){ 
	//initialize
	
	// 1 turn on the AHB so GPIOs are ON
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// 2. GPIO Mode Register -> set to output
	GPIOA->MODER &= ~(3UL << (2*5)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*5));// set the two bits to OUTPUT (01) for PA5
	// 3. Set GPIO Output type to push pull (clear logic 0)
	GPIOA->OTYPER &= ~(1UL << (1*5));
	// 4 (optional) set GPIO pull-up / pull down register to default (no pull)
	GPIOA->PUPDR &= ~(3UL << (2*5));
	// 5 write logic 1 to GPIOA ODR bit 5 (PA5 to Controlling LED)
	GPIOA->ODR |= (1UL << (1*5)); //TURN LED ON
	//Program Loop
}
void TOGGLE(void){
	GPIOA->ODR ^= (1UL << (1*5)); //TURN LED ON
}

void mainMenu(void){
		UARTprintf("MAIN MENU:\n");
    UARTprintf("Press 'y' to move left motor forward\n");
    UARTprintf("Press 'h' to stop left motor\n");
    UARTprintf("Press 'n' to move left motor backward\n");
    UARTprintf("Press 'u' to move right motor forward\n");
    UARTprintf("Press 'j' to stop right motor\n");
    UARTprintf("Press 'm' to move right motor backward\n");
    UARTprintf("Press 'w' to increase servo angle\n");
    UARTprintf("Press 'q' to decrease servo angle\n");
    UARTprintf("Press '0' to stop stepper motor\n");
    UARTprintf("Press '1' to move stepper motor clockwise (full step)\n");
    UARTprintf("Press '2' to move stepper motor counterclockwise (full step)\n");
    UARTprintf("Press '3' to move stepper motor clockwise (half step)\n");
    UARTprintf("Press '4' to move stepper motor counterclockwise (half step)\n");
    UARTprintf("Enter an option: ");
}
