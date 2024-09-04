/**
  ******************************************************************************
  * @file    iks01a3_conf_template.h
  * @author  MEMS Application Team
  * @brief   IKS01A3 configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to iks01a3_conf.h.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Replace the header file names with the ones of the target platform */
#include "stm32yyxx_hal.h"
#include "nucleo_xyyyzz_bus.h"
#include "nucleo_xyyyzz_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IKS01A3_CONF_H__
#define __IKS01A3_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */
#define USE_IKS01A3_ENV_SENSOR_HTS221_0                1U
#define USE_IKS01A3_ENV_SENSOR_LPS22HH_0               1U
#define USE_IKS01A3_ENV_SENSOR_STTS751_0               1U

#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0            1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0           1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0            1U
/* USER CODE END 1 */

#define IKS01A3_I2C_INIT BSP_I2C1_Init
#define IKS01A3_I2C_DEINIT BSP_I2C1_DeInit
#define IKS01A3_I2C_READ_REG BSP_I2C1_ReadReg
#define IKS01A3_I2C_WRITE_REG BSP_I2C1_WriteReg
#define IKS01A3_GET_TICK BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __IKS01A3_CONF_H__*/
