/*
 * main.c
 *
 *  Created on: Nov 2, 2024
 *      Author: belal
 *
 *  Description:
 *  ------------
 *  This program is the HMI (Human-Machine Interface) side of a door lock system.
 *  It interacts with the user via LCD and keypad, and communicates with a Control ECU
 *  through UART. It handles password creation, verification, and door control logic.
 *
 *  Modules Used:
 *  - lcd.h: For LCD display
 *  - timer.h: For handling delays with timer1
 *  - uart.h: UART communication with Control ECU
 *  - util/delay.h: Basic delays
 */

#include "lcd.h"
#include <avr/io.h>
#include "timer.h"
#include <util/delay.h>
#define PASS_SIZE 5
#include "uart.h"

/* Global flags and variables */
uint8 result = 0;
uint8 attempts = 0;
uint8 disabled = FALSE;
uint8 locked = FALSE;
uint8 passwordset = FALSE;
uint8 selection = 0;
uint8 status = 0;
uint8 enter = 0;
volatile uint8 secs = 0;

/* Timer configuration for 1-second intervals */
Timer_ConfigType timer15 = {0, 39061, TIMER1, F_CPU_1024, COMPARE_MODE };

/* Send a single byte over UART using handshaking */
void senduart(uint8 mail){
	UART_sendByte(ready);
	while(UART_recieveByte() != iamready);
	UART_sendByte(mail);
}

/* Receive a single byte over UART using handshaking */
uint8 receiveuart(void){
	while(UART_recieveByte() != ready);
	UART_sendByte(iamready);
	return UART_recieveByte();
}

/* Send a full password over UART */
void sendpassUART(uint8 *string){
	UART_sendByte(ready);
	while(UART_recieveByte() != iamready);
	for (uint8 counter = 0; counter < PASS_SIZE; counter++){
		UART_sendByte(string[counter]);
	}
}

/* Receive a full password from UART */
void receivepassUART(uint8 *Str){
	while(UART_recieveByte() != ready);
	UART_sendByte(iamready);
	for (uint8 counter = 0; counter < PASS_SIZE; counter++){
		Str[counter] = UART_recieveByte();
	}
}

/* Timer callback: Stops the motor after 3 seconds */
void timermotorstop(void){
	secs++;
	if(secs >= 3){
		Timer_deInit(TIMER1);
		secs = 0;
	}
}

/* General-purpose timer callback (increments seconds) */
void timercallback(void){
	secs++;
}

/* Timer callback: Stops the buzzer after 12 seconds */
void timerbuzzer(void){
	secs++;
	if(secs >= 12){
		Timer_deInit(TIMER1);
		secs = 0;
	}
}

/* Ask user to input password via keypad */
void askPassword(char *password){
	for (uint8 i = 0; i < PASS_SIZE; i++) {
		password[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}
	while (KEYPAD_getPressedKey() != 13);  // Wait for Enter
	_delay_ms(500);
}

/* Sequence for opening and closing the door */
void dooropen(void){
	attempts = 0;
	LCD_clearScreen();

	LCD_displayString("Door is unlocking...");
	delay_seconds(3);

	LCD_clearScreen();
	LCD_displayString("Waiting for people to enter");
	receiveuart();

	LCD_clearScreen();
	LCD_displayString("Door is locking");
	delay_seconds(3);
}

/* Blocking delay using Timer1 */
void delay_seconds(uint8 seconds) {
	secs = 0;
	Timer_init(&timer15);
	Timer_setCallBack(timercallback, TIMER1);
	while (secs < seconds);
	Timer_deInit(TIMER1);
}

/* Password creation flow with confirmation */
void createpass(void){
	char password[PASS_SIZE];
	char passwordReenter[PASS_SIZE];

	while(!passwordset){
		LCD_clearScreen();
		LCD_displayString("plz enter pass:");
		LCD_moveCursor(1, 0);
		askPassword(password);
		_delay_ms(10);

		LCD_clearScreen();
		sendpassUART(password);

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "plz re-enter the");
		LCD_displayStringRowColumn(1, 0, "same pass:");
		LCD_moveCursor(1, 10);
		askPassword(passwordReenter);
		sendpassUART(passwordReenter);

		LCD_clearScreen();
		result = receiveuart();
		if(result == 1){
			passwordset = 1;
		}
	}
}

/* Main program */
int main(void){
	UART_ConfigType uartconfig;
	uartconfig.bit_data = eightbit;
	uartconfig.parity = NONE;
	uartconfig.stop_bit = ONE_STOP_BIT;
	uartconfig.baud_rate = 9600;

	UART_init(&uartconfig);
	LCD_init();

	char password[PASS_SIZE];
	char passwordReenter[PASS_SIZE];

	SREG |= (1<<7);  // Enable global interrupts

	createpass();

	while(1){
		if(passwordset == 0){
			createpass();
		}

		while(passwordset){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "+ : OPEN DOOR");
			LCD_displayStringRowColumn(1, 0, "- : CHANGE PASS");
			selection = KEYPAD_getPressedKey();
			_delay_ms(500);

			/* Handle door open request */
			if(selection == '+'){
				senduart(openclose);
				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				LCD_moveCursor(1, 0);
				askPassword(password);
				sendpassUART(password);
				status = receiveuart();

				if(!status){
					_delay_ms(10);
					senduart(wrongpasstryagain);

					LCD_clearScreen();
					LCD_displayString("plz enter pass:");
					LCD_moveCursor(1, 0);
					askPassword(password);
					_delay_ms(10);
					sendpassUART(password);
					_delay_ms(60);
					status = receiveuart();

					if(!status){
						_delay_ms(10);
						senduart(wrongpasstryagain);

						LCD_clearScreen();
						LCD_displayString("plz enter pass:");
						LCD_moveCursor(1, 0);
						askPassword(password);
						sendpassUART(password);
						status = receiveuart();

						if(!status){
							_delay_ms(20);
							senduart(buzzerlock);
							LCD_clearScreen();
							LCD_displayStringRowColumn(0, 0, "ERROR, wrong");
							LCD_displayStringRowColumn(1, 0, "attempts");
							delay_seconds(12);
						}
						else{
							dooropen();
						}
					}
					else{
						dooropen();
					}
				}
				else{
					dooropen();
				}
			}

			/* Handle password change request */
			else if(selection == '-'){
				senduart(passwordchange);

				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				LCD_moveCursor(1, 0);
				askPassword(password);
				sendpassUART(password);
				status = receiveuart();

				if(!status){
					_delay_ms(10);
					senduart(wrongpasstryagainchange);

					LCD_clearScreen();
					LCD_displayString("plz enter pass:");
					LCD_moveCursor(1, 0);
					askPassword(password);
					_delay_ms(10);
					sendpassUART(password);
					_delay_ms(60);
					status = receiveuart();

					if(!status){
						_delay_ms(10);
						senduart(wrongpasstryagainchange);

						LCD_clearScreen();
						LCD_displayString("plz enter pass:");
						LCD_moveCursor(1, 0);
						askPassword(password);
						_delay_ms(10);
						sendpassUART(password);
						_delay_ms(60);
						status = receiveuart();

						if(!status){
							_delay_ms(20);
							senduart(buzzerlock);
							LCD_clearScreen();
							LCD_displayString("ERROR, wrong attempts");
							delay_seconds(12);
						}
						else{
							passwordset = 0;
							continue;
						}
					}
					else{
						passwordset = 0;
						continue;
					}
				}
				else{
					passwordset = 0;
					continue;
				}
			}
		}
	}
}
