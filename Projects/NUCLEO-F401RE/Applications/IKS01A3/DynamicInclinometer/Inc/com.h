/**
  *******************************************************************************
  * @file    com.h
  * @author  MEMS Software Solutions Team
  * @brief   header for com.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "bsp_ip_conf.h"

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

/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t UartRxBuffer[];
extern Uart_Engine_t UartEngine;

/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void UART_StartReceiveMsg(void);
int32_t UART_ReceivedMSG(Msg_t *Msg);
void UART_SendMsg(Msg_t *Msg);

#ifdef __cplusplus
}
#endif

#endif /* COM_H */
