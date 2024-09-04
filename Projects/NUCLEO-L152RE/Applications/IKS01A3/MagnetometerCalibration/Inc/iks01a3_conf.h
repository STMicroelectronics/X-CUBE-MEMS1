/**
  ******************************************************************************
  * @file    iks01a3_conf.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define IKS01A3_DELAY HAL_Delay

#define BUS_IKS01A3_INSTANCE BUS_I2C1_INSTANCE
#define BUS_IKS01A3_CLK_DISABLE() __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_IKS01A3_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_IKS01A3_SCL_GPIO_PORT BUS_I2C1_SCL_GPIO_PORT
#define BUS_IKS01A3_SCL_GPIO_AF BUS_I2C1_SCL_GPIO_AF
#define BUS_IKS01A3_SCL_GPIO_CLK_ENABLE() BUS_I2C1_SCL_GPIO_CLK_ENABLE()
#define BUS_IKS01A3_SCL_GPIO_CLK_DISABLE() BUS_I2C1_SCL_GPIO_CLK_DISABLE()
#define BUS_IKS01A3_SCL_GPIO_PIN BUS_I2C1_SCL_GPIO_PIN
#define BUS_IKS01A3_SDA_GPIO_PIN BUS_I2C1_SDA_GPIO_PIN
#define BUS_IKS01A3_SDA_GPIO_CLK_DISABLE() BUS_I2C1_SDA_GPIO_CLK_DISABLE()
#define BUS_IKS01A3_SDA_GPIO_PORT BUS_I2C1_SDA_GPIO_PORT
#define BUS_IKS01A3_SDA_GPIO_AF BUS_I2C1_SDA_GPIO_AF
#define BUS_IKS01A3_SDA_GPIO_CLK_ENABLE() BUS_I2C1_SDA_GPIO_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* __IKS01A3_CONF_H__*/

