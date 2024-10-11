#ifndef __Macros
#define __Macros

#define SET_BITS( port, bits ) ((port) |= (bits)) //
#define CLR_BITS( port, bits ) ((port) &= (~(bits))) //
#define FLIP_BITS( port, bits ) ((port) ^= (bits)) //

#define FORCE_BITS( port, mask, value ) ((port) = ((port) & (~(mask))) | ((value) & (mask))) //
#define IS_BIT_SET( port, bits ) //
#define IS_BIT_CLEAR( port, bits ) //

#define	GPIOx( port ) GPIO ## port //
#define GPIO_MODER_SET( port, pin, mode ) \ (GPIOx( port ) -> SET_BITS(MODER,(mode << (2*pin)))) //
#define GPIO_OTYPER_SET(port, pin, mode) \ FORCE_BITS( GPIOx(port -> OTYPER), (1UL << ((pin) * 1)), ((mode) << ((pin) * 1)) )
#define GPIO_PUPDR_SET( port, pin, mode ) \ FORCE_BITS( GPIOx(port -> PUPDR), (3UL << ((pin) * 1)), ((mode) << ((pin) * 1)) )
#define Delay_ms(int millisecond) //Your precision delay function from Lab 2

#endif