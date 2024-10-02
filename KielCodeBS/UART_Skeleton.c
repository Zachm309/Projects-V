// UART Init

#include "UART.h"

void UART2_Init(void)
{
	// Enable PA2 and PA3 on AF7 for UART2 Comm
	
	// 1. Enable APB1, so it is now driven by the scaled clock
	//RCC->APB1ENR |= (0x1UL << (1 * 17));
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // equivalent
	
	
	 // 1.1 Need to choose UART clock source (so to drive the UART subsystem)
	 //Select USART2 clock source
			//  00: PCLK (peripheral crystal clock, not available)
			//  01: SYstem block (SYSCLK)
			//  10: HSI16 clock source (research into what it is)
			//  11: LSE clock source (research into what it is)
	
	
	
	// 2. Set MODER to AF for PA2 and PA3
	// Then, Configure the desired pins - we will use PA2 and PA3 (common choice)
	//    PA.2 => USART2 TX (on AF7)
	//    PA.3 => USART2 RX (on AF7)
	//  Refer back to LED project to see how to set it up... 00 OUT, 01 IN, 10 AF, 11 Analog
	// Thus, we need to set up 
		// 1) Alternative Function, 
		// 2) Speed (Low Speed?), 3) Push-Pull Mode, and 4) Pull-Up
	
	
	// 3. Alternative Function Register for PA2 and PA3.  Set them to 07 (AF7)
	  // Zero in on the AFRL (AFR[0]), set AF7 on AFR[0] to be 0x7 (AF7)
	
	// 4. Output speed register (OSPEEDR)  set it to LOW Speed (we are on APB, ie. slower south bridge)
	  // Low speed = 00
	
	// 5. Pull-up / Pull-down register (PUPDR)   set it to No Pull
	  // No Pull = 00
	
	// 6. Output type register (OTYPER)   set it to Push-Pull
		// Push Pull = 0
	
}

void UART2_Config(void)
{
	// 1. Disable UART2 (set UE on CR1 to 0)
		// USART2 -> CR1, clear UE bit
	
	// 2. Set the baud rate register (i.e. clock division register) (BRR) to hit 9600 Buad
		// USART2 -> BRR = System Clco Rate / Baud Rate
	
	
	// 3. Configuring Data Size (8bit), Start bit (1), Stop bit (1/2/1.5), Parity bit (No parity, even / odd parity)
	  // USART2 -> CR1, use M mask OR focus on bit 12 and 28.  They are M0 amd M1
	  //   Set to 00 to make data frame size 8-bit
	
	  // USART2 -> CR2, STOP set to 00 (1 bit), 01 (0.5 bit), 10 (2 bits), 11 (1.5 bit)
	
		// OVER8 setup (stick with 16x)
		// USART2 -> CR1, bit OVER8 set to 0
	
	// 4. Enable Transmit and Receive system block (TE and RE)
		// USART2 -> CR1, set TE and RE
	
	// 5. Enable UART2 (set UE on CR1 to 1)
	  // USART2 -> CR1, set CR1
	
	// 6. Wait for the UART2 block to boot up and get ready
	while ( (USART2 -> ISR & USART_ISR_TEACK) == 0 );  // Wait till Transmitter is ready to go
	while ( (USART2 -> ISR & USART_ISR_REACK) == 0 );  // Wait till Receiver is ready to go
	
}

// For initialization, you can either...
//  1. Call UART_Config() inside UART_Init(), then call UART_Init() in main() or
//  2. In main(), call UART_Init() then UART_Config()


// Then, you need to implement:
//  a) UARTputc(char c)  // put a character to the UART TDR when TXE is set
//  b) UARTgetc()  	 // read a character from the UART RDR when RXNE is set.  Check whether you need to reset RXNE.
//  c) UARTputs(char* s) // FOr the entire string s, put the individual characters onto UART with UARTputc()
//  d) UARTprintf(...)	 // The UART version of printf().  Check the slides for implementation details.
 //                    ALSO!, watch out for string "c" vs char 'c'
 //				Recall how they are different! (solution in the slides)	 
