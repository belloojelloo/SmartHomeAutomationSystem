/*
 * timer.c
 *
 *  Created on: Nov 2, 2024
 *      Author: belal
 */
#include "timer.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

static void (*Timer0_CallBackPtr)(void) = NULL_PTR;
static void (*Timer1_CallBackPtr)(void) = NULL_PTR;
static void (*Timer2_CallBackPtr)(void) = NULL_PTR;

void Timer_init(const Timer_ConfigType * Config_Ptr) {
    switch (Config_Ptr->timer_ID) {
        case TIMER0:

            TCNT0 = Config_Ptr->timer_InitialValue;
            if (Config_Ptr->timer_mode == NORMAL_MODE) {
            	TCCR0 = 0;
                TIMSK |= (1<<TOIE0);
            }
            else if (Config_Ptr->timer_mode == COMPARE_MODE) {
                TCCR0 |= (1<<WGM01);
                OCR0 = Config_Ptr->timer_compare_MatchValue;
                TIMSK |= (1<<OCIE0);
            }
            TCCR0 |= Config_Ptr->timer_clock;
            break;

        case TIMER1:
            TCCR1A = 0;
            TCCR1B = 0;
            TCNT1 = Config_Ptr->timer_InitialValue;
            if (Config_Ptr->timer_mode == NORMAL_MODE) {
                TIMSK |= (1<<TOIE1);
            }
            else if (Config_Ptr->timer_mode == COMPARE_MODE) {
                TCCR1B |= (1<<WGM12);
                OCR1A = Config_Ptr->timer_compare_MatchValue;
                TIMSK |= (1<<OCIE1A);
            }
            TCCR1B |= Config_Ptr->timer_clock;
            break;

        case TIMER2:
            TCCR2 = 0;
            TCNT2 = Config_Ptr->timer_InitialValue;
            if (Config_Ptr->timer_mode == NORMAL_MODE) {
                TCCR2 |= (1<<FOC2);
                TIMSK |= (1<<TOIE2);
            }
            else if (Config_Ptr->timer_mode == COMPARE_MODE) {
                TCCR2 |= (1<<WGM21) | (1<<FOC2);
                OCR2 = Config_Ptr->timer_compare_MatchValue;
                TIMSK |= (1<<OCIE2);
            }
            TCCR2 |= Config_Ptr->timer_clock;
            break;
    }
}
void Timer_deInit(Timer_ID_Type timer_type) {
    switch (timer_type) {
        case TIMER0:
            TCCR0 = 0;
            TIMSK &= ~(1<<TOIE0) & ~(1<<OCIE0);
            break;
        case TIMER1:
            TCCR1A = 0;
            TCCR1B = 0;
            TIMSK &= ~(1<<TOIE1) & ~(1<<OCIE1A);
            break;
        case TIMER2:
            TCCR2 = 0;
            TIMSK &= ~(1<<TOIE2) & ~(1<<OCIE2);
            break;
    }
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID) {
    switch (a_timer_ID) {
        case TIMER0:
            Timer0_CallBackPtr = a_ptr;
            break;
        case TIMER1:
            Timer1_CallBackPtr = a_ptr;
            break;
        case TIMER2:
            Timer2_CallBackPtr = a_ptr;
            break;
    }
}


ISR(TIMER0_OVF_vect) {
    if (Timer0_CallBackPtr != NULL_PTR) {
        Timer0_CallBackPtr();
    }
}

ISR(TIMER0_COMP_vect) {
    if (Timer0_CallBackPtr != NULL_PTR) {
        Timer0_CallBackPtr();
    }
}

ISR(TIMER1_OVF_vect) {
    if (Timer1_CallBackPtr != NULL_PTR) {
        Timer1_CallBackPtr();
    }
}

ISR(TIMER1_COMPA_vect) {
    if (Timer1_CallBackPtr != NULL_PTR) {
        Timer1_CallBackPtr();
    }
}

ISR(TIMER2_OVF_vect) {
    if (Timer2_CallBackPtr != NULL_PTR) {
        Timer2_CallBackPtr();
    }
}

ISR(TIMER2_COMP_vect) {
    if (Timer2_CallBackPtr != NULL_PTR) {
        Timer2_CallBackPtr();
    }
}
