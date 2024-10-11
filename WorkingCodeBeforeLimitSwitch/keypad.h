
#ifndef __KeyPad_H
#define __KeyPad_H

#include "stm32f303xe.h"

void KeyPad_Init(void);
uint8_t KeyPad_MatrixScan(void);
uint8_t KeyPad_GetKey(void);

#endif
