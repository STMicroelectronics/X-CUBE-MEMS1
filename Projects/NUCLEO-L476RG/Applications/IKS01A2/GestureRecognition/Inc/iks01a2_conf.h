/**
 ******************************************************************************
 * @file    iks01a2_conf.h
 * @author  MEMS Application Team
 * @brief   This file contains definitions for the MEMS components bus interfaces
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the “License�?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_bus.h"
#include "stm32l4xx_nucleo_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IKS01A2_CONF_H__
#define __IKS01A2_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */
#define USE_IKS01A2_ENV_SENSOR_HTS221_0                1U
#define USE_IKS01A2_ENV_SENSOR_LPS22HB_0               1U

#define USE_IKS01A2_MOTION_SENSOR_LSM6DSL_0            1U
#define USE_IKS01A2_MOTION_SENSOR_LSM303AGR_ACC_0      1U
#define USE_IKS01A2_MOTION_SENSOR_LSM303AGR_MAG_0      1U
/* USER CODE END 1 */

#define IKS01A2_I2C_Init BSP_I2C1_Init
#define IKS01A2_I2C_DeInit BSP_I2C1_DeInit
#define IKS01A2_I2C_ReadReg BSP_I2C1_ReadReg
#define IKS01A2_I2C_WriteReg BSP_I2C1_WriteReg
#define IKS01A2_GetTick BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __IKS01A2_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

