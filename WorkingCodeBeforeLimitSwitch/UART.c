//UART Init 
//Blaise Swan
// In this file we will be setting up the UART_INIT and UART_CONFIG so that we can use the appropriate ports and print out characters
#include "UART.h"
#include <stdio.h>
#include <stdarg.h>
#define BAUD_RATE 9600
void UART2_INIT(void)
{
	// Enable PA2 and PA3 on AF7 for UART2 Comm
	
	// 1. Enable APB1, so it is now driven by the scaled clocl
	RCC->APB1ENR |= (0x1UL<<(1*17)); // this statement is equivalent to 
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // this one 
	
	//new code
	RCC -> CFGR3 &= ~(0x3UL <<(16)); //set 0
	RCC -> CFGR3 |= (0x1UL <<(16)); //set 1
	
	
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // same line as LED
	
	GPIOA -> MODER &= ~( 0x03UL << (2*2) ); //clear out GPIOA Pin 2 mode bits
	GPIOA -> MODER &= ~( 0x03UL << (2*3) ); //clear out GPIOA Pin 2 mode bits
	
	GPIOA -> MODER |= ( 0x02UL << (2*2) ); //Set GPIOA Pin 2 Mode bits to 10 for AF
	GPIOA -> MODER |= ( 0x02UL << (2*3) ); //Set GPIOA Pin 2 Mode bits to 10 for AF
	// Recall: 00 - input, 01 - output, 10 - AF, 11 - analog
	
	GPIOA -> AFR[0] |= ( 0x07UL << (4*2) ); // Set PA2 to AF7 (each AFR is 4 bit, AFR[0] is AFRL)
	GPIOA -> AFR[0] |= ( 0x07UL << (4*3) ); //
	
	GPIOA -> OSPEEDR &= ~( 0x03UL << (4*2) ); // Low speed for PA2, why?
	GPIOA -> OSPEEDR &= ~( 0x03UL << (4*3) ); // Low speed for PA3, why?

	
	//Pull-Up Configuration : No Pull 
	//00 - No Pull, 01 - Pull - Up, 10 - Pull Down, 11 - Reserved (Not used)
	GPIOA -> PUPDR &= ~ (0x3UL << (2*2)); // No Pull for PA2
	GPIOA -> PUPDR &= ~ (0x3UL << (2*3)); // No pull for PA3
	
	//Output Type Configuration : Push-Pull
	//0 - Push-pull, 1 - Open Drain
	GPIOA -> OTYPER &= ~ (0x1UL << 2 );
	GPIOA -> OTYPER &= ~ (0x1UL << 3 );
	
	
	// 2. Set MODER to AF for PA2 and PA3 (Done Above)
	
	
	// 3. Alternative Function Register for PA2 and PA3. Set them to o7 (AF7) (Done Above)
	
	// 4. Output speed register (OSPEEDR) set it to LOW speed (we are on APB, ie, slower south bridge (Done Above)
	
	// 5. Pull-Up / Pull-down register (PUPDR) set it to no PULL (Done above)
	//GPIOA->PUPDR &= ~(3UL << (2*2));
	//GPIOA->PUPDR &= ~(3UL << (2*3));
	//6. Output type register (OTYPER) set it to push pull (done above)
	
	
	
	
	
}

void UART2_Config(void)
{
	// 1. Disable UART2 (set UE on CR1 to 0)
	USART2 -> CR1 &= ~USART_CR1_UE;
	
	//Configure word length (M0 and M1 Combined
	//Mode: 00 - 8 bit legnth, 01 - 9 bit length, 10 - 7 bit 
	USART2 -> CR1 &= ~USART_CR1_M;
	
	//Sampling Rate (set to 16x, Do NOT go with 8x - well, not gonna hurt
	//Sampling Rate : 0 - Oversample  by 16x, 1 - oversample by 8x (just don't)
	
	USART2 -> CR1 &= ~USART_CR1_OVER8;
	
	USART2 -> CR1 &= ~USART_CR2_STOP;
	
	// Stop Bit Configuration (we will use 1 stop bit)
	//This setting is on CR2
	//00 - 1 bit, 01 - .5 bit (what?), 10 2 bit, 11 - 1.5bit (No clue)
	//USART2 -> CR2 &= ~USART_CR2_STOP;
	
	// Configure BUAD Rate (BAUD_RATE = 9600, #define at the beginning)
	// APB System Clock Rate is 72MHz 
	// System_stm32f3xx.c has some definitions but confusing...set to 80Mhz
	// To hit 9600 BAUD, we need to set the BAUD clock divider by:
	// 72MHz / BAUD_RATE (for 16x oversampling)
	// 72MHz / 2 * BUAD_RATE (for 8x oversampling) 
	//Thus, with our settings, we should set USARTDIV to:
	// USARTDIV = 72,000,000 / 9600 = 7500
	// Note: BBR is 16 bit long, so consider the design constraints!
	USART2 -> BRR = SystemCoreClock / BAUD_RATE;
	
	// 6 Enable Transmitter and Receiver 
	USART2 -> CR1 |=USART_CR1_RE;
	USART2 -> CR1 |=USART_CR1_TE;
	// 7 Re-enable USART 
	USART2 -> CR1 |= USART_CR1_UE; 
	
	//Make sure USART is ready to respond 
	//Zhu: After re-enabling USART with new settings
	//     REACK will take some time to be automatically set to zero when reciever 
	// 		 TEACK should aalso be set to zero automatically after reset, so to get 
	//Both flags are available in USART interrupt status register (ISR)
	
	while ( (USART2 -> ISR & USART_ISR_TEACK) == 0); //wait till good to go
	while ( (USART2 -> ISR & USART_ISR_REACK) == 0); //wait till good to go //1 good to go. 0 is to be set

	
}

void UARTputc(char myChar)
{
	while( (USART2 -> ISR & USART_ISR_TXE) == 0 );
	USART2 -> TDR = (uint8_t) myChar;
}
void UARTputs(char *myStr)
{
	while( *myStr ){ //as long as we have not yet seen a null character 
		UARTputc( *myStr++ ); // send this character and move on to the next character 
	}
}
char UARTgetc(void)
{
	while( (USART2 -> ISR & USART_ISR_RXNE) == 0);
	return (char)(USART2 -> RDR);
}

#define MAX_BUF_SIZE 100
void UARTprintf(char *format, ...)
{
	va_list args;
	char buffer[MAX_BUF_SIZE];
	va_start(args, format);
	
	vsnprintf(buffer, MAX_BUF_SIZE, format, args);

	va_end(args);
  UARTputs(buffer);
}
char UARTgetcND(void){
	if(USART2-> ISR & USART_ISR_RXNE)
		return(USART2->RDR);
	else
		return('\0');
}






