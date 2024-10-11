#include "stm32f303xe.h"
#include "utility.h"
//#include "SysClock.h"
#include "UART.h"

void limInit(void){
	// Enable GPIOC Clock
RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;

// Configure PC5 and PC6 as Input Mode
// Clear the mode bits for PC5 and PC6
GPIOC->MODER &= ~((3U << (2 * 5)) | (3U << (2 * 6)));

// Configure PC5 and PC6 with Pull-Up Resistors
// Clear the PUPDR bits for PC5 and PC6
GPIOC->PUPDR &= ~((3U << (2 * 5)) | (3U << (2 * 6)));
// Set Pull-Up for PC5 and PC6
GPIOC->PUPDR |= ((1U << (2 * 5)) | (1U << (2 * 6)));

// Enable SYSCFG Clock (Needed for EXTI configuration)
RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

// Connect EXTI Lines to GPIOC Pins
// EXTI5 and EXTI6 are configured in EXTICR[1] (EXTICR1 covers EXTI4-7)
SYSCFG->EXTICR[1] &= ~((0xF << (4 * (5 - 4))) | (0xF << (4 * (6 - 4)))); // Clear EXTI5 and EXTI6 settings
SYSCFG->EXTICR[1] |= ((SYSCFG_EXTICR2_EXTI5_PC) | (SYSCFG_EXTICR2_EXTI6_PC)); // Set EXTI5 and EXTI6 to PC5 and PC6
	
	

// Enable Interrupts for EXTI5 and EXTI6
EXTI->IMR |= (EXTI_IMR_IM5 | EXTI_IMR_IM6); // Unmask EXTI5 and EXTI6

// Configure Falling Edge Trigger for EXTI5 and EXTI6
EXTI->FTSR |= (EXTI_FTSR_FT5 | EXTI_FTSR_FT6); // Enable falling edge trigger

// (Optional) Disable Rising Edge Trigger if not needed
EXTI->RTSR &= ~(EXTI_RTSR_RT5 | EXTI_RTSR_RT6); // Disable rising edge trigger

// Configure NVIC for EXTI Events on Pins 5 and 6
// EXTI4_15_IRQn handles EXTI4 to EXTI15
// Enable NVIC for EXTI events on pin 5-9 (for PC5 and PC6)
NVIC_EnableIRQ(EXTI9_5_IRQn); 
NVIC_SetPriority(EXTI9_5_IRQn, 0);  // Set priority to 0 (next highest to NMIs)

// Example Interrupt Handler for EXTI4_15_IRQn


}

void EXTI4_15_IRQHandler(void) {
    // Check if EXTI5 triggered the interrupt
    if (EXTI->PR & EXTI_PR_PR5) {
        EXTI->PR |= EXTI_PR_PR5; // Clear the pending interrupt flag for EXTI5
        // TODO: Add your PC5 interrupt handling code here
    }

    // Check if EXTI6 triggered the interrupt
    if (EXTI->PR & EXTI_PR_PR6) {
        EXTI->PR |= EXTI_PR_PR6; // Clear the pending interrupt flag for EXTI6
        // TODO: Add your PC6 interrupt handling code here
    }
}

