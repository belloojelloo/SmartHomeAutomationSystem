/*
 * pwm.h
 *
 *  Created on: Oct 5, 2024
 *      Author: belal
 */

#ifndef PWM_H_
#define PWM_H_
#include "std_types.h"
#define PWM_OC0_PORT_ID PORTB_ID
#define PWM_OC0_PIN_ID PIN3_ID


void PWM_Timer0_Start(uint8 duty_cycle);
void PWM_init(void);

#endif /* PWM_H_ */
