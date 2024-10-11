// PushButton SOurce File (need PC13)

#include "Pushbutton.h"
#include "stm32f303xe.h"
#include "utility.h"

static int rowNum;
static int colNum;

//[rowNum][colNum];

static char charTable[4][4] = { { '1', '2', '3', 'A'}, \
																{ '4', '5', '6', 'B'}, \
																{ '7', '8', '9', 'C'}, \
																{ '*', '0', '#', 'D'} }; \

void PushButton_Init(void)
{
	// Enable GPIO Port C
	RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
	//ENABLE_GPIO_CLOCK( C );

	

	
	//Inizialize general purpose pins
	GPIO_MODER_SET( B, 0, MODER_GP); 
	GPIO_MODER_SET( B, 1, MODER_GP);
	GPIO_MODER_SET( B, 2, MODER_GP);
	GPIO_MODER_SET( B, 3, MODER_GP);
	GPIO_MODER_SET( B, 4, MODER_IN);
	GPIO_MODER_SET( B, 5, MODER_IN);
	GPIO_MODER_SET( B, 6, MODER_IN);
	GPIO_MODER_SET( B, 7, MODER_IN);
	
	
	
	//Inizilaize pull up resistors and no pull
	GPIO_PUPDR_SETS( B, 0, PUPDR_NO);
	GPIO_PUPDR_SETS( B, 1, PUPDR_NO);
	GPIO_PUPDR_SETS( B, 2, PUPDR_NO);
	GPIO_PUPDR_SETS( B, 3, PUPDR_NO);
	GPIO_PUPDR_SETS( B, 4, PUPDR_PULLU);
	GPIO_PUPDR_SETS( B, 5, PUPDR_PULLU);
	GPIO_PUPDR_SETS( B, 6, PUPDR_PULLU);
	GPIO_PUPDR_SETS( B, 7, PUPDR_PULLU);
	
	GPIO_OTYPER_SET( B, 0, OTYPER_PP );
	GPIO_OTYPER_SET( B, 1, OTYPER_PP );
	GPIO_OTYPER_SET( B, 2, OTYPER_PP );
	GPIO_OTYPER_SET( B, 3, OTYPER_PP );
	GPIO_OTYPER_SET( B, 4, OTYPER_PP );
	GPIO_OTYPER_SET( B, 5, OTYPER_PP );
	GPIO_OTYPER_SET( B, 6, OTYPER_PP );
	GPIO_OTYPER_SET( B, 7, OTYPER_PP );
	
	
	
	// Set PUPD to no pull (00 for pin 13)
	// GPIOC ->PUPDR &= ~(0x3UL << (2*13));
	
	
}

char scanKey(void)
{

		
	CLEAR_BITS (GPIOB -> ODR, GPIO_ODR_0);
  CLEAR_BITS (GPIOB -> ODR, GPIO_ODR_1);
  CLEAR_BITS (GPIOB -> ODR, GPIO_ODR_2);
  CLEAR_BITS (GPIOB -> ODR, GPIO_ODR_3);
	
	colNum = 0;
	rowNum = 0;
	
	uint8_t readout = (GPIOB -> IDR & 0x00F0);
	if ( readout == 0x00F0 )
	{
		return '\0';
	}
	// 	2.2 If not 0xF, convert:
	//   0xE (0b1110) colNum = 0;
	//   0xD (0b1101) colNum = 1;
	//   0xB (0b1011) colNum = 2;
	// 	 0x7 (0b0111) colNum = 3;
	//  This will identify the column to track
	
		switch (readout) {
			
			case 0xE0:
			colNum = 0;
			break;
			
			case 0xD0:
			colNum = 1;
			break;
			
			case 0xB0:
			colNum = 2;
			break;
			
			case 0x70:
			colNum = 3;
			break;
		}
	
	
	//   Start Row Scanning!
	
	
	
	// 3. Debounce (call debounce())
	
	debounce(); 
	
	uint8_t readoutNew = (GPIOB-> IDR & 0x00F0);
	if (readout != readoutNew)
	{
		return '\0';
	}
	
	
	for (int i = 0; i < 4; i++)
	{
		switch(i) {
			
			case 0:
			GPIOB -> ODR &= ~(0x000F);
			GPIOB -> ODR |= 0x0001; 	//0001
			
			break;
			
			case 1:
			GPIOB -> ODR &= ~(0x000F);
			GPIOB -> ODR |= 0x0002;  //0010
			
			break;
				
			case 2:
			GPIOB -> ODR &= ~(0x000F);
			GPIOB -> ODR |= 0x0004;  //0100
			
			break;
				
			case 3:
			GPIOB -> ODR &= ~(0x000F);
			GPIOB -> ODR |= 0x008; //1000
			
			break;
		}
		readout = (GPIOB -> IDR & 0x00F0);
		if (readout == 0x00F0)
		{
			rowNum = i;
			break;
		}
	}
	
	
	
	// 4. Row Scanning...
	//  4.1 Set PB0-3 to 0b1110 (0xE)
	//    This targets row 1
	//    Monitor the C# corresponding to colNum
	//     IF C# turns 0
	//		then record rowNum = 0
	//      break out of step 4.
	//     OTHERWISE move on to 4.2
	
	//  4.2 Set PB0-3 to 0b1101 (0xD)
	//    This targets row 2
	//    IF C# turns 0
	//     record rowNum = 1
	//     break out of 4.
	//    OTHERWISE move on to 4.3
	
	//  4.3 Set PB0-3 to 0b1011 (0xB)
	//    Do the same thing... rowNum = 2 if C# = 0
	
	//  4.4 Set PB0-3 to 0b0111 (0x7)
	//    Do the same thing... rowNum = 3 if C# = 0
	//    IF C# = 1 after scanning, move on to 4.5 
	
	//  4.5 return NULL
	
	
	
	// After step 4, we have
	//  colNum and rowNum locked in.
	
	// 5. Use colNum and rowNum to look for
    //    corresponding chars
	
	return charTable[rowNum][colNum];
}

void debounce(void) 
{
	Delay_ms(10);
}

// Lab 4
//  1. Init PB0-7 for matrix keypad
//  2. Can scan correctly for keypress
//  3. Debounce working
//  4. Print the pressed character on terminal

//  Above and Beyond
//  1. Use keypad as alternative control
//     for stepper and servo motors
//  2. Counter-based debounce mechanism
