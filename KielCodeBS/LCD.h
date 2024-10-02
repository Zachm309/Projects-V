//LCD HEADER
#ifndef __LCD_H
#define __LCD_H

#include "stm32f303xe.h"
#include "utility.h"

//Command to LCD Module
#define LCD_CMD_CLEAR						0x01u
#define LCD_CMD_HOME						0x02u
#define LCD_CMD_ENTRY						0x04u
#define LCD_CMD_DISPLAY					0x08u
#define LCD_CMD_CDSHIFT					0x10u
#define LCD_CMD_FUNCTION				0x20u
#define LCD_CMD_CGRAMADDR				0x40u
#define LCD_CMD_SETDDADDR				0x80u

//settings for LCD command entry
#define LCD_ENTRY_MOVE_DISPLAY	0x01
#define LCD_ENTRY_MOVE_CURSOR		0x00
#define LCD_ENTRY_INC						0x02
#define LCD_ENTRY_DEC						0x00

//settings for LCD command display
#define LCD_DISPLAY_BLINK				0x01
#define LCD_DISPLAY_NOBLINK			0x00
#define LCD_DISPLAY_CURSOR			0x02
#define LCD_DISPLAY_NOCURSOR		0x00
#define LCD_DISPLAY_ON					0x04
#define LCD_DISPLAY_OFF					0x00

//Settings for LCD CD shift 
#define LCD_CDSHIFT_RIGHT				0x04
#define LCD_CDSHIFT_LEFT				0x00
#define LCD_CDSHIFT_DISPLAY			0x08
#define LCD_CDSHIFT_CURSOR			0x00

//Settings for LCD function commands
#define LCD_FUNCTION_5X10FONT		0x04 
#define LCD_FUNCTION_5X8FONT		0x00
#define LCD_FUNCTION_2LINES			0x08
#define LCD_FUNCTION_1LINE			0x00
#define LCD_FUNCTION_8BITBUS		0x10
#define LCD_FUNCTION_4BITBUS		0x00

//Common LCD operation delays
#define LCD_LONG_CMD_DELAY			16
#define LCD_STD_CMD_DELAY				2

#define LCD_DDRAM_ADDR_LINE1		0x00
#define LCD_DDRAM_ADDR_LINE2 		0x40

//GPIO port Constants
#define LCD_GPIO_PORT						A
#define LCD_PORT								GPIOA->ODR
#define LCD_RS_BIT							(1UL << 6) //PA6
#define LCD_E_BIT								(1UL << 7) //PA7
#define LCD_BUS_BIT							(0xF << 8) //PA8, 9, 10 & 11
#define LCD_BUS_BIT_POS 				8

#define LCD_PORT_BITS						(LCD_RS_BIT | LCD_E_BIT | LCD_BUS_BIT)

// Helper macros
#define LCD_E_LO								CLEAR_BITS( LCD_PORT, LCD_E_BIT )
#define LCD_E_HI								SET_BITS (LCD_PORT, LCD_E_BIT )
#define LCD_RS_IR								CLEAR_BITS( LCD_PORT, LCD_RS_BIT )
#define LCD_RS_DR								SET_BITS( LCD_PORT, LCD_RS_BIT )
#define LCD_BUS( value )				FORCE_BITS (LCD_PORT, LCD_BUS_BIT, (value) << LCD_BUS_BIT_POS )

// Constants
#define MAX_LCD_BUFSIZ 					81 //for 80 character and 1 null character

void LCD_GPIO_Init(void);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_HomeCursor(void);

void LCDcmd(uint8_t cmd);
void LCDdata(uint8_t data);

void LCDputc(unsigned char ch);
void LCDputs(char* str);
void LCDprintf(char* str, ...);

#endif