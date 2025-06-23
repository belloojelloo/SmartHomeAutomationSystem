/*
 * maincont.c
 *
 *  Created on: Nov 3, 2024
 *      Author: belal
 */
#include "buzzer.h"
#include <avr/io.h>
#include "dcmotor.h"
#include "external_eeprom.h"
#include "pir.h"
#include "pwm.h"
#include "timer.h"
#include "twi.h"
#include "uart.h"
#include <util/delay.h>
#include <string.h>
#define PASS_SIZE 5
#define address 0x01
uint8 counter;
uint8 attempts = 0;
uint8 passwordset = 0;
uint8 menuselect = 0;
uint8 flag = 0;

Timer_ConfigType timer15 = {0,39061,TIMER1, F_CPU_1024,COMPARE_MODE};
volatile uint8 secs = 0;
char savedPass[PASS_SIZE];
void senduart(uint8 mail){
	UART_sendByte(ready);
	while( UART_recieveByte() != iamready );
	UART_sendByte(mail);
}
uint8 receiveuart(void){
	while(UART_recieveByte() != ready);
	UART_sendByte(iamready);
	uint8 sent = UART_recieveByte();
	return sent;
}

void receivepassUART(uint8 *Str){
	while(UART_recieveByte() != ready);
	UART_sendByte(iamready);
	for (uint8 counter = 0; counter < PASS_SIZE; counter++)
	{
		Str[counter] = UART_recieveByte();
	}
}
void timercallback(void){
	secs++;
}
void delay_seconds(uint8 seconds) {
	secs = 0;
	Timer_init(&timer15);
	Timer_setCallBack(timercallback, TIMER1);
	while (secs < seconds);
	Timer_deInit(TIMER1);
}
void timermotoroff(void){
	secs++;
	if(secs>=3){
		secs = 0;
		DcMotor_Rotate(DCMOTOR_MODE_OFF,0);
		Timer_deInit(TIMER1);
	}
}
void timerbuzzer(void){
	secs++;
	if(secs>=12){
		secs = 0;
		Buzzer_off();
		Timer_deInit(TIMER1);
	}
}
void readPass(char *password) {
	for (counter = 0; counter < PASS_SIZE; counter++) {
		EEPROM_readByte(address + counter,&password[counter]);
		_delay_ms(10);
	}
}
void unconditionalopens(void){
		DcMotor_Rotate(DCMOTOR_MODE_CW,100);
		delay_seconds(3);
		DcMotor_Rotate(DCMOTOR_MODE_OFF,0);
		while(!PIR_getState());
		while(PIR_getState());
		senduart(1);
		DcMotor_Rotate(DCMOTOR_MODE_ACW,100);
		delay_seconds(3);
		DcMotor_Rotate(DCMOTOR_MODE_OFF,0);
	}

int main(void){
	SREG |= (1<<7);
	char receivedPass[PASS_SIZE];
	char receivedPass2[PASS_SIZE];
	char receive[PASS_SIZE];
	UART_ConfigType uartconfig;
	uartconfig.bit_data = eightbit;
	uartconfig.parity =NONE;
	uartconfig.stop_bit = ONE_STOP_BIT;
	uartconfig.baud_rate = 9600;
	UART_init(&uartconfig);
	TWI_ConfigType twiconfig = {address, 0};
	TWI_init(&twiconfig);
	Buzzer_init();
	GPIO_setupPinDirection(PORTB_ID,3,PIN_OUTPUT);

	PWM_init();
	DcMotor_init();
	PIR_init();
	DcMotor_Rotate(DCMOTOR_MODE_OFF,0);


	while(1){
		if(passwordset == 0){

			receivepassUART(receivedPass);
			receivepassUART(receivedPass2);



			if(memcmp(receivedPass, receivedPass2, PASS_SIZE) == 0){

				for (counter = 0; counter < PASS_SIZE; counter++) {
					EEPROM_writeByte(address + counter, receivedPass[counter]);
					_delay_ms(10);
				}
				senduart(1);
				passwordset = 1;
			}
			else{
				senduart(0);
			}
		}

		if(passwordset == 1){
			menuselect = receiveuart();
			if(menuselect == openclose){

				receivepassUART(receive);
				readPass(savedPass);
				if(memcmp(receive, savedPass, PASS_SIZE) == 0){
					senduart(1);
					unconditionalopens();
				}
				else{

					senduart(0);


				}
			}
			else if(menuselect == passwordchange){
				receivepassUART(receivedPass);
				readPass(savedPass);
				if(memcmp(receivedPass, savedPass, PASS_SIZE) == 0){
					_delay_ms(80);
					senduart(1);
					passwordset = 0;
					continue;
				}
				else{
					senduart(0);
				}
			}
			else if(menuselect == wrongpasstryagain){

				receivepassUART(receivedPass);
				readPass(savedPass);

				if(memcmp(receivedPass, savedPass, PASS_SIZE) == 0){
					senduart(1);
					unconditionalopens();
				}
				else{

					senduart(0);
				}
			}
			else if(menuselect == wrongpasstryagainchange){
				receivepassUART(receivedPass);
				readPass(savedPass);
				if(memcmp(receivedPass, savedPass, PASS_SIZE) == 0){
					senduart(1);
				}
				else{

					senduart(0);
				}
			}
			else if(menuselect == buzzerlock){
				Buzzer_on();
				delay_seconds(12);
				Buzzer_off();

			}

		}










	}
}
