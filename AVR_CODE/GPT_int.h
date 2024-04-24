/*
 * GPT_int.h
 *
 *  Created on: Sep 5, 2022
 *      Author: Dell
 */
//General Purpose Timers

#ifndef MCAL_GPT_TIMERS_GPT_INT_H_
#define MCAL_GPT_TIMERS_GPT_INT_H_
#include "STD_Types.h"

typedef enum

{
	GPT_OK,
	GPT_NOK
}GPT_tenuErrorStatus;

typedef struct
{
  u8 Local_u8Prescaler;
  u8 Local_u8Mode;
  u8 Local_u8CompareOutputMode;
  u8 Local_u8CompareValue ;
  u8 Local_u8Interrupt;
  u8 Local_u8DesiredPin;
}GPT_tstrOptions;



// Macros for pins //
#define GPT_u8_OC0     0
#define GPT_u8_OC1A    1
#define GPT_u8_OC1B    2
#define GPT_u8_OC2     3

// macros for timers
#define GPT_u8_TIMER0                    0
#define GPT_u8_TIMER1                    1
#define GPT_u8_TIMER2                    2

// macros for mode
#define GPT_u8_NORMAL                   0X00
#define GPT_u8_CTC                      0X08

//****************************************************** Compare Output Mode Section ***************************************************
//**************** Timer 0 ***********************
/* compare output (OC0) mode options */
#define GPT_OC0_NORMAL_PORT_CONNECTION      0X00
#define GPT_u8_OC0_TOGGLE                   0X10
#define GPT_u8_OC0_CLEAR                    0X20
#define GPT_u8_OC0_SET                      0X30

//***************** Timer 1 **********************
/* compare output (OC1A) mode options */
#define GPT_OC1A_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC1A_TOGGLE_ON_COMPARE_MATCH	0X40
#define GPT_OC1A_CLEAR_ON_COMPARE_MATCH		0X80
#define GPT_OC1A_SET_ON_COMPARE_MATCH		0XC0

/* compare output (OC1B) mode options */
#define GPT_OC1B_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC1B_TOGGLE_ON_COMPARE_MATCH	0X10
#define GPT_OC1B_CLEAR_ON_COMPARE_MATCH		0X20
#define GPT_OC1B_SET_ON_COMPARE_MATCH		0X30

//***************** Timer 2 *********************
/* compare output (OC2) mode options */
#define GPT_OC2_NORMAL_PORT_CONNECTION		0X00
#define GPT_OC2_TOGGLE_ON_COMPARE_MATCH		0X10
#define GPT_OC2_CLEAR_ON_COMPARE_MATCH		0X20
#define GPT_OC2_SET_ON_COMPARE_MATCH		0X30



//************************************************************** Interrupt Sections ****************************************************

//****************************** Timer 0 *********************************
/* timer0 output compare match interrupt status options */
#define GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE		0X02
#define GPT_TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE		0X00

/* timer0 overflow interrupt status options */
#define GPT_TIMER0_OVERFLOW_INTERRUPT_ENABLE		0X01
#define GPT_TIMER0_OVERFLOW_INTERRUPT_DISABLE		0X00

//****************************** Timer 1 *********************************
/* timer1 output compare match interrupt status options */
#define GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE	0X10
#define GPT_TIMER1A_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE	0X00

/* timer1 output compare match interrupt status options */
#define GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE	0X08
#define GPT_TIMER1B_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE	0X00

/* timer1 overflow interrupt status options */
#define GPT_TIMER1_OVERFLOW_INTERRUPT_ENABLE		0X04
#define GPT_TIMER1_OVERFLOW_INTERRUPT_DISABLE		0X00

//****************************** Timer 2 *********************************
/* timer2 output compare match interrupt status options */
#define GPT_TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE		0X80
#define GPT_TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_DISABLE		0X00

/* timer2 overflow interrupt status options */
#define GPT_TIMER2_OVERFLOW_INTERRUPT_ENABLE		0X40
#define GPT_TIMER2_OVERFLOW_INTERRUPT_DISABLE		0X00



// NEW FOR TIMER 2
#define GPT_Asynchronise_Active   0x08
#define GPT_Asynchronise_DeActive 0x00

#define GPT_Asynchronise_Option GPT_Asynchronise_DeActive 


//******************************************************* Prescaler Section ***********************************************************
//*************** macros for Prescaler for Timer 0 and Timer 1 ***************/
#define GPT_u8_NO_CLOCK_SOURCE                          0X00
#define GPT_u8_NO_PRESCALER_1                           0X01
#define GPT_u8_NO_PRESCALER_8                           0X02
#define GPT_u8_NO_PRESCALER_64                          0X03
#define GPT_u8_NO_PRESCALER_256                         0X04
#define GPT_u8_NO_PRESCALER_1024                        0X05
#define GPT_EXTERNAL_CLOCK_SOURCE_FALLING_EDGE          0X06  // FOR TIMER 0 and TIMER 1 ONLY
#define GPT_EXTERNAL_CLOCK_SOURCE_RISING_EDGE           0X07 // FOR TIMER 0 and TIMER 1 ONLY


//*************** macros for Prescaler for Timer 2 **************************/
#define GPT_CHANNEL2_NO_CLK_SOURCE								0X00
#define GPT_CHANNEL2_PRESCALER_1								0X01
#define GPT_CHANNEL2_PRESCALER_8								0X02
#define GPT_CHANNEL2_PRESCALER_32								0X03
#define GPT_CHANNEL2_PRESCALER_64								0X04
#define GPT_CHANNEL2_PRESCALER_128								0X05
#define GPT_CHANNEL2_PRESCALER_256								0X06
#define GPT_CHANNEL2_PRESCALER_1024								0X07


//**************************************************************************************************************************************

GPT_tenuErrorStatus GPT_enuConfigure(u8 Copy_u8Timer,GPT_tstrOptions* Add_strOptions);


GPT_tenuErrorStatus GPT_enuStartTimer(u8 Copy_u8Timer,u32 Copy_time_in_ms);


GPT_tenuErrorStatus GPT_enuStopTimer(u8 Copy_u8Timer);


GPT_tenuErrorStatus GPT_enuRegisterCBF(u8 Copy_u8Timer,func_ptr Add_pfuncCBF);




/* Name: Gpt_enuDelay_ms
 * Description: halt the processor for the desired time in ms
 * Arguments:
 * 				first arguments: the timer user want to use for delay, it can be GPT_TIMER0 or GPT_TIMER1 or GPT_TIMER02
 * 				second argument: the delay time in ms
 * Return: Error Status --> OK or NOk
 */
GPT_tenuErrorStatus GPT_enuDelay_ms(u8 Copy_u8ChannelNumber, u16 Copy_u16TimeMs);


// New Functions 
GPT_tenuErrorStatus GPT_enuStartTimer_MultiTimes(u8 Copy_u8Timer,u32 Copy_time1_in_ms,u32 Copy_time2_in_ms,u32 Copy_time3_in_ms);
GPT_tenuErrorStatus GPT_enuRegisterCBF_MultiTimes(u8 Copy_u8Timer,func_ptr Add_pfuncCBF1,func_ptr Add_pfuncCBF2,func_ptr Add_pfuncCBF3);



u32 TIMER_Ticks(u8 Copy_Timer);


#endif /* MCAL_GPT_TIMERS_GPT_INT_H_ */
