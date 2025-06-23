

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */

typedef uint32 UART_BaudRateType;


typedef enum{
	fivebit,
	sixbit,
	sevenbit,
	eightbit,
	ninebit
}UART_BitDataType;
typedef enum {
    NONE,
    EVEN,
    ODD,
} UART_ParityType;
typedef enum {
    ONE_STOP_BIT,
    TWO_STOP_BIT,
} UART_StopBitType;
typedef enum {
	openclose =2,
	passwordchange,
	wrongpasstryagain,
	wrongpasstryagainchange,
	buzzerlock,
	ready,
	iamready,
	unconditionalopen,

} commands;
typedef struct {

UART_ParityType parity;
UART_BitDataType bit_data;
UART_BaudRateType baud_rate;
UART_StopBitType stop_bit;
}UART_ConfigType;
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_H_ */
