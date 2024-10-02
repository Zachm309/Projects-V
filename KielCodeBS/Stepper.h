#ifndef __STEPPER_H
#define __STEPPER_H

#include "stm32f303xe.h"

#define STEPPER_STEP_STOP 0
#define STEPPER_STEP_CW_HALF 1
#define STEPPER_STEP_CCW_HALF -1
#define STEPPER_STEP_CW_FULL 2
#define STEPPER_STEP_CCW_FULL -2

#define STEP_INDEX_MASK 0x07

void Stepper_Init(void);

void Stepper_Step(int8_t stepType);

#endif
