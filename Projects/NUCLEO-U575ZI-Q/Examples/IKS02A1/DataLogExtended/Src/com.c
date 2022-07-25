/**
  ******************************************************************************
  * @file    com.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides communication functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "com.h"

/** @addtogroup X_NUCLEO_IKS02A1_Examples X_NUCLEO_IKS02A1 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define Uart_Msg_Max_Size TMsg_MaxLen

/* Private macro -------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef HdmaRx;
DMA_HandleTypeDef HdmaTx;

volatile uint8_t UartRxBuffer[UART_RxBufferSize];
volatile uint32_t UsartBaudRate = 921600;
TUart_Engine UartEngine;

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t UartTxBuffer[UART_TxBufferSize];

/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
 * @brief  Check if a message is received via UART
 * @param  Msg the pointer to the message to be received
 * @retval 1 if a complete message is found, 0 otherwise
 */
int UART_ReceivedMSG(TMsg *Msg)
{
  uint16_t i, j, k, j2;
  uint16_t dma_counter, length;
  uint8_t data;
  uint16_t source = 0;
  uint8_t inc;

  if (Get_DMA_Flag_Status(hcom_uart[COM1].hdmarx) == (uint32_t)RESET)
  {
    dma_counter = (uint16_t)UART_RxBufferSize - (uint16_t)Get_DMA_Counter(hcom_uart[COM1].hdmarx);

    if (dma_counter >= UartEngine.StartOfMsg)
    {
      length = dma_counter - UartEngine.StartOfMsg;
    }
    else
    {
      length = (uint16_t)UART_RxBufferSize + dma_counter - UartEngine.StartOfMsg;
    }

    j = UartEngine.StartOfMsg;

    for (k = 0; k < length; k++)
    {
      data = UartRxBuffer[j];
      j++;

      if (j >= (uint16_t)UART_RxBufferSize)
      {
        j = 0;
      }

      if (data == (uint8_t)TMsg_EOF)
      {
        j = UartEngine.StartOfMsg;

        for (i = 0; i < k; i += inc)
        {
          uint8_t  Source0;
          uint8_t  Source1;
          uint8_t *Dest;

          j2 = (j + 1U) % (uint16_t)UART_RxBufferSize;

          if (source >= TMsg_MaxLen)
          {
            UartEngine.StartOfMsg = j;
            return 0;
          }

          Source0 = UartRxBuffer[j];
          Source1 = UartRxBuffer[j2];
          Dest    = &Msg->Data[source];

          inc = (uint8_t)ReverseByteStuffCopyByte2(Source0, Source1, Dest);

          if (inc == 0U)
          {
            UartEngine.StartOfMsg = j2;
            return 0;
          }

          j = (j + inc) % (uint16_t)UART_RxBufferSize;
          source++;
        }

        Msg->Len = source;
        j = (j + 1U) % (uint16_t)UART_RxBufferSize; /* skip TMsg_EOF */
        UartEngine.StartOfMsg = j;

        /* check message integrity */
        return (CHK_CheckAndRemove(Msg) != 0) ? 1 : 0;
      }
    }

    if (length > (uint16_t)Uart_Msg_Max_Size)
    {
      UartEngine.StartOfMsg = dma_counter;
    }
  }

  return 0;
}

/**
 * @brief  Send a message via UART
 * @param  Msg the pointer to the message to be sent
 * @retval None
 */
void UART_SendMsg(TMsg *Msg)
{
  uint16_t count_out;
  CHK_ComputeAndAdd(Msg);

  /* MISRA C-2012 rule 11.8 violation for purpose */
  count_out = (uint16_t)ByteStuffCopy((uint8_t *)UartTxBuffer, Msg);

  /* MISRA C-2012 rule 11.8 violation for purpose */
  while (HAL_UART_Transmit_DMA(&hcom_uart[COM1], (uint8_t *)UartTxBuffer, count_out) == HAL_BUSY)
  {}
}

/**
 * @brief  Start receiving messages via DMA
 * @param  None
 * @retval None
 */
void UART_StartReceiveMsg(void)
{
  hcom_uart[COM1].pRxBuffPtr = (uint8_t *)UartRxBuffer; /* MISRA C-2012 rule 11.8 violation for purpose */
  hcom_uart[COM1].RxXferSize = UART_RxBufferSize;
  hcom_uart[COM1].ErrorCode = (uint32_t)HAL_UART_ERROR_NONE;

  /* Enable the DMA transfer for the receiver request by setting the DMAR bit
     in the UART CR3 register */
  /* MISRA C-2012 rule 11.8 violation for purpose */
  (void)HAL_UART_Receive_DMA(&hcom_uart[COM1], (uint8_t *)UartRxBuffer, UART_RxBufferSize);
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == hcom_uart[COM1].Instance)
  {
    /* Re-Start receiving messages via DMA */
    UART_StartReceiveMsg();
  }
}

/* Private functions ---------------------------------------------------------*/

/**
 * @}
 */

/**
 * @}
 */
