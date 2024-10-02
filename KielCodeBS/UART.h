#ifndef __UART_H
#define __UART_H

#include "stm32f303xe.h"
void UART2_INIT(void);

void UART2_Config(void);

void UARTputc(char myChar);
void UARTputs(char *myStr);
char UARTgetc(void);
void UARTprintf( char *format, ...);
char UARTgetcND(void);
#endif
