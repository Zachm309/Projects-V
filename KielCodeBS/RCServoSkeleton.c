// RC Servo Skeleton
#include "UART.h"
#include "stm32f303xe.h"
#include "RCServo.h"

#define MAX_PULSE_WIDTH  2400UL
#define MIN_PULSE_WIDTH  1140UL
#define CENTERPERIOD 1500UL

#define MAX_ANGLE  45
#define MIN_ANGLE  -45
#define SERVO_ANGLE_STEP 10
void RCServo_Init(void)
{
	// Set up PB15 to Output, AF1, PP, no Pull
	//GPIOC -> MODER &= ~(0x3UL << (2*4));
	//GPIOC->MODER |= (0x1UL<<(2*4));
	// 1. Enable clock to Port B
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
	GPIOB->MODER &= ~(3UL << (2*15));
	GPIOB->MODER |= (2UL << (2*15));
	// 2. Set the MODER to AF
	GPIOB->AFR[1] &= ~(3UL << 28);
	GPIOB->AFR[1] |= (1UL <<28);
	// 3. Set the AFR..([0] or [1]?) to AF1 for PB15
	// 4. Set the OTYPER to Push-Pull
	GPIOB ->OTYPER  &= ~(0x1UL <<15); //push pull
	// 5. Set the PUPDR to no pull
	GPIOB->PUPDR &= ~(3UL << 30);
	// Your Task - Use your MACRO in place of the bit-operation statements from the previous labs.
	
	
	
	// Configure TIM15
	// 1. Turn on TIM15 by feeding in the APB clock.  (RCC_APB2ENR_TIM15EN)
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	// 2. Program the prescaler (PSC) to ensure TIM15 counts at 1us
	uint32_t psc = (SystemCoreClock / 1000000) -1;
	TIM15->PSC = psc;
	//		Timer Period = (Prescaler - 1) / SystemClockFreq
	//      aka.  Timer Period = System Clock Period * (Prescaler - 1)
	//    Then, what is PSC with Timer Period = 1us?
	// 3. Set TIM15 to Upcounting (no need to do it, because TIM15 only knows upcounting...) ***OPTIONAL***
	// 4. Set the ARR to 20000 us period
	TIM15-> ARR = 20000 -1;
	//      Repeating Counter Period = ARR + 1
	//    Then, what is ARR?
	// 5. Enable TIM15 ARR Preload (ARPE flag on CR1)
	TIM15->CR1 |= TIM_CR1_ARPE;
	// 6. Enable TIM15 main output, so to make it avaiable to the PWM OC (MOE flag on BDTR)
	TIM15->BDTR |= TIM_BDTR_MOE;
	
	
	// Configure CH2 of TIM15 for PWM OC mode
	// 1. Select PWM mode 1 (low-count mode) for TIM15 CH2 (OC2M flags - 2 bits - on CCMR1)
	//    ** PAY VERY CLOSE ATTENTION ON THIS ITEM **
	TIM15->CCMR1 &= ~(TIM_CCMR1_OC2M);
	TIM15->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
	// 2. Enable the Output Compare Preload on CH2 (OC2PE flag on CCMR1)
	TIM15->CCMR1 |= TIM_CCMR1_OC2PE;
	// 3. Enable output channel 2 (CH2 with no negate) - (CC2E flag on CCER)
	TIM15->CCER |= TIM_CCER_CC2E;
	// 4. Make CH2 Active Hig (no negate) - (CC2P flag on CCER)
	TIM15->CCER &= ~(TIM_CCER_CC2P);
	// 5. Set CH2 PWM initial on-time to 0, so PWM will not output anything before preload is done (writing 0 into CCR2)
	TIM15->CCR2 = 0;
	
	
	// Set TIM15 off!
	// 1. Force and Update Event to ensure all preload operations are done in sync! (UG flag on EGR)
	TIM15->EGR |= TIM_EGR_UG;
	// 2. Enable TIM15 Counting! (CEN flag on CR1)
	TIM15->CR1 |= TIM_CR1_CEN;
	
	// If everything done correctly, you should be able to see on PB15 a PWM waveform
	//
	// There are 3 waveforms to capture
	//  1. Neutral Position (Pulse Width = 1500 us)
	//  2. -45 Deg Position (Pulse Width = 1050 us)
	//  3. +45 Deg Position (Pulse Width = 1950 us)
}


uint16_t RCServo_SetAngle( int16_t angle )
{
	//define for max and min have yet to be defined 
	
	// 1. Convert the target angle to the corresponding target Pulse Width
	uint16_t targetPW = MIN_PULSE_WIDTH + ((MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) / (MAX_ANGLE - MIN_ANGLE)) * (angle + MIN_ANGLE);
	//UARTprintf("TARGETPW is %d", targetPW);
	// 2. Check whether the PW has exceeded the mechanical (+45 ~ -45 degrees) & motor limit (+/- 90 degrees)
	//    and of course cap the target PW at the limits!
	if (targetPW < MIN_PULSE_WIDTH) 
	{	
	targetPW = MIN_PULSE_WIDTH;
	}
	if (targetPW > MAX_PULSE_WIDTH) 
	{	
	targetPW = MAX_PULSE_WIDTH;
	}
	// 3. Write the new target PW into TIM15 CR2
	TIM15->CCR2 =targetPW;
	// 4. return the calculated PW for printout in main()
	//UARTprintf("the MAX TargetPW is %d", targetPW);
	return targetPW;
}