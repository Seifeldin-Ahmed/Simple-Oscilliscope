/*
 * GPT_prv.h
 *
 *  Created on: Sep 5, 2022
 *      Author: Dell
 */

#ifndef MCAL_GPT_TIMERS_GPT_PRV_H_
#define MCAL_GPT_TIMERS_GPT_PRV_H_

#define GPT_u8_TIMSK_REG           (*(volatile u8*)0X59) //For INTERRUPT
 #define GPT_u8_TIFR_REG          (*(volatile u8*)0X58) //For Flag                          

//******************************************************* TIMER 1 ***************************************************************
#define GPT_u8_TCCR1A              (*(volatile u8*)0X4F)

#define GPT_u8_TCCR1B              (*(volatile u8*)0X4E)

#define GPT_u16_TCNT1L             (*(volatile u16*)0x4C) //Timer/Counter1 – Counter Register Low Byte

#define GPT_u16_OCR1AL             (*(volatile u16*)0X4A)  //Timer/Counter1 – Output Compare Register A Low Byte

#define GPT_u16_OCR1BL             (*(volatile u16*)0X48)  //Timer/Counter1 – Output Compare Register B Low Byte

//**********************************************************************************************************************************



//****************************************************** TIMER 0 *******************************************************************
#define GPT_u8_TCCR0_REG           (*(volatile u8*)0X53) // Control Register

#define GPT_u8_TCNT0_REG           (*(volatile u8*)0X52) // Control Register

#define GPT_u8_OCR0_REG            (*(volatile u8*)0X5C)

#define GPT_u8_ASSR_REG           (*(volatile u8*)0X42)

//***********************************************************************************************************************************



//***************************************************** TIMER2 **********************************************************************
#define GPT_u8_TCCR2_REG           (*(volatile u8*)0X45) // Control Register

#define GPT_u8_TCNT2_REG           (*(volatile u8*)0X44)  // Control Register

#define GPT_u8_OCR2_REG            (*(volatile u8*)0X43)
//***********************************************************************************************************************************
#endif /* MCAL_GPT_TIMERS_GPT_PRV_H_ */
