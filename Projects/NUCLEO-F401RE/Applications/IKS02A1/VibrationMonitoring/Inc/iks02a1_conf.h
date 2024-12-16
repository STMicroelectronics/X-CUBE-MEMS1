/**
  ******************************************************************************
  * @file    iks02a1_conf.h
  * @author  MEMS Application Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "nucleo_f401re_bus.h"
#include "nucleo_f401re_errno.h"

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

#define IKS02A1_I2C_INIT BSP_I2C1_Init
#define IKS02A1_I2C_DEINIT BSP_I2C1_DeInit
#define IKS02A1_I2C_READ_REG BSP_I2C1_ReadReg
#define IKS02A1_I2C_WRITE_REG BSP_I2C1_WriteReg
#define IKS02A1_GET_TICK BSP_GetTick
#define IKS02A1_DELAY HAL_Delay

#ifdef __cplusplus
}
#endif

#endif /* __IKS02A1_CONF_H__*/
