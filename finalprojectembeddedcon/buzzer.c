/*
 * buzzer.c
 *
 *  Created on: Oct 5, 2024
 *      Author: belal
 */
#include "buzzer.h"
#include "std_types.h"


void Buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

