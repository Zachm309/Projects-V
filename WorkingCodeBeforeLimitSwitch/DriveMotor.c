// Drive Motor Module Source File

#include "DriveMotor.h"
#include "utility.h"

void DriveMotor_Init(void)
{
	ENABLE_GPIO_CLOCK( MOTOR_PORT );
	
	// Motor Direction Controls
	GPIO_MODER_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_L, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_L, GPIO_OTYPE_PP );
	GPIO_PUPDR_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_L, GPIO_PUPD_NO );
	GPIO_ODR_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_L, GPIO_ODR_BIT_CLEAR );
	//FWD PIN ASSI up/down
	GPIO_MODER_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_R, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_R, GPIO_OTYPE_PP );
	GPIO_PUPDR_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_R, GPIO_PUPD_NO );
	GPIO_ODR_SET( MOTOR_PORT, MOTOR_PIN_DIR_FWD_R, GPIO_ODR_BIT_CLEAR );
	
	GPIO_MODER_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_L, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_L, GPIO_OTYPE_PP );
	GPIO_PUPDR_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_L, GPIO_PUPD_NO );
	GPIO_ODR_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_L, GPIO_ODR_BIT_CLEAR );
	//RWD PIN ASSIGN Up/DOWN
	GPIO_MODER_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_R, GPIO_MODE_OUT );
	GPIO_OTYPER_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_R, GPIO_OTYPE_PP );
	GPIO_PUPDR_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_R, GPIO_PUPD_NO );
	GPIO_ODR_SET( MOTOR_PORT, MOTOR_PIN_DIR_RWD_R, GPIO_ODR_BIT_CLEAR );
	
	
	// PWM pins, Using TIM 8 (output channel is on AF4 on PC10 and PC11
	GPIO_MODER_SET( MOTOR_PORT, MOTOR_PIN_PWM_L, GPIO_MODE_AF );
	GPIO_OTYPER_SET( MOTOR_PORT, MOTOR_PIN_PWM_L, GPIO_OTYPE_PP );
	GPIO_PUPDR_SET( MOTOR_PORT, MOTOR_PIN_PWM_L, GPIO_PUPD_NO );
	// TIM8 CH1N
	FORCE_BITS( GPIOC -> AFR[1], GPIO_AFRH_AFRH2_Msk, 0x4UL << GPIO_AFRH_AFRH2_Pos );
	
	GPIO_MODER_SET( MOTOR_PORT, MOTOR_PIN_PWM_R, GPIO_MODE_AF );
	GPIO_OTYPER_SET( MOTOR_PORT, MOTOR_PIN_PWM_R, GPIO_OTYPE_PP );
	GPIO_PUPDR_SET( MOTOR_PORT, MOTOR_PIN_PWM_R, GPIO_PUPD_NO );
	// TIM8 CH2N
	FORCE_BITS( GPIOC -> AFR[1], GPIO_AFRH_AFRH3_Msk, 0x4UL << GPIO_AFRH_AFRH3_Pos );
	
	
	
	// Configure Timer 8 for CH1N and CH2N 
	// 1. Turn o Timer 8
		RCC -> APB2ENR |= RCC_APB2ENR_TIM8EN;
	
	// 2. Program Timer 8 prescaler to count in microseconds
		TIM8 -> PSC = SystemCoreClock / (1000000UL) - 1;
		//You need -1 to allow the timer start sequence (mandatory)
	
	// 3. Set TIM8 to upcounting mode (MANDATORY!! because TIM8 default mode can be something else)
		CLEAR_BITS(TIM8 -> CR1, TIM_CR1_DIR);
	
	// 4. Set the TIM8 reload value to 1000 us period (1000 - 1 because it's repeating)
		TIM8 -> ARR = 1000UL - 1;  // 1ms
	
	// 5. Enable TIM8 ARR preload
		SET_BITS( TIM8 -> CR1, TIM_CR1_ARPE );
	
	// 6. Set TIM8 main output enabled
		SET_BITS( TIM8 -> BDTR, TIM_BDTR_MOE );
		
		
	
	// Configure CH1N (Complementary) of TIM8 for Left Wheel PWM output compare mode
	
	// 1. Select PWM mode 1 for TIM8 channel 1N
		FORCE_BITS( TIM8 -> CCMR1, TIM_CCMR1_OC1M, (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2) );
	
	// 2. Enable output compare preload on channel 2 (i.e. update counter register only on timer enabled (UE) )
		SET_BITS( TIM8 -> CCMR1, TIM_CCMR1_OC1PE );
	
	// 3. Enable the COMPLEMENTARY output channel (CH1N)
		SET_BITS( TIM8 -> CCER, TIM_CCER_CC1NE );
	
	// 4. Make CH1N active HI (standard polarity)
		CLEAR_BITS( TIM8 -> CCER, TIM_CCER_CC1NP );
	
	// 5. Set the CH1N initial PWM ON-time to 0 by clearing CCR1, so that PWM will not be outputted until the first 1ms elapsed
		TIM8 -> CCR1 = 0;
	
	
	// Configure CH2N (Complementary) of TIM8 for Right Wheel PWM output compare mode
	
	// 1. Select PWM mode 1 for TIM8 channel 1N
		FORCE_BITS( TIM8 -> CCMR1, TIM_CCMR1_OC2M, (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2) );
	
	// 2. Enable output compare preload on channel 2 (i.e. update counter register only on timer enabled (UE) )
		SET_BITS( TIM8 -> CCMR1, TIM_CCMR1_OC2PE );
	
	// 3. Enable the COMPLEMENTARY output channel (CH1N)
		SET_BITS( TIM8 -> CCER, TIM_CCER_CC2NE );
	
	// 4. Make CH1N active HI (standard polarity)
		CLEAR_BITS( TIM8 -> CCER, TIM_CCER_CC2NP );
	
	// 5. Set the CH1N initial PWM ON-time to 0 by clearing CCR1, so that PWM will not be outputted until the first 1ms elapsed
		TIM8 -> CCR2 = 0;
	
	
	// Start TIM8 CH1N and CH2N Outputs
	
	// 1. Force an update event to preload all the registers
		SET_BITS( TIM8 -> EGR, TIM_EGR_UG );
	
	// 2. Enable TIM15 to start counting
		SET_BITS( TIM8 -> CR1, TIM_CR1_CEN );
	
}


void SetMotorSpeed( uint8_t motor, uint16_t dutyCycle ) //Sets the motor speed according to the duty cycle 
{
	if(dutyCycle > MOTOR_MAX_DUTYCYCLE)
		dutyCycle = MOTOR_MAX_DUTYCYCLE;
	
	if(motor == LEFT_MOTOR)
	{
		TIM8 -> CCR1 = dutyCycle;
	}
	else if(motor == RIGHT_MOTOR)
	{
		TIM8 -> CCR2 = dutyCycle;
	}

}	




void SetMotorDir( uint8_t motor, uint8_t dir )
{
		switch(dir)
		{
			case MOTOR_FWD:
				
				if(motor == LEFT_MOTOR)
				{
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_FWD_L, GPIO_ODR_BIT_SET ); //FWD
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_RWD_L, GPIO_ODR_BIT_CLEAR );

				}
				else if(motor == RIGHT_MOTOR)
				{
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_FWD_R, GPIO_ODR_BIT_SET ); //FWD
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_RWD_R, GPIO_ODR_BIT_CLEAR );
				}
			
				break;
				
			
			case MOTOR_RWD:
				
				if(motor == LEFT_MOTOR)
				{
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_FWD_L, GPIO_ODR_BIT_CLEAR ); //RWD
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_RWD_L, GPIO_ODR_BIT_SET );
				
				}
				else if(motor == RIGHT_MOTOR)
				{
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_FWD_R, GPIO_ODR_BIT_CLEAR ); //RWD
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_RWD_R, GPIO_ODR_BIT_SET );
				}

				break;
		
			case MOTOR_STOP:
			default:
				
				if(motor == LEFT_MOTOR) //STOP
				{
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_FWD_L, GPIO_ODR_BIT_CLEAR );
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_RWD_L, GPIO_ODR_BIT_CLEAR );
				}
				else if(motor == RIGHT_MOTOR)
				{
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_FWD_R, GPIO_ODR_BIT_CLEAR );
					GPIO_ODR_SET( C, MOTOR_PIN_DIR_RWD_R, GPIO_ODR_BIT_CLEAR );
				}

				break;
			
		}

}




