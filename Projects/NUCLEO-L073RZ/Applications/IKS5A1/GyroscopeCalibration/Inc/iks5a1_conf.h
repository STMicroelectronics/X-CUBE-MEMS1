/**
  ******************************************************************************
  * @file    iks5a1_conf.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#ifndef __IKS5A1_CONF_H__
#define __IKS5A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */
#define USE_IKS5A1_ENV_SENSOR_ILPS22QS_0            1U

#define USE_IKS5A1_MOTION_SENSOR_ISM330IS_0         1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2DULPX_0        1U
#define USE_IKS5A1_MOTION_SENSOR_ISM6HG256X_0       1U
#define USE_IKS5A1_MOTION_SENSOR_IIS2MDC_0          1U
/* USER CODE END 1 */

#define IKS5A1_I2C_INIT BSP_I2C1_Init
#define IKS5A1_I2C_DEINIT BSP_I2C1_DeInit
#define IKS5A1_I2C_READ_REG BSP_I2C1_ReadReg
#define IKS5A1_I2C_WRITE_REG BSP_I2C1_WriteReg
#define IKS5A1_GET_TICK BSP_GetTick
#define IKS5A1_DELAY HAL_Delay

#define BUS_IKS5A1_INSTANCE BUS_I2C1_INSTANCE
#define BUS_IKS5A1_CLK_DISABLE() __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_IKS5A1_CLK_ENABLE() __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_IKS5A1_SCL_GPIO_PORT BUS_I2C1_SCL_GPIO_PORT
#define BUS_IKS5A1_SCL_GPIO_AF BUS_I2C1_SCL_GPIO_AF
#define BUS_IKS5A1_SCL_GPIO_CLK_ENABLE() BUS_I2C1_SCL_GPIO_CLK_ENABLE()
#define BUS_IKS5A1_SCL_GPIO_CLK_DISABLE() BUS_I2C1_SCL_GPIO_CLK_DISABLE()
#define BUS_IKS5A1_SCL_GPIO_PIN BUS_I2C1_SCL_GPIO_PIN
#define BUS_IKS5A1_SDA_GPIO_PIN BUS_I2C1_SDA_GPIO_PIN
#define BUS_IKS5A1_SDA_GPIO_CLK_DISABLE() BUS_I2C1_SDA_GPIO_CLK_DISABLE()
#define BUS_IKS5A1_SDA_GPIO_PORT BUS_I2C1_SDA_GPIO_PORT
#define BUS_IKS5A1_SDA_GPIO_AF BUS_I2C1_SDA_GPIO_AF
#define BUS_IKS5A1_SDA_GPIO_CLK_ENABLE() BUS_I2C1_SDA_GPIO_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* __IKS5A1_CONF_H__*/

