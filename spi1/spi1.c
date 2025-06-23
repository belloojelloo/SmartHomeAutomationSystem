/*
 * spi1.c
 *
 *  Created on: Jan 20, 2025
 *      Author: belal
 */
#include <avr/io.h>
#include "gpio.h"
#include "std_types.h"
#include "spi1.h"
#include "common_macros.h"


void SPIM_init(void){
	SET_BIT(SPCR,SPE);
	SET_BIT(SPCR,MSTR);
	GPIO_setupPinDirection(PORTB_ID, PIN4_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_INPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT);
}



