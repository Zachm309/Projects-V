


#ifndef __scottMacro_H
#define __scottMacro_H

//////////////////////////////////
// General Purpose Utility Macros
//////////////////////////////////

#include "stm32f303xe.h"

// Notice that variables in the macro are resolved into values using "()"

#define SET_BITS(port, bits)		((port) |= (bits))
#define CLEAR_BITS(port, bits) 	((port) &= ~(bits))
#define FLIP_BITS(port, bits)		((port) ^= (bits))

// Use this set or clear multiple bits in a port pointed by the mask
#define FORCE_BITS(port, mask, value) 	(((port) & (~(mask))) | ((value) & (mask)))

// Logic Bit Test
#define IS_BIT_SET(port, bits)		((port) & (bits))
#define IS_BIT_CLEAR(port, bits)	((~(port)) & (bits))

// Nibble Selection
#define LO_NYBBLE(value)	((value) & 0x0F)
#define HI_NYBBLE(value)	((value >> 4) & 0x0F)


// Some New MACROs

// Port Name Macro
#define GPIO(port) GPIOx(port)
#define GPIOx(port)	GPIO ## port  // i.e. GPIOx(A) will return GPIOA

// GPIO Register Setting Macros
#define GPIO_ODR_BIT_CLEAR	0UL
#define GPIO_ODR_BIT_SET		1UL
#define GPIO_ODR_SET( port, pin, state ) \
							FORCE_BITS( GPIO(port) -> ODR, (1UL << ((pin) * 1)), ( (state) << ((pin) * 1)) )

#define GPIO_MODE_IN  0UL
#define GPIO_MODE_OUT	1UL
#define GPIO_MODE_AF	2UL
#define	GPIO_MODE_ANALOG	3UL
#define GPIO_MODER_SET( port, pin, mode )	 	\
							FORCE_BITS( GPIO(port) -> MODER, (3UL << ((pin) * 2)), ( (mode) << ((pin) * 2)) )
							
#define GPIO_OTYPE_PP	0UL
#define GPIO_OTYPE_OD 1UL
#define GPIO_OTYPER_SET( port, pin, type )	\
							FORCE_BITS( GPIO(port) -> OTYPER, (1UL << ((pin) * 1)), ( (type) << ((pin) * 1)) )

#define GPIO_PUPD_NO 0UL
#define GPIO_PUPD_PU 1UL
#define GPIO_PUPD_PD 2UL
#define GPIO_PUPDR_SET( port, pin, pupd )		\
							FORCE_BITS( GPIO(port) -> PUPDR, (3UL << ((pin) * 2)), ( (pupd) << ((pin) * 2)) )


#define ENABLE_GPIO_CLOCK( port )	ENABLE_GPIO_CLOCKx( port )

#define ENABLE_GPIO_CLOCKx( port ) \
							RCC -> AHBENR |= RCC_AHBENR_GPIO ## port ## EN







#define DELAY_CONSTANT 5000000

void Delay_Arbitrary(void);  // this is only for blocking for loop delay.
// must introduce systick delay in lab 3!

void Delay_ms (uint32_t msec); // blocking loop delay with SysTick precision timer

#endif