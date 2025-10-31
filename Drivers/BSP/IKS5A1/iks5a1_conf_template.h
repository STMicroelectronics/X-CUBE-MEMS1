/**
  ******************************************************************************
  * @file    iks5a1_conf_template.h
  * @author  MEMS Software Solutions Team
  * @brief   IKS5A1 configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to iks5a1_conf.h.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS5A1_CONF_H
#define IKS5A1_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Replace the header file names with the ones of the target platform */
#include "stm32yyxx_hal.h"
#include "nucleo_xyyyzz_bus.h"
#include "nucleo_xyyyzz_errno.h"

#define USE_IKS5A1_ENV_SENSOR_ILPS22QS_0       1U

#define USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0  1U
#define USE_IKS5A1_MOTION_SENSOR_ISM330IS_0    1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2DULPX_0   1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2MDC_0     1U

#define IKS5A1_I2C_INIT BSP_I2C1_Init
#define IKS5A1_I2C_DEINIT BSP_I2C1_DeInit
#define IKS5A1_I2C_READ_REG BSP_I2C1_ReadReg
#define IKS5A1_I2C_WRITE_REG BSP_I2C1_WriteReg
#define IKS5A1_I2C_READ BSP_I2C1_Recv
#define IKS5A1_I2C_WRITE BSP_I2C1_Send
#define IKS5A1_GET_TICK BSP_GetTick
#define IKS5A1_DELAY HAL_Delay

#ifdef __cplusplus
}
#endif

#endif /* IKS5A1_CONF_H*/
