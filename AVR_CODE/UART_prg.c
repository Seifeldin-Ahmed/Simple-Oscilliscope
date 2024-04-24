/*
 * UART_prg.c
 *
 *  Created on: Fep 2, 2023
 *      Author: Seifeldin Ahmed
 */

#include "STD_Types.h"
#include "UART_int.h"
#include "UART_prv.h"
#define F_CPU 8000000UL
#include <util/delay.h>
UART_tenuErrorStatus  UART_enuInit(UART_tstrConfiguration* Add_pstrConfigue)
{
	UART_tenuErrorStatus Local_enuErrorStatus= UART_OK;

	if(Add_pstrConfigue==NULL)
	{
		Local_enuErrorStatus= UART_NOK;
	}
	else
	{

		
		u16 Local_u16UBRR = (F_CPU/(16*Add_pstrConfigue->UART_u32BaudRate))- 1;
		UART_u8_UBRRH_REG = (u8)(Local_u16UBRR>>8);
		UART_u8_UBRRL_REG = (u8)(Local_u16UBRR);
		
		//URSEL Set to 1           //Set Parity                    //Set Stop Bits                  // Set Character Size
		UART_u8_UCSRC_REG  = (1<<7) | Add_pstrConfigue->UART_u8ParityBit | Add_pstrConfigue->UART_u8PStopBits | Add_pstrConfigue->UART_u8DataBits ;

		//check 9 bit
		if(Add_pstrConfigue->UART_u8DataBits == UART_u8_NINE_DATA_BITS)
		{
			UART_u8_UCSRB_REG |=0x04;
		}

		//Choose transmit or Receive
		UART_u8_UCSRB_REG = Add_pstrConfigue->UART_u8Mode;
		
	}

	return Local_enuErrorStatus;
}

UART_tenuErrorStatus  UART_enuTransmite(u16 Copy_u16Data)
{
	UART_tenuErrorStatus Local_enuErrorStatus = UART_OK;

	if(Copy_u16Data> 511)
	{
		Local_enuErrorStatus= UART_NOK;
	}
	else
	{
		// polling on the flag >>> wait for UDR to be empty
		while(((UART_u8_UCSRA_REG >> 5) & 0x01 )== 0); //bit5 is UDRE (USART Data Register Empty)

		UART_u8_UCSRB_REG &= ~(1<<0);  // Clear TXB8
		if ( Copy_u16Data & 0x0100 )
		{
			UART_u8_UCSRB_REG |= 0x01; //Set TXB8
		}
		/* Put data into UDR Register */
		UART_u8_UDR_REG = (u8)(Copy_u16Data);
	}

	return Local_enuErrorStatus;
}

UART_tenuErrorStatus  UART_enuReceive(u16* Add_pu16Data)
{
	UART_tenuErrorStatus Local_enuErrorStatus= UART_OK;

	if(Add_pu16Data==NULL)
	{
		Local_enuErrorStatus= UART_NOK;
	}
	else
	{

		// poling on the flag >>> wait for data to be received
		while(((UART_u8_UCSRA_REG >> 7) & 0x01 )== 0); // bit7 is RXC(RX Complete)
		// check Data over Run error and  frame error
		if(((UART_u8_UCSRA_REG>>3) & 0x01) || ((UART_u8_UCSRA_REG>>4) & 0x01)) // bit3 is DOR , bit4 is FE
		{
			Local_enuErrorStatus= UART_NOK;
			u8 dummy;
			dummy = UART_u8_UDR_REG; // to clear the RXC Flag
		}
		else
		{
			//bit1 is RXB8
			*Add_pu16Data =(((UART_u8_UCSRB_REG>>1) & 0X01) << 8); // get bit number 9
			*Add_pu16Data |= UART_u8_UDR_REG;     // get Lower byte
		}
	}

	return Local_enuErrorStatus;
}

UART_tenuErrorStatus UART_enuTransmiteString(u8* Add_pu8Data)
{
	UART_tenuErrorStatus Local_enuErrorStatus= UART_OK;

	if(Add_pu8Data==NULL)
	{
		Local_enuErrorStatus= UART_NOK;
	}
	else
	{
		while(*Add_pu8Data  != '/0')
		{
			UART_enuTransmite(*Add_pu8Data);
			Add_pu8Data++;
			_delay_ms(50);
		}
	}

	return Local_enuErrorStatus;
}


u8 *UART_TransmitData;
u8 *UART_ReceivedData;
u8 UART_ReceivedDataSize;

void UART_TransmitAsync(u8 *Add_TransmitData)
{
	// Remember The Address Of Data To Be Transmitted (For Interrupt To Use)
	UART_TransmitData = Add_TransmitData;

	// Enable Transmit Complete & Data Register Empty Interrupts
	UART_u8_UCSRB_REG |= ((1 << 6) | (1 << 5)); //bit6: TXCIE(TX Complete interrupt enable) , bit5: UDRIE(USART DATA REGISTER INTERRUPT ENABLE)
}

void UART_ReceiveAsync(u8 *Add_ReceivedData, u8 Copy_Size)
{
	// Remember The Address Of Where Received Data To Be Stored (For Interrupt To Use)
	UART_ReceivedData = Add_ReceivedData;
	UART_ReceivedDataSize = Copy_Size;

	// Enable Receive Complete Interrupts
	UART_u8_UCSRB_REG |= (1 << 7);  //bit7: RXCIE(RX Complete interrupt enable)
}

// --------------------------------------------------------- ISR --------------------------------------------------------- //

// Receive Complete Interrupt Request
void __vector_13(void)     __attribute__((signal,used));
void __vector_13(void)
{
	// Copy Received Data From UDR
	*UART_ReceivedData = UART_u8_UDR_REG;
	UART_ReceivedData++;
	if (--UART_ReceivedDataSize == 0)
	// Disable Receive Complete Interrupt
	UART_u8_UCSRB_REG &= ~(1 << 7); //bit7: RXCIE(RX Complete interrupt enable)
}

// Data Register Empty Interrupt Request
void __vector_14(void)     __attribute__((signal,used));
void __vector_14(void)
{
	if (*UART_TransmitData != '\0')
	{
		UART_u8_UDR_REG = *UART_TransmitData;
		UART_TransmitData++;
	}
}

// Transmit Complete Interrupt Request
void __vector_15(void)     __attribute__((signal,used));
void __vector_15(void)
{
	// Disable Transmit Complete & Data Register Empty Interrupts
	UART_u8_UCSRB_REG &= ~((1 << 6) | (1 << 5));   //bit6: TXCIE(TX Complete interrupt enable) , bit5: UDRIE(USART DATA REGISTER INTERRUPT ENABLE)
}

