/*
 * dcmotor.c
 *
 *  Created on: Oct 5, 2024
 *      Author: belal
 */
#include "dcmotor.h"
#include "pwm.h"

void DcMotor_init(void){
	/*Setup pins as output*/
	GPIO_setupPinDirection(DCMOTOR_PORT_ID,DCMOTOR_IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_PORT_ID,DCMOTOR_IN2_PIN_ID,PIN_OUTPUT);

	/*Initialized to stop*/
	GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2_PIN_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed){
	if(speed>100 || speed<0 || state>3 || state<0){
		/* Do Nothing*/
	}
	else{
		switch(state){
		case DCMOTOR_MODE_OFF:
			GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1_PIN_ID, LOGIC_LOW);
			GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2_PIN_ID, LOGIC_LOW);
			PWM_Timer0_Start(speed);
			break;
		case DCMOTOR_MODE_CW:
			GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1_PIN_ID, LOGIC_HIGH);
			GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2_PIN_ID, LOGIC_LOW);
			PWM_Timer0_Start(speed);
			break;
		case DCMOTOR_MODE_ACW:
			GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1_PIN_ID, LOGIC_LOW);
			GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2_PIN_ID, LOGIC_HIGH);
			PWM_Timer0_Start(speed);
			break;
		}


	}
}


