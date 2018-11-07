#include "modbusTrans.h"

 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  u8 data;
  if( UART2_GetITStatus(UART2_IT_RXNE) == SET ) {
    data = UART2_ReceiveData8();
    switch( uart_step ) {
    case UART_STEP_WAIT_HEAD_0:
      if( data == MULTI_MESSAGE_HEAD ) 
      {
        uart_step = UART_STEP_WAIT_HEAD_1;
        uartReceiveDataBuf[msgWPtr][uartDataPtr++] = data;
      }
      break;
    case UART_STEP_WAIT_HEAD_1:
      if( data == MULTI_MESSAGE_CODE )
      {
        uart_step = UART_STEP_WAIT_LEN;
        uartReceiveDataBuf[msgWPtr][uartDataPtr++] = data;
      }
      else {
        uartDataPtr = 0;
        uart_step = UART_STEP_WAIT_HEAD_0;
      }
      break;
    case UART_STEP_WAIT_LEN:
      if( data > 1 && data < MAX_UART_BUF_SIZE ) {
        //uartDataPtr = 0;
        uartReceiveDataBuf[msgWPtr][uartDataPtr++] = data;
        uart_step = UART_STEP_WAIT_PAYL;
      } else {
        uartDataPtr = 0;
        uart_step = UART_STEP_WAIT_HEAD_0;
      }
      break;
    case UART_STEP_WAIT_PAYL:   
      uartReceiveDataBuf[msgWPtr][uartDataPtr++] = data;    
      if( uartDataPtr == uartReceiveDataBuf[msgWPtr][2]+UART_STEP_WAIT_PAYL ) uart_step = UART_STEP_WAIT_CHECKSUM0;
      break;
    case UART_STEP_WAIT_CHECKSUM0:
      uartReceiveDataBuf[msgWPtr][uartDataPtr++] = data;
      uart_step = UART_STEP_WAIT_CHECKSUM1;
      break;
    case UART_STEP_WAIT_CHECKSUM1:
      uartReceiveDataBuf[msgWPtr][uartDataPtr++] = data;
      uart_step = UART_STEP_WAIT_CHECKSUM1;
      msgWPtr = (msgWPtr+1)%MSG_NUM;
      msgLen++;
      uartDataPtr = 0;
      uart_step = UART_STEP_WAIT_HEAD_0;
      break;
    default:
      uartDataPtr = 0;
      uart_step = UART_STEP_WAIT_HEAD_0;
      break;
    }
    UART2_ClearITPendingBit(UART2_IT_RXNE);
  }
}
