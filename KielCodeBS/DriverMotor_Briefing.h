// Drive Motor Module Header File

#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f303xe.h"

#define MOTOR_PORT C
#define MOTOR_PIN_PWM_L				10
#define MOTOR_PIN_PWM_R				11
#define MOTOR_PIN_DIR_FWD_L		12
#define MOTOR_PIN_DIR_RWD_L		13
#define MOTOR_PIN_DIR_FWD_R		8
#define MOTOR_PIN_DIR_RWD_R		9

#define LEFT_MOTOR 	0u
#define RIGHT_MOTOR 1u

#define MOTOR_STOP	0u
#define MOTOR_FWD		1u
#define MOTOR_RWD		2u

#define MOTOR_MAX_DUTYCYCLE	1000 // 0.1% per step

void DriveMotor_Init(void);
void SetMotorSpeed( uint8_t motor, uint16_t dutyCycle );
void SetMotorDir( uint8_t motor, uint8_t dir );

#endif
