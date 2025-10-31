/**
  ******************************************************************************
  * @file    iks5a1_conf.h
  * @author  MEMS Application Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2025 STMicroelectronics.
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
#ifndef __IKS5A1_CONF_H__
#define __IKS5A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define USE_IKS5A1_ENV_SENSOR_ILPS22QS_0       1U

#define USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0  1U
#define USE_IKS5A1_MOTION_SENSOR_ISM330IS_0    1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2DULPX_0   1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2MDC_0     1U

#define USE_IKS5A1_ENV_SENSOR_SHT40AD1B_0      1U

#define USE_IKS5A1_MOTION_SENSOR_AIS2IH_0      1U
#define USE_IKS5A1_MOTION_SENSOR_ASM330LHHX_0  1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2DLPC_0    1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2ICLX_0    1U
#define USE_IKS5A1_MOTION_SENSOR_ISM330BX_0    1U
#define USE_IKS5A1_MOTION_SENSOR_ISM330DHCX_0  1U

#define IKS5A1_I2C_INIT       BSP_I2C1_Init
#define IKS5A1_I2C_DEINIT     BSP_I2C1_DeInit
#define IKS5A1_I2C_READ_REG   BSP_I2C1_ReadReg
#define IKS5A1_I2C_WRITE_REG  BSP_I2C1_WriteReg
#define IKS5A1_I2C_READ       BSP_I2C1_Recv
#define IKS5A1_I2C_WRITE      BSP_I2C1_Send
#define IKS5A1_DELAY          HAL_Delay
#define IKS5A1_GET_TICK       BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __IKS5A1_CONF_H__*/
