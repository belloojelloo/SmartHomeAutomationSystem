/*
 * timer.h
 *
 *  Created on: Nov 2, 2024
 *      Author: belal
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2
#define NORMAL_MODE 0
#define COMPARE_MODE 1
typedef uint8 Timer_ID_Type;
typedef uint8 Timer_ModeType;
typedef uint8 Timer_ClockType;
typedef struct
{
uint16 timer_InitialValue;
uint16 timer_compare_MatchValue;
Timer_ID_Type timer_ID;
Timer_ClockType timer_clock;
Timer_ModeType timer_mode;
}Timer_ConfigType;
typedef enum {
    NO_CLOCK,
    F_CPU_1,
    F_CPU_8,
    F_CPU_64,
    F_CPU_256,
    F_CPU_1024,
    EXTERNAL_CLOCK_FALLING,
    EXTERNAL_CLOCK_RISING
} Timer_Clock;
void Timer_init(const Timer_ConfigType * Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );


#endif /* TIMER_H_ */
