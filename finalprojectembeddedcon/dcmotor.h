/*
 * dcmotor.h
 *
 *  Created on: Oct 5, 2024
 *      Author: belal
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "gpio.h"
#include "common_macros.h"
typedef unsigned char DcMotor_State;
#define DCMOTOR_PORT_ID PORTD_ID
#define DCMOTOR_IN1_PIN_ID PIN6_ID
#define DCMOTOR_IN2_PIN_ID PIN7_ID
#define DCMOTOR_EN_PIN_ID PIN3_ID
#define DCMOTOR_MODE_OFF 0
#define DCMOTOR_MODE_CW 1
#define DCMOTOR_MODE_ACW 2

void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DCMOTOR_H_ */
