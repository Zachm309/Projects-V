//LCD Module Source File

#include "LCD.h"
#include "utility.h"
#include <stdarg.h>

void LCD_GPIO_Init(void)
{
	ENABLE_GPIO_CLOCK( LCD_GPIO_PORT );
	
	//PA6,7,8,9,10,11,out/nopull/pushpull
	
	// 1 turn on the AHB so GPIOs are ON
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	GPIOA->MODER &= ~(3UL << (2*6)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*6));// set the two bits to OUTPUT (01) for PA5
	GPIOA->OTYPER &= ~(1UL << (1*6));
	GPIOA->PUPDR &= ~(3UL << (2*6));
	
	GPIO_MODER_SET( LCD_GPIO_PORT,  6, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 6, GPIO_OTYPE_PP);
	GPIO_PUPDR_SET( LCD_GPIO_PORT,  6, GPIO_PUPD_NO );
	
	
	GPIOA->MODER &= ~(3UL << (2*7)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*7));// set the two bits to OUTPUT (01) for PA5
	GPIOA->OTYPER &= ~(1UL << (1*7));
	GPIOA->PUPDR &= ~(3UL << (2*7));
	
	GPIO_MODER_SET(A, 7, GPIO_MODE_OUT);
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 7, GPIO_OTYPE_PP);
	GPIO_PUPDR_SET( LCD_GPIO_PORT,  7, GPIO_PUPD_NO );
	
	
	GPIOA->MODER &= ~(3UL << (2*8)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*8));// set the two bits to OUTPUT (01) for PA5
	GPIOA->OTYPER &= ~(1UL << (1*8));
	GPIOA->PUPDR &= ~(3UL << (2*8));
	
	GPIO_MODER_SET( LCD_GPIO_PORT,  8, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 8, GPIO_OTYPE_PP);
	GPIO_PUPDR_SET( LCD_GPIO_PORT,  8, GPIO_PUPD_NO );
	
	
	
	GPIOA->MODER &= ~(3UL << (2*9)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*9));// set the two bits to OUTPUT (01) for PA5
	GPIOA->OTYPER &= ~(1UL << (1*9));
	GPIOA->PUPDR &= ~(3UL << (2*9));
	
	GPIO_MODER_SET( LCD_GPIO_PORT,  9, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 9, GPIO_OTYPE_PP);
	GPIO_PUPDR_SET( LCD_GPIO_PORT,  9, GPIO_PUPD_NO );
	
	
	
	GPIOA->MODER &= ~(3UL << (2*10)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*10));// set the two bits to OUTPUT (01) for PA5
	GPIOA->OTYPER &= ~(1UL << (1*10));
	GPIOA->PUPDR &= ~(3UL << (2*10));
	
	GPIO_MODER_SET( LCD_GPIO_PORT,  10, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 10, GPIO_OTYPE_PP);
	GPIO_PUPDR_SET( LCD_GPIO_PORT,  10, GPIO_PUPD_NO );
	
	
	
	GPIOA->MODER &= ~(3UL << (2*11)); //reset the two bits for PA5
	GPIOA->MODER |= (1UL << (2*11));// set the two bits to OUTPUT (01) for PA5
	GPIOA->OTYPER &= ~(1UL << (1*11));
	GPIOA->PUPDR &= ~(3UL << (2*11));
	
	GPIO_MODER_SET( LCD_GPIO_PORT,  11, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( LCD_GPIO_PORT, 11, GPIO_OTYPE_PP);
	GPIO_PUPDR_SET( LCD_GPIO_PORT,  11, GPIO_PUPD_NO );
	
	
	
}


void LCD_Init(void)
{
	LCD_GPIO_Init();
	
	
	CLEAR_BITS(LCD_PORT, LCD_PORT_BITS);
	
	LCD_E_LO;
	LCD_RS_IR;
	
	Delay_ms(10);
	
	LCD_E_HI;
	//Delay(4);
	LCD_BUS(0x03); //wake up isplay
	//delay(4);
	LCD_E_LO;
	Delay_ms(5);
	
	LCD_E_HI;
	LCD_BUS(0x03);
	LCD_E_LO;
	Delay_ms(1);
	
	LCD_E_HI;
	LCD_BUS(0x03);
	LCD_E_LO;
	
	
	LCD_E_HI;
	LCD_BUS(0x02);
	LCD_E_LO;
	
	//LCD_E_HI;
	//LCD_BUS(0x02UL);
	//LCD_E_LO;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//set up
	//4-bit mode, 2 display line, 5x8 font
	LCDcmd( LCD_CMD_FUNCTION | LCD_FUNCTION_4BITBUS | LCD_FUNCTION_2LINES | LCD_FUNCTION_5X8FONT);
	Delay_ms(LCD_STD_CMD_DELAY);
	
	
	//turn off LCD for rest of init
	LCDcmd(LCD_CMD_DISPLAY | LCD_DISPLAY_OFF);
	Delay_ms(LCD_STD_CMD_DELAY);
	
	//send clear cmd
	LCDcmd(LCD_CMD_CLEAR);
	Delay_ms(LCD_STD_CMD_DELAY);
	
	//Send entry cmd to lcd for move cursor mode and increment mode (left to right entry)
	LCDcmd(LCD_CMD_ENTRY | LCD_ENTRY_MOVE_CURSOR | LCD_ENTRY_INC);
	Delay_ms(LCD_STD_CMD_DELAY);
	
	//send display cmd again to lcd and turn on lcd with no cursor display/blink
	LCDcmd(LCD_CMD_DISPLAY | LCD_DISPLAY_ON| LCD_DISPLAY_NOCURSOR |LCD_DISPLAY_NOBLINK);
	Delay_ms(LCD_STD_CMD_DELAY);
	
	//Fertig!
}

void LCD_Clear(void)
{
	Delay_ms(LCD_STD_CMD_DELAY);
	LCDcmd( LCD_CMD_CLEAR);
	Delay_ms(LCD_LONG_CMD_DELAY);
}

void LCD_HomeCursor(void)
{
	LCDcmd( LCD_CMD_HOME );
	Delay_ms( LCD_LONG_CMD_DELAY );
}

void LCDcmd(uint8_t cmd)
{
	Delay_ms(LCD_STD_CMD_DELAY);
	
	LCD_E_LO;
	LCD_RS_IR;
	
	LCD_E_HI;
	LCD_BUS( HI_NYBBLE( cmd ) );
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS( LO_NYBBLE( cmd ) );
	LCD_E_LO;
}
void LCDdata(uint8_t data)
{
	Delay_ms(LCD_STD_CMD_DELAY);
	
	LCD_E_LO;
	LCD_RS_DR;
	
	LCD_E_HI;
	LCD_BUS( HI_NYBBLE( data ) );
	LCD_E_LO;
	
	LCD_E_HI;
	LCD_BUS( LO_NYBBLE( data ) );
	LCD_E_LO;
}

void LCDputc(unsigned char ch)
{
	switch(ch)
	{
		case '\n':
			LCDcmd(LCD_CMD_SETDDADDR | LCD_DDRAM_ADDR_LINE2);
			break;
		
		case '\r':
			LCD_HomeCursor();
			break;
		
		default:
			LCDdata(ch);
			break;
	}
}

void LCDputs(char* str)
{
	while (*str != '\0')
	{
		LCDputc(*str++);
	}
}

void LCDprintf(char* str, ...)
{
	va_list args;
	char buffer[MAX_LCD_BUFSIZ];
	
	va_start( args, str);
	(void)vsnprintf(buffer, MAX_LCD_BUFSIZ, str, args);
	va_end(args);
	
	LCDputs(buffer);
}
