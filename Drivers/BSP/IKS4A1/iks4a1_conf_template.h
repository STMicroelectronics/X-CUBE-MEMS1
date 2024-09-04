/**
  ******************************************************************************
  * @file    iks4a1_conf_template.h
  * @author  MEMS Software Solutions Team
  * @brief   IKS4A1 configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to iks4a1_conf.h.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS4A1_CONF_H
#define IKS4A1_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Replace the header file names with the ones of the target platform */
#include "stm32yyxx_hal.h"
#include "nucleo_xyyyzz_bus.h"
#include "nucleo_xyyyzz_errno.h"

#define USE_IKS4A1_ENV_SENSOR_SHT40AD1B_0                      1U
#define USE_IKS4A1_ENV_SENSOR_LPS22DF_0                        1U
#define USE_IKS4A1_ENV_SENSOR_STTS22H_0                        1U

#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0                  1U
#define USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0                  1U
#define USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0                     1U
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0                 1U

#define IKS4A1_I2C_INIT BSP_I2C1_Init
#define IKS4A1_I2C_DEINIT BSP_I2C1_DeInit
#define IKS4A1_I2C_READ_REG BSP_I2C1_ReadReg
#define IKS4A1_I2C_WRITE_REG BSP_I2C1_WriteReg
#define IKS4A1_I2C_READ BSP_I2C1_Recv
#define IKS4A1_I2C_WRITE BSP_I2C1_Send
#define IKS4A1_GET_TICK BSP_GetTick
#define IKS4A1_DELAY HAL_Delay

#ifdef __cplusplus
}
#endif

#endif /* IKS4A1_CONF_H*/
