#include "stm32f303xe.h"
#include "LED.h"
#include "LCD.h"
#include "SysClock.h"
#include "UART.h"
#include "Stepper.h"
#include "utility.h"
#include "RCServo.h"
#include "ultraSonicInit.h"
#include "DriveMotor.h"
#include "Encoder.h"
//#include "Pushbutton.h"
#include "keypad.h"

int main(void)
{
	//KeyPad_Init();
	RCServo_Init();
	UART2_INIT();
	UART2_Config();
	//LED_INIT();
	Stepper_Init();
	LCD_Init();
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	//NVIC_SetPriority(SysTick_IRQn,1);
	Encoder_Init();
	ultraSonicInit();
	UARTprintf(" Stepper Demo\n__________________\n0: Stop\n1: Clockwise Full Step\n2: Counter Clockwise Full Step\n3: Clockwise Half Step\n4: Counter Clockwise Half Step\nCMD: \n");
	int8_t stepType;
	char holderLab3;
	int16_t defaultAngle = 90;
	int16_t ticks_per_revolution = 32;
	char holder;
	char input = 'ZZZ';
	char inputCommand = '\0';
	DriveMotor_Init();
	char mode = 'm'; // 'm' for motor mode, 's' for stepper mode
	
	LCDprintf("Demo Start");
	UARTprintf("START");
//for(;;){
//UARTprintf("TEST");
//char pressedKey = KeyPad_GetKey();
//	UARTprintf("\nYou pressed %c:", pressedKey);
//}
for(;;) {
    // Input command from user to either switch modes or control the motors/servos
    inputCommand = UARTgetcND();
    RCServo_SetAngle(defaultAngle);
    // Mode switching logic
    if(inputCommand == 'm') {
        mode = 'm'; // Switch to motor mode
        UARTprintf("Motor mode selected.\n");
    } else if (inputCommand == 's') {
        mode = 's'; // Switch to stepper mode
        UARTprintf("Stepper mode selected.\n");
    }
    
    // Motor mode
    if (mode == 'm') {
			//UARTprintf("MotorLoop");
        switch(inputCommand) {
            case 'y': // L FWD
                SetMotorSpeed(LEFT_MOTOR, 750);
                SetMotorDir(LEFT_MOTOR, MOTOR_FWD);
                UARTprintf("Left Wheel Forward\n");
                break;
            
            case 'h': // L STOP
                SetMotorSpeed(LEFT_MOTOR, 0);
                SetMotorDir(LEFT_MOTOR, MOTOR_STOP);
                break;
            
            case 'n': // L RWD
                SetMotorSpeed(LEFT_MOTOR, 750);
                SetMotorDir(LEFT_MOTOR, MOTOR_RWD);
                UARTprintf("Left Wheel Backwards\n");
                break;
            
            case 'u': // R FWD
                SetMotorSpeed(RIGHT_MOTOR, 750);
                SetMotorDir(RIGHT_MOTOR, MOTOR_FWD);
                UARTprintf("Right Wheel Forward\n");
                break;
            
            case 'j': // R STOP
                SetMotorSpeed(RIGHT_MOTOR, 0);
                SetMotorDir(RIGHT_MOTOR, MOTOR_STOP);
                break;
            
            case 'k': // R RWD
                SetMotorSpeed(RIGHT_MOTOR, 750);
                SetMotorDir(RIGHT_MOTOR, MOTOR_RWD);
                UARTprintf("Right Wheel Backwards\n");
                break;
						case 'r': // read encoder
           Encoder_CalculateSpeed();
				UARTprintf("\nValues: %d %d", Global_LeftEncoderPeriod, Global_RightEncoderPeriod);
						break;
        }
    }
    
    // Stepper mode
    else if (mode == 's') {
			//UARTprintf("loop");
			Stepper_Step(stepType);
        switch(inputCommand) {
            case '0': // Stop stepper
                stepType = STEPPER_STEP_STOP;
                UARTprintf("CMD STOP\n");
                UARTprintf("Number: %d\n", stepType);
                break;
                
            case '1': // Clockwise full step
                stepType = STEPPER_STEP_CW_FULL;
                UARTprintf("CMD CW F\n");
                UARTprintf("Number: %d\n", stepType);
              //  Stepper_Step(stepType);
                break;
                
            case '2': // Counter-clockwise full step
                stepType = STEPPER_STEP_CCW_FULL;
                UARTprintf("CMD CCW F\n");
                UARTprintf("Number: %d\n", stepType);
              //  Stepper_Step(stepType);
                break;
                
            case '3': // Clockwise half step
                stepType = STEPPER_STEP_CW_HALF;
                UARTprintf("CMD CW H\n");
                UARTprintf("Number: %d\n", stepType);
               // Stepper_Step(stepType);
                break;
                
            case '4': // Counter-clockwise half step
                stepType = STEPPER_STEP_CCW_HALF;
                UARTprintf("CMD CCW H\n");
                UARTprintf("Number: %d\n", stepType);
               // Stepper_Step(stepType);
                break;
// Servo control
            case 'w': // Increase servo angle
                defaultAngle += 3;
                RCServo_SetAngle(defaultAngle);
                break;
                
            case 'q': // Decrease servo angle
                defaultAngle -= 3;
                RCServo_SetAngle(defaultAngle);
                break;
        }

    }

    Delay_ms(30); // Small delay to prevent overwhelming the system
    TOGGLE(); // Toggle LED or similar action to show the system is running
}

}
	


