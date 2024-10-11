#ifndef __UART_H
#define __UART_H

#include "stm32f303xe.h"
void UART2_INIT(void);



void UART3_INIT(void);
void UART3_Config(void);

void UART3putc(char myChar);
void UART3puts(char *myStr);
char UART3getc(void);
void UART3printf( char *format, ...);
char UART3getcND(void);





void UART2_Config(void);

void UARTputc(char myChar);
void UARTputs(char *myStr);
char UARTgetc(void);
void UARTprintf( char *format, ...);
char UARTgetcND(void);
#endif
