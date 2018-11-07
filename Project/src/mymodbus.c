#include <stm8s.h>
#include "mb.h"
#include "mbport.h"

/**
  * @brief  UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    pxMBFrameCBTransmitterEmpty(  );
}


/**
  * @brief  UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
/*uint8_t buff[256] = {0};
uint8_t i = 0;*/
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
  /*  u8 data;
  if( UART2_GetITStatus(UART2_IT_RXNE) == SET ) {
    data = UART2_ReceiveData8();
    buff[i++] = data;
  }
  UART2_ClearITPendingBit(UART2_IT_RXNE);*/
  pxMBFrameCBByteReceived(  );
}
