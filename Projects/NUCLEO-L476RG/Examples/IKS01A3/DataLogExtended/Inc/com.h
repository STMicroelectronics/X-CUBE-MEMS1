/**
 *******************************************************************************
 * @file    com.h
 * @author  MEMS Software Solutions Team
 * @brief   header for com.c.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0077, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0077
  *
  ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef COM_H
#define COM_H

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
} TUart_Engine;

/* Exported defines ----------------------------------------------------------*/
#define UART_RxBufferSize (2 * TMsg_MaxLen)
#define UART_TxBufferSize (2 * TMsg_MaxLen)

/* User can use this section to tailor USARTx/UARTx instance used and associated resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __USART2_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA

/* Exported variables --------------------------------------------------------*/
extern UART_HandleTypeDef UartHandle;
extern DMA_HandleTypeDef HdmaRx;
extern DMA_HandleTypeDef HdmaTx;

extern volatile uint8_t UartRxBuffer[UART_RxBufferSize];
extern TUart_Engine UartEngine;
extern volatile uint32_t UsartBaudRate;
extern volatile uint8_t ReadyToSend;

/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void USARTConfig(void);
int UART_ReceivedMSG(TMsg *Msg);
void UART_SendMsg(TMsg *Msg);
void USART_DMA_Configuration(void);

#endif /* COM_H */
