/**
  ******************************************************************************
  * @file    iks4a1_conf.h
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

#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_bus.h"
#include "stm32l0xx_nucleo_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS4A1_CONF_H
#define IKS4A1_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

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

#define BUS_IKS4A1_INSTANCE BUS_I2C1_INSTANCE
#define BUS_IKS4A1_CLK_DISABLE() __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_IKS4A1_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_IKS4A1_SCL_GPIO_PORT BUS_I2C1_SCL_GPIO_PORT
#define BUS_IKS4A1_SCL_GPIO_AF BUS_I2C1_SCL_GPIO_AF
#define BUS_IKS4A1_SCL_GPIO_CLK_ENABLE() BUS_I2C1_SCL_GPIO_CLK_ENABLE()
#define BUS_IKS4A1_SCL_GPIO_CLK_DISABLE() BUS_I2C1_SCL_GPIO_CLK_DISABLE()
#define BUS_IKS4A1_SCL_GPIO_PIN BUS_I2C1_SCL_GPIO_PIN
#define BUS_IKS4A1_SDA_GPIO_PIN BUS_I2C1_SDA_GPIO_PIN
#define BUS_IKS4A1_SDA_GPIO_CLK_DISABLE() BUS_I2C1_SDA_GPIO_CLK_DISABLE()
#define BUS_IKS4A1_SDA_GPIO_PORT BUS_I2C1_SDA_GPIO_PORT
#define BUS_IKS4A1_SDA_GPIO_AF BUS_I2C1_SDA_GPIO_AF
#define BUS_IKS4A1_SDA_GPIO_CLK_ENABLE() BUS_I2C1_SDA_GPIO_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* IKS4A1_CONF_H*/
