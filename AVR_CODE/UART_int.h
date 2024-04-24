/*
 * UART_int.h
 *
 *  Created on: Sep 9, 2022
 *      Author: Seifeldin Ahmed
 */

#ifndef MCAL_UART_UART_INT_H_
#define MCAL_UART_UART_INT_H_


// CHECK ERROR STATUS //
typedef enum
{
	UART_OK,
	UART_NOK
}UART_tenuErrorStatus;


typedef struct
{
	u8 UART_u8ParityBit;
	u8 UART_u8PStopBits;
	u8 UART_u8DataBits;
	u8 UART_u8Mode;
	u32 UART_u32BaudRate;

}UART_tstrConfiguration;



//***************************************************** Mode Options ************************************************************
#define UART_u8_Transmite                      0x08
#define UART_u8_Recieve                        0x10
#define UART_u8_FULL_DUPLEX                    0x18
//*******************************************************************************************************************************


//***************************************************** Parity Bit Options ******************************************************
#define UART_u8_DISABLED_PARITY                0X00
#define UART_u8_EVEN_PARITY                    0X20
#define UART_u8_ODD_PARITY                     0X30
//*******************************************************************************************************************************

//**************************************************** Stop Bit Options *********************************************************
#define UART_u8_ONE_STOP_BIT                   0X00
#define UART_u8_TWO_STOP_BIT                   0X08
//*******************************************************************************************************************************



//**************************************************** Character Size Options ***************************************************
#define UART_u8_FIVE_DATA_BITS                 0X00
#define UART_u8_SIX_DATA_BITS                  0X02
#define UART_u8_SEVEN_DATA_BITS                0X04
#define UART_u8_EIGHT_DATA_BITS                0X06
#define UART_u8_NINE_DATA_BITS                 0X06
//*******************************************************************************************************************************


UART_tenuErrorStatus  UART_enuInit(UART_tstrConfiguration* Add_pstrConfigue);

UART_tenuErrorStatus  UART_enuTransmite(unsigned short int Copy_u16Data);

UART_tenuErrorStatus  UART_enuReceive(unsigned short int* Add_pu16Data);

UART_tenuErrorStatus UART_enuTransmiteString(unsigned char* Add_pu8Data);

void UART_TransmitAsync(unsigned char *Add_TransmitData);
void UART_ReceiveAsync(unsigned char *Add_ReceivedData, unsigned char Copy_Size);
#endif /* MCAL_UART_UART_INT_H_ */
