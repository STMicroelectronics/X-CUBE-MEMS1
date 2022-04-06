/**
 ******************************************************************************
 * @file    iks02a1_conf.h
 * @author  MEMS Application Team
 * @brief   This file contains definitions for the MEMS components bus interfaces
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

#include "stm32l1xx_hal.h"
#include "stm32l1xx_nucleo_bus.h"
#include "stm32l1xx_nucleo_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IKS02A1_CONF_H__
#define __IKS02A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */
#define USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0         1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0           1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0            1U
/* USER CODE END 1 */

#define IKS02A1_I2C_Init BSP_I2C1_Init
#define IKS02A1_I2C_DeInit BSP_I2C1_DeInit
#define IKS02A1_I2C_ReadReg BSP_I2C1_ReadReg
#define IKS02A1_I2C_WriteReg BSP_I2C1_WriteReg
#define IKS02A1_GetTick BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __IKS02A1_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
