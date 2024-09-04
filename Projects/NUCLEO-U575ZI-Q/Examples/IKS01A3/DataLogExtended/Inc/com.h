/**
  ******************************************************************************
  * @file    com.h
  * @author  MEMS Software Solutions Team
  * @brief   header for com.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef COM_H
#define COM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "serial_protocol.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  Serial message engine structure definition
 */
typedef struct
{
  uint8_t *pDMA_Buffer;
  uint16_t StartOfMsg;
} Uart_Engine_t;

/* Exported defines ----------------------------------------------------------*/
#define UART_RX_BUFFER_SIZE (2 * Msg_MaxLen)
#define UART_TX_BUFFER_SIZE (2 * Msg_MaxLen)

/* User can use this section to tailor USARTx/UARTx instance used and associated resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA

/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef UartHandle;
extern DMA_HandleTypeDef HdmaRx;
extern DMA_HandleTypeDef HdmaTx;

extern volatile uint8_t UartRxBuffer[];
extern Uart_Engine_t UartEngine;
extern volatile uint32_t UsartBaudRate;

/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void UART_StartReceiveMsg(void);
int32_t UART_ReceivedMSG(Msg_t *Msg);
void UART_SendMsg(Msg_t *Msg);
void USART_DMA_Configuration(void);

#ifdef __cplusplus
}
#endif

#endif /* COM_H */
