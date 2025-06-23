/*
 * pir.c
 *
 *  Created on: Nov 2, 2024
 *      Author: belal
 */
#include "pir.h"
#include "gpio.h"

void PIR_init(void){
	GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

uint8 PIR_getState(void){
	return GPIO_readPin(PIR_PORT_ID, PIR_PIN_ID);
}


