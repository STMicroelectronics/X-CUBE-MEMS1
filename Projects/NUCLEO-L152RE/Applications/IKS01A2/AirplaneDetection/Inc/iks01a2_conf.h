/**
  ******************************************************************************
  * @file    iks01a2_conf.h
  * @author  MEMS Application Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "stm32l1xx_hal.h"
#include "stm32l1xx_nucleo_bus.h"
#include "stm32l1xx_nucleo_errno.h"

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

#define BUS_IKS01A2_INSTANCE BUS_I2C1_INSTANCE
#define BUS_IKS01A2_CLK_DISABLE() __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_IKS01A2_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_IKS01A2_SCL_GPIO_PORT BUS_I2C1_SCL_GPIO_PORT
#define BUS_IKS01A2_SCL_GPIO_AF BUS_I2C1_SCL_GPIO_AF
#define BUS_IKS01A2_SCL_GPIO_CLK_ENABLE() BUS_I2C1_SCL_GPIO_CLK_ENABLE()
#define BUS_IKS01A2_SCL_GPIO_CLK_DISABLE() BUS_I2C1_SCL_GPIO_CLK_DISABLE()
#define BUS_IKS01A2_SCL_GPIO_PIN BUS_I2C1_SCL_GPIO_PIN
#define BUS_IKS01A2_SDA_GPIO_PIN BUS_I2C1_SDA_GPIO_PIN
#define BUS_IKS01A2_SDA_GPIO_CLK_DISABLE() BUS_I2C1_SDA_GPIO_CLK_DISABLE()
#define BUS_IKS01A2_SDA_GPIO_PORT BUS_I2C1_SDA_GPIO_PORT
#define BUS_IKS01A2_SDA_GPIO_AF BUS_I2C1_SDA_GPIO_AF
#define BUS_IKS01A2_SDA_GPIO_CLK_ENABLE() BUS_I2C1_SDA_GPIO_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* __IKS01A2_CONF_H__*/

