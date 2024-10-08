#ifndef __MOTOR_ENCODER_H
#define __MOTOR_ENCODER_H

#include "stm32f303xe.h"
#include "utility.h"

#define ENCODER_PORT A

#define ENCODER_TIMER				TIM2
#define ENCODER_TIMER_INT		TIM2_IRQn

#define LEFT_ENCODER_CH			CCR1
#define LEFT_ENCODER_PIN 		0  				//PA0

#define RIGHT_ENCODER_CH		CCR2
#define RIGHT_ENCODER_PIN		1  				//PA1

#define ENCODER_PRIORITY 9

#define LEFT_ENC	0
#define RIGHT_ENC 1

void Encoder_Init(void);
void TIM2_IRQHandler(void);
void Encoder_CalculateSpeed(void);

extern uint32_t Global_LeftEncoderPeriod;
extern uint32_t Global_RightEncoderPeriod;

#endif
