/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */
/*
 * reference follow link for freeModbus use in STM8S.
 * <http://bbs.ednchina.com/BLOG_ARTICLE_3016783.HTM>
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm8s.h"
#include "stm8s_uart2.h"


/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
  if(xRxEnable == TRUE)
  {
//    UART2->CR2 |= (1<<5);       // UART2_ITConfig(UART2_IT_RXNE, ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
  }
  else
  {
//    UART2->CR2 &= ~(1<<5);       // UART2_ITConfig(UART2_IT_RXNE, DISABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR, DISABLE);
  }
  if(xTxEnable == TRUE)
  {
//    UART2->CR2 |= (1<<6);        // UART2_ITConfig(UART2_IT_TXE, ENABLE);
    UART2_ITConfig(UART2_IT_TXE, ENABLE);
  }
  else
  {
//    UART2->CR2 &= ~(1<<6);       //UART2_ITConfig(UART2_IT_TXE, DISABLE);
    UART2_ITConfig(UART2_IT_TXE, DISABLE);
  }
}

/* Serial initialization */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{

  UART2_WordLength_TypeDef WordLength=UART2_WORDLENGTH_8D;
  UART2_Parity_TypeDef Parity=UART2_PARITY_NO;
  
  if(ucDataBits!=8)WordLength=UART2_WORDLENGTH_9D;
  
  switch(eParity)
  {
  case MB_PAR_NONE:
    Parity = UART2_PARITY_NO;
    break;
  case MB_PAR_ODD:
    Parity = UART2_PARITY_ODD;
    break;
  case MB_PAR_EVEN:
    Parity = UART2_PARITY_EVEN;
    break;
  default:
    Parity = UART2_PARITY_NO;
    break;
  }
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
  UART2_DeInit();
  
  UART2_Init((uint32_t)ulBaudRate, WordLength,UART2_STOPBITS_1, Parity,
                   UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);

  /* Enable UART */
  UART2_Cmd(ENABLE);
  

  return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */  
//  while((UART2->SR & 0x80)==0x00)
    UART2->DR = ucByte;
//  UART2_SendData8(TxBuffer[TxCounter++]);
  return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
  *pucByte = UART2->DR;
  return TRUE;
}

