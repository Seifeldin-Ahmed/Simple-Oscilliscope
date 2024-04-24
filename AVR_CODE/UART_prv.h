/*
 * UART_prv.h
 *
 *  Created on: Sep 9, 2022
 *      Author: Dell
 */

#ifndef MCAL_UART_UART_PRV_H_
#define MCAL_UART_UART_PRV_H_


#define UART_u8_UCSRA_REG                (*(volatile u8*)0X2B)

#define UART_u8_UCSRB_REG                (*(volatile u8*)0X2A)

#define UART_u8_UCSRC_REG                (*(volatile u8*)0X40)

#define UART_u8_UBRRL_REG                (*(volatile u8*)0X29)

#define UART_u8_UBRRH_REG                (*(volatile u8*)0X40)

#define UART_u8_UDR_REG                  (*(volatile u8*)0X2C)



#endif /* MCAL_UART_UART_PRV_H_ */
