/*
 * buzzer.h
 *
 *  Created on: Oct 5, 2024
 *      Author: belal
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "gpio.h"
#include "std_types.h"

#define BUZZER_PORT_ID PORTC_ID
#define BUZZER_PIN_ID PIN7_ID

void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);
#endif /* BUZZER_H_ */
