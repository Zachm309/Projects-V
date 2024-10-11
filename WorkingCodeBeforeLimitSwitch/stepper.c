#include "Stepper.h"
#include "UART.h"
#include "stm32f303xe.h"

static uint8_t currentStepIndex = 0;

const uint16_t stepPatternTable[8] = 
{
	0x8,
	0xA,
	0x2,
	0x6,
	0x4,
	0x5,
	0x1,
	0x9
};

void Stepper_Init(void){
	
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;
	
	GPIOC -> MODER &= ~(0x3UL << (2*0));
	GPIOC->MODER |= (0x1UL<<(2*0));
	
	GPIOC -> PUPDR &= ~(0x3UL << (2*0)); //no pull
	
	GPIOC->OTYPER &= ~(1UL);//psuh pull
	
	GPIOC -> MODER &= ~(0x3UL << (2*1));
	GPIOC -> MODER |= (0x1UL <<(2*1));
	
	GPIOC -> PUPDR &= ~(0x3UL <<(2*1));
	GPIOC -> OTYPER &= ~(0x1UL <<(1));
	
	GPIOC -> MODER &= ~(0x3UL << (2*2)); //clear
	GPIOC -> MODER |= (0x1UL <<(2*2)); //set 01
	
	GPIOC -> PUPDR &= ~(0x3UL << (2*2));
	 //no pull
	GPIOC ->OTYPER  &= ~(0x1UL <<2); //push pull
	
	GPIOC -> MODER &= ~(0x3UL << (2*3)); //clear both bits
	GPIOC -> MODER |= (0x1UL <<(2*3));
	
	GPIOC -> PUPDR &= ~(0x3UL << (2*3));
	
	GPIOC -> OTYPER &= ~(0x1UL <<3);
	
}
void Stepper_Step(int8_t stepType){
	currentStepIndex += stepType;
	GPIOC->ODR = (stepPatternTable[currentStepIndex%8]); //output
		
}



