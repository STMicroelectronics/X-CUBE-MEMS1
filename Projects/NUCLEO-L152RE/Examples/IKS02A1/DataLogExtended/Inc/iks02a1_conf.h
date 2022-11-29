/**
  ******************************************************************************
  * @file    iks02a1_conf.h
  * @author  MEMS Application Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "stm32l1xx_hal.h"
#include "nucleo_l152re_bus.h"
#include "nucleo_l152re_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IKS02A1_CONF_H__
#define __IKS02A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0         1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0           1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0            1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2ICLX_0           1U
#define USE_IKS02A1_MOTION_SENSOR_ASM330LHHX_0         1U
#define USE_IKS02A1_MOTION_SENSOR_LSM6DSV16X_0         1U
#define USE_IKS02A1_MOTION_SENSOR_LSM6DSV16BX_0        1U

#define IKS02A1_I2C_Init BSP_I2C1_Init
#define IKS02A1_I2C_DeInit BSP_I2C1_DeInit
#define IKS02A1_I2C_ReadReg BSP_I2C1_ReadReg
#define IKS02A1_I2C_WriteReg BSP_I2C1_WriteReg
#define IKS02A1_Delay HAL_Delay
#define IKS02A1_GetTick BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __IKS02A1_CONF_H__*/
