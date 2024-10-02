//Blaise Swan
#include "stm32f303xe.h"
#include "ultraSonicInit.h"
int ultraSonicInit(void){
// Ultrasonic Sensor Polling Mode - Bonus Lab
//TIM16->CR1 |= TIM_CR1_CEN;
// In this lab, we will design a simple ultrasonic sensor control module that allows you to:
//  1. Call the Ultrasonic Distance Measurement function on demand, and
//  2. Read the Measurement results some time later.
// This is a preparation lab for your full Lab 5 - Ultrasonic Sensor in Interrupt Mode

// As described in class, an Ultrasonic Sensor will require two timers:
//  1. Trigger-Generation Timer (OCM on PWM mode, 10us PW)
//      Recommended Pin - PA.12 TIM 16 - Output CH1
//
//  2. Echo Measurement Timer (ICM on Slave Mode - Rising Edge Reset)
//      Recommended Pin - PC.7 TIM 3 - Input CH2

// For Trigger Generation Timer, we will further set it to ONE-SHOT MODE, so that we can trigger the sensor on demand.
//  i.e. Only trigger the sensor if we want a measurement
// 
// In One-Shot mode, the timer will generate a pulse after enabled, and immediately disable itself automatically after the pulse is generated.





//  Part I - Configure Timer 16 CH1 as Trigger-Generation Timer

//  I.1 Configure GPIO Pin
//		Enable GPIOA, set PA12 to AF - TIM16 CH1, Push-Pull, and no pull-up or pull-down.
RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable GPIOA clock
GPIOA->MODER |= GPIO_MODER_MODER12_1; // Set PA12 to alternate function mode
GPIOA->AFR[1] |= (0x02 << ((4) * 4)); // Set alternate function for TIM16_CH1 on PA12
GPIOA->OTYPER &= ~GPIO_OTYPER_OT_12; // Set output type as push-pull
GPIOA ->OTYPER  &= ~(0x1UL <<15); //push pull

//GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR12; // Disable pull-up and pull-down
//  I.2 Configure TIM16
//      a) Turn on clock for TIM16
//      b) Set the PSC so TIM16 counts in us unit
//      c) Set the TIM16 counting direction to upcounting
//      d) Set the ARR to 10us pulse 
//      e) Enable ARR preload (ARPE) in CR1
//      f) Set main output enabled (MOE) in BDTR

RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; // Enable TIM16 clock
TIM16->PSC = SystemCoreClock/1000000; // Set PSC so TIM16 counts in us unit
//TIM16->CR1 &= ~TIM_CR1_DIR; // Set the TIM16 counting direction to upcounting /// ?
TIM16->CR1 |= TIM_CR1_DIR;
TIM16->ARR = 10; // Set the ARR to 10us pulse 
TIM16->CR1 |= TIM_CR1_ARPE; // Enable ARR preload (ARPE) in CR1
TIM16->BDTR |= TIM_BDTR_MOE; // Set main output enabled (MOE) in BDTR

//  I.3 Configure TIM16 Channel 1 for OCM with PWM
// 		a) Set TIM16 CH1 to be PWM mode 1 through (OC1M) in CCMR2
//      b) Enable the Output Compare Preload on CH1 (we will update CCR1 only upon UE) through setting (OC1PE) on CCMR1
//      c) Enable the Regular Ouptut Channel for CH1 by setting (CC1E) in CCER
//      d) Make CH1 active HI through CLEARING (CC1P) in CCER
//      e) Set CH1 initial output waveform on-time to 1 in CCR1.
//         - NOTE: this setting is different from periodic PWM in Lab 3.
//                 in one-shot mode, you need to start CCR1 with init value of 1 to make sure the timer works correctly. 
//  			   ( is there a better way to do it? )

TIM16->CCMR1 |= TIM_CCMR1_OC1M_1; // Set TIM16 CH1 to be PWM mode 1
TIM16->CCMR1 |= TIM_CCMR1_OC1PE; // Enable the Output Compare Preload on CH1
TIM16->CCER |= TIM_CCER_CC1E; // Enable the Regular Output Channel for CH1
TIM16->CCER &= ~TIM_CCER_CC1P; // Make CH1 active HI
TIM16->CCR1 = 1; // Set CH1 CCR1 initial output waveform on-time for 1us PWM pulse width

//  I.4 Configure TIM16 CH1 for PWM - One Shot
//      a) Set TIM16 CH1 for one-shot mode by setting (OPM) in CR1
// 		b) Force an update event to preload all the registers by setting (UG) in EGR
//      c) !! DO NOT !! enable TIM16 here.
 //        Because we are making an on-demand Trigger, so we only will enable it when the program asks for it

// I.4 Configure TIM16 CH1 for PWM - One Shot
TIM16->CR1 |= TIM_CR1_OPM; // Set TIM16 CH1 for one-shot mode
TIM16->EGR |= TIM_EGR_UG; // Force an update event to preload all the registers
// DO NOT enable TIM16 here





// Part III - Configure Timer 3 CH2 as Edge-Measurement Timer

//  III.1 Configure GPIO Pin
//		Enable GPIOC, set PC7 to AF for TIM3 CH2, no pull-up / pull-down.
//      ** read the ref manual to confirm the AF selection for TIM3 Input Channel 2 (TI2 == TIM3 Input CH2)

RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Enable the clock for GPIOC
GPIOC->MODER |= GPIO_MODER_MODER7_1; // Set PC7 to alternate function mode
GPIOC->AFR[0] |= 0x2 << 28; // Set AF2 (TIM3) for PC7 //28->8
GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR7; // Set PC7 to no pull-up / pull-down

//  III.2 Configure TIM3 Timer Input Channel 2 (TI2)
//  	a) Turn on clock for TIM3
//      b) Set the PSC so TIM3 counts in us
//      c) Set TIM3 counting direction to upcounting
//      d) Set ARR to MAXIMUM possible value (i.e. 0xFFFF)
//          so that if we capture a reall large PW measurement, we know it's garbage.
//      e) Set TIM3 TI2 with No Input Filtering to start with, by CLEARING all 4 bits of (IC2F[3:0]) in CCMR1
//			** read the manual!!! (you may want to increase the filtering in the future, and you need to know how to set it)
//      f) Set TI2 Prescaler to ZERO to capture every valid transition event.  This can be done by CLEARING both bits in
//         (IC2PSC[1:0]) in CCMR1
//          ** read the reference manual for more detail
//          Simple Ver.: Prescaler to 1 => only use EVERY OTHER event as valid event

// III.2 Configure TIM3 Timer Input Channel 2 (TI2)
RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable TIM3 clock
TIM3->PSC = SystemCoreClock/1000000; // Set PSC so TIM3 counts in us
TIM3->CR1 &= ~TIM_CR1_DIR; // Set TIM3 counting direction to upcounting
TIM3->ARR = 0xFFFF; // Set ARR to maximum possible value
TIM3->CCMR1 &= ~TIM_CCMR1_IC2F; // Clear IC2F[3:0] to disable input filter
TIM3->CCMR1 &= ~TIM_CCMR1_IC2PSC; // Clear IC2PSC[1:0] to set TI2 prescaler to zero
TIM3->CCER &= ~TIM_CCER_CC2P; // Clear CC2P to capture every valid transition event

//  III.3 Configure TIM3 CCR1 for PW measurement on TI2
//     	a) Make sure TIM3 TI2 is connected internally to CCR1, so Echo signal falling edge from PC7 can be measured
//         and captured by the timer.
//         This is done by setting (CC1S) in CCMR1 to selection 2
//          ** read the manual to see what is the correct value to write into CC1S

TIM3->CCMR1 |= TIM_CCMR1_CC1S_0; // Set CC1S to selection 1 to connect TI2 to CCR1
// Set CC1P to capture falling edge and clear CC1NP to disable capture of rising edge
TIM3->CCER |= TIM_CCER_CC1P;
TIM3->CCER &= ~TIM_CCER_CC1NP;
//    	b) Set TIM3 CCR1 to capture TI2 falling edge by setting (CC1P) and clearing (CC1NP) in CCER
TIM3->CCER |= TIM_CCER_CC1P;    // set CC1P to capture falling edge
TIM3->CCER &= ~TIM_CCER_CC1NP;  // clear CC1NP to capture only falling edge

//  III.4 Configure TIM3 to Slave Reset on TI2 Rising Edge
//      a) Select Filtered Input Timer 2 (TI2) as the trigger source of the Timer Reset
// 		   Done by setting (TS[3:0]) in SMCR to 6
//          ** read the manual to confirm the configuration value.
TIM3->SMCR |= (6 << TIM_SMCR_TS_Pos);
//   	b) Select RESET slave mode on TI2 Rising Edge, done by writing 4 into (SMS[2:0]) in SMCR
//  	    ** read the manual to confrim the configuration value.
TIM3->SMCR |= TIM_SMCR_SMS_2;
TIM3->SMCR &= ~(TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);

//  III.5 Enable Counters
// 		a) Enable CCR1 by setting (CC1E) in CCER
// Enable CCR1
TIM3->CCER |= TIM_CCER_CC1E;


//  	b) Enable TIM3 main counter by setting (CEN) in CR1
// Enable TIM3 main counter
TIM3->CR1 |= TIM_CR1_CEN;


}
int ECHO_RECEIVED(void) {
		//UARTprintf("ECHO RECIEVED FUNCTION INITIATED");

    if(TIM3->SR & TIM_SR_CC1IF) {
        return 1;
    } else {
        return 0;
    }
}

// 		b) READ_SENSOR function
//  		read the valid CCR1 value from TIM3 to calculate the detected distance.
//				You can return the CCR1 value and let the main program do the calculation, OR
//				You can do the count-to-distance calculation and return the calculated value back to the main program.

float READ_SENSOR(void)
{
    uint16_t ccr1_val = TIM3->CCR1;
	UARTprintf("The CCR1 Val is: %d", ccr1_val);
    float distance_cm;

    // Calculate the distance in cm based on the CCR1 value
    distance_cm = (float)ccr1_val / 58.0;

	UARTprintf("The Distance is: %d", distance_cm);
    return distance_cm;
	
}

void start_trigger() {
    // Enable TIM16
    TIM16->CR1 |= TIM_CR1_CEN;
}

int ON_DEMAND_TRIGGER(){
	TIM3->CR1 |= TIM_CR1_CEN;
}
