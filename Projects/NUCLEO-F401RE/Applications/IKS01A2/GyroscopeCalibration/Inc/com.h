/**
 *******************************************************************************
 * @file    com.h
 * @author  MEMS Software Solutions Team
 * @brief   header for com.c
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
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
} TUart_Engine;

/* Exported defines ----------------------------------------------------------*/
#define UART_RxBufferSize (2 * TMsg_MaxLen)
#define UART_TxBufferSize (2 * TMsg_MaxLen)

/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t UartRxBuffer[UART_RxBufferSize];
extern TUart_Engine UartEngine;

/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void UART_StartReceiveMsg(void);
int UART_ReceivedMSG(TMsg *Msg);
void UART_SendMsg(TMsg *Msg);

#ifdef __cplusplus
}
#endif

#endif /* COM_H */
