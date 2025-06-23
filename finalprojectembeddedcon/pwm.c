/*
 * pwm.c
 *
 *  Created on: Oct 5, 2024
 *      Author: belal
 */
#include "pwm.h"
#include <avr/io.h>
#include "common_macros.h"
#include "gpio.h"
void PWM_init(void){
	TCNT0 = 0;
		    /* Configure the OC0 pin as output */
		    GPIO_setupPinDirection(PWM_OC0_PORT_ID, PWM_OC0_PIN_ID, PIN_OUTPUT);
			    /* Set timer0 to Fast PWM */


			    /* Set prescaler to 64 and non-inverting mode */
			    TCCR0 = (1<<CS01) | (1<<CS00) | (1<<COM01) | (1<<WGM00) | (1<<WGM01);
}
void PWM_Timer0_Start(uint8 duty_cycle){

	 duty_cycle = (uint8)((uint16)(duty_cycle * 255)/ 100);

	    OCR0 = duty_cycle;







	}




