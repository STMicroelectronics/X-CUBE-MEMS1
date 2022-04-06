/**
 ******************************************************************************
 * @file    com.c
 * @author  MEMS Software Solutions Team
 * @brief   This file provides communication functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the "License". You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "com.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/* Private types -------------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define Uart_Msg_Max_Size TMsg_MaxLen

/* Private macro -------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
volatile uint8_t UartRxBuffer[UART_RxBufferSize];
TUart_Engine UartEngine;

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t UartTxBuffer[UART_TxBufferSize];

/* Private function prototypes -----------------------------------------------*/
static uint32_t Get_DMA_Flag_Status(DMA_HandleTypeDef *handle_dma);
static uint32_t Get_DMA_Counter(DMA_HandleTypeDef *handle_dma);

/* Exported functions --------------------------------------------------------*/
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
  (void)HAL_UART_Transmit(&hcom_uart[COM1], (uint8_t *)UartTxBuffer, count_out, 5000);
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

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Get the DMA Stream pending flags
 * @param  handle_dma DMA handle
 * @retval The state of FLAG (SET or RESET)
 */
static uint32_t Get_DMA_Flag_Status(DMA_HandleTypeDef *handle_dma)
{
  return (__HAL_DMA_GET_FLAG(handle_dma, __HAL_DMA_GET_TE_FLAG_INDEX(handle_dma)));
}

/**
 * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer
 * @param  handle_dma DMA handle
 * @retval The number of remaining data units in the current DMA Stream transfer
 */
static uint32_t Get_DMA_Counter(DMA_HandleTypeDef *handle_dma)
{
  return (__HAL_DMA_GET_COUNTER(handle_dma));
}

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
