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

/** @addtogroup X_NUCLEO_IKS01A3_Examples X_NUCLEO_IKS01A3 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define UART_MSG_MAX_SIZE Msg_MaxLen

/* Private macro -------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef HdmaRx;
DMA_HandleTypeDef HdmaTx;

volatile uint8_t UartRxBuffer[UART_RX_BUFFER_SIZE];
volatile uint32_t UsartBaudRate = 921600;
volatile uint8_t ReadyToSend = 1;
Uart_Engine_t UartEngine;

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t UartTxBuffer[Msg_MaxLen * 2];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
/**
 * @brief  Check if a message is received via UART
 * @param  Msg the pointer to the message to be received
 * @retval 1 if a complete message is found, 0 otherwise
 */
int32_t UART_ReceivedMSG(Msg_t *Msg)
{
  uint16_t i;
  uint16_t j;
  uint16_t k;
  uint16_t j2;
  uint16_t dma_counter;
  uint16_t length;
  uint8_t data;
  uint16_t source = 0;
  uint8_t inc;

  if (Get_DMA_Flag_Status(&HdmaRx) == (uint32_t)RESET)
  {
    dma_counter = (uint16_t)UART_RX_BUFFER_SIZE - (uint16_t)Get_DMA_Counter(&HdmaRx);

    if (dma_counter >= UartEngine.StartOfMsg)
    {
      length = dma_counter - UartEngine.StartOfMsg;
    }
    else
    {
      length = (uint16_t)UART_RX_BUFFER_SIZE + dma_counter - UartEngine.StartOfMsg;
    }

    j = UartEngine.StartOfMsg;

    for (k = 0; k < length; k++)
    {
      data = UartRxBuffer[j];
      j++;

      if (j >= (uint16_t)UART_RX_BUFFER_SIZE)
      {
        j = 0;
      }

      if (data == (uint8_t)Msg_EOF)
      {
        j = UartEngine.StartOfMsg;

        for (i = 0; i < k; i += inc)
        {
          uint8_t  Source0;
          uint8_t  Source1;
          uint8_t *Dest;

          j2 = (j + 1U) % (uint16_t)UART_RX_BUFFER_SIZE;

          if (source >= Msg_MaxLen)
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

          j = (j + inc) % (uint16_t)UART_RX_BUFFER_SIZE;
          source++;
        }

        Msg->Len = source;
        j = (j + 1U) % (uint16_t)UART_RX_BUFFER_SIZE; /* skip Msg_EOF */
        UartEngine.StartOfMsg = j;

        if (CHK_CheckAndRemove(Msg) != 0) /* check message integrity */
        {
          return 1;
        }
        else
        {
          return 0;
        }
      }
    }

    if (length > (uint16_t)UART_MSG_MAX_SIZE)
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
void UART_SendMsg(Msg_t *Msg)
{
  uint16_t count_out;

  while (ReadyToSend == 0)
  {}

  CHK_ComputeAndAdd(Msg);

  /* MISRA C-2012 rule 11.8 violation for purpose */
  count_out = (uint16_t)ByteStuffCopy((uint8_t *)UartTxBuffer, Msg);

  ReadyToSend = 0;
  /* MISRA C-2012 rule 11.8 violation for purpose */
  (void)HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)UartTxBuffer, count_out);
}

/**
 * @brief  Configure DMA for the reception via USART
 * @param  HdmaRx DMA handle
 * @retval None
 */
void USART_DMA_Configuration(void)
{
  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

  Config_DMA_RxHandler(&HdmaRx);
  (void)HAL_DMA_Init(&HdmaRx);

  /* Associate the initialized DMA handle to the the UART handle */
  __HAL_LINKDMA(&UartHandle, hdmarx, HdmaRx);

  Config_DMA_TxHandler(&HdmaTx);
  (void)HAL_DMA_Init(&HdmaTx);

  /* Associate the initialized DMA handle to the the UART handle */
  __HAL_LINKDMA(&UartHandle, hdmatx, HdmaTx);
}

/**
 * @brief  Configure the USART
 * @param  None
 * @retval None
 */
void USARTConfig(void)
{
  GPIO_InitTypeDef gpio_init_struct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART2 clock */
  USARTx_CLK_ENABLE();
  /* Enable DMA1 clock */
  DMAx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  gpio_init_struct.Pin       = USARTx_TX_PIN;
  gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
  gpio_init_struct.Pull      = GPIO_NOPULL;
  gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_struct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &gpio_init_struct);

  /* UART RX GPIO pin configuration  */
  gpio_init_struct.Pin = USARTx_RX_PIN;
  gpio_init_struct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &gpio_init_struct);

  /*##-3- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = UsartBaudRate;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;

  (void)HAL_UART_Init(&UartHandle);

  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  USART_DMA_Configuration();

  UartHandle.pRxBuffPtr = (uint8_t *)UartRxBuffer; /* MISRA C-2012 rule 11.8 violation for purpose */
  UartHandle.RxXferSize = UART_RX_BUFFER_SIZE;

  UartHandle.pTxBuffPtr = (uint8_t *)UartTxBuffer; /* MISRA C-2012 rule 11.8 violation for purpose */
  UartHandle.TxXferSize = UART_TX_BUFFER_SIZE;

  UartHandle.ErrorCode = (uint32_t)HAL_UART_ERROR_NONE;
  UartEngine.StartOfMsg = 0;

  /* Enable the DMA transfer for the receiver request by setting the DMAR bit
     in the UART CR3 register */
  /* MISRA C-2012 rule 11.8 violation for purpose */
  (void)HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)UartRxBuffer, UART_RX_BUFFER_SIZE);
}

/**
 * @brief  UART error callback
 * @param  huart UART handle
 * @retval None
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  huart->ErrorCode = (uint32_t)HAL_UART_ERROR_NONE;
  UartEngine.StartOfMsg = 0;

  /* Enable the DMA transfer for the receiver request by setting the DMAR bit
     in the UART CR3 register */
  /* MISRA C-2012 rule 11.8 violation for purpose */
  (void)HAL_UART_Receive_DMA(huart, (uint8_t *)UartRxBuffer, UART_RX_BUFFER_SIZE);
}

/**
 * @brief  UART transfer complete callback
 * @param  huart UART handle
 * @retval None
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  ReadyToSend = 1;
}

/**
 * @}
 */

/**
 * @}
 */
