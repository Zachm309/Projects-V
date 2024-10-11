#include "stm32f303xe.h"

//Blaise Swan
//Ultrasoniccccccccccccccccccc

void sensorInit(void);
void start_trigger(void);
void TIM3_IRQHandler(void);
float READ_SENSOR(void);
int ultraSonicInit(void);
int ECHO_RECEIVED(void) ;
void start_trigger(void);
int ON_DEMAND_TRIGGER(void);