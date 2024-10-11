#include "UART.h"
#include <stdio.h>
#include <stdarg.h>
#define BAUD_RATE 9600

void UART3_INIT(void)
{
	// Enable PB10 and PB11 on AF7 for UART3 Comm
	
	// 1. Enable APB1 for USART3
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; 
	
	RCC -> CFGR3 &= ~(RCC_CFGR3_USART3SW_1); //set 1
	RCC -> CFGR3 |= (RCC_CFGR3_USART3SW_0); //set 0
	
	
	//	RCC -> CFGR3 &= ~(RCC_CFGR3_USART3SW_1); //set 1
	//  RCC -> CFGR3 |= (RCC_CFGR3_USART3SW_0); //set 0
	
	// Enable clock for GPIOB
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
	// 2. Configure PB10 as Tx (AF7) and PB11 as Rx (AF7)
	GPIOB -> MODER &= ~( 0x03UL << (2*10) ); // Clear PB10 mode bits
	GPIOB -> MODER &= ~( 0x03UL << (2*11) ); // Clear PB11 mode bits
	
	GPIOB -> MODER |= ( 0x02UL << (2*10) ); // Set PB10 to Alternate Function (AF)
	GPIOB -> MODER |= ( 0x02UL << (2*11) ); // Set PB11 to Alternate Function (AF)
	
	// 3. Set Alternate Function to AF7 (USART3) for PB10 (Tx) and PB11 (Rx)
	GPIOB -> AFR[1] |= ( 0x07UL << (4*(10-8)) ); // Set PB10 to AF7 (AFR[1] for PB8-PB15) // 
	GPIOB -> AFR[1] |= ( 0x07UL << (4*(11-8)) ); // Set PB11 to AF7 (AFR[1] for PB8-PB15) // 

	// Set output speed to low for both PB10 and PB11
	GPIOB -> OSPEEDR &= ~( 0x03UL << (2*10) ); // Low speed for PB10
	GPIOB -> OSPEEDR &= ~( 0x03UL << (2*11) ); // Low speed for PB11
	
	// No Pull-Up/Down configuration for PB10 and PB11
	GPIOB -> PUPDR &= ~ (0x3UL << (2*10)); // No Pull for PB10
	GPIOB -> PUPDR &= ~ (0x3UL << (2*11)); // No pull for PB11
	
	// Push-pull output type for PB10 and PB11
	GPIOB -> OTYPER &= ~ (0x1UL << 10 );
	GPIOB -> OTYPER &= ~ (0x1UL << 11 );
}

void UART3_Config(void)
{
	// 1. Disable USART3 (set UE on CR1 to 0)
	USART3 -> CR1 &= ~USART_CR1_UE;
	
	// Configure word length (M0 and M1 Combined)
	USART3 -> CR1 &= ~USART_CR1_M;  // 8-bit word length
	
	// Set oversampling by 16x
	USART3 -> CR1 &= ~USART_CR1_OVER8;
	
	// Configure stop bits to 1
	USART3 -> CR2 &= ~USART_CR2_STOP;
	
	// Configure baud rate (BAUD_RATE = 9600)
	USART3 -> BRR = SystemCoreClock / BAUD_RATE;
	
	// Enable Transmitter and Receiver
	USART3 -> CR1 |= USART_CR1_RE;
	USART3 -> CR1 |= USART_CR1_TE;
	
	// Re-enable USART3
	USART3 -> CR1 |= USART_CR1_UE; 
	
	// Wait until USART3 is ready
	while ( (USART3 -> ISR & USART_ISR_TEACK) == 0);
	while ( (USART3 -> ISR & USART_ISR_REACK) == 0);
}

void UART3putc(char myChar)
{
	while( (USART3 -> ISR & USART_ISR_TXE) == 0 );
	USART3 -> TDR = (uint8_t) myChar;
}

void UART3puts(char *myStr)
{
	while( *myStr ){ 
		UART3putc( *myStr++ ); 
	}
}

char UART3getc(void)
{
	while( (USART3 -> ISR & USART_ISR_RXNE) == 0);
	return (char)(USART3 -> RDR);
}

#define MAX_BUF_SIZE 100
void UART3printf(char *format, ...)
{
	va_list args;
	char buffer[MAX_BUF_SIZE];
	va_start(args, format);
	
	vsnprintf(buffer, MAX_BUF_SIZE, format, args);
	va_end(args);
	UART3puts(buffer);
}

char UART3getcND(void)
{
	if(USART3-> ISR & USART_ISR_RXNE)
		return(USART3->RDR);
	else
		return('\0');
}
