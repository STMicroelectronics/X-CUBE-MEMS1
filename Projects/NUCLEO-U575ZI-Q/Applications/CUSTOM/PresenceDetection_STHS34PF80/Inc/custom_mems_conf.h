/**
  ******************************************************************************
  * @file    custom_mems_conf.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains definitions of the MEMS components bus interfaces for custom boards
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
#ifndef CUSTOM_MEMS_CONF_H
#define CUSTOM_MEMS_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo_bus.h"
#include "stm32u5xx_nucleo_errno.h"

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#define USE_CUSTOM_ENV_SENSOR_STHS34PF80_0        1U

#define CUSTOM_STHS34PF80_0_I2C_INIT BSP_I2C1_Init
#define CUSTOM_STHS34PF80_0_I2C_DEINIT BSP_I2C1_DeInit
#define CUSTOM_STHS34PF80_0_I2C_READ_REG BSP_I2C1_ReadReg
#define CUSTOM_STHS34PF80_0_I2C_WRITE_REG BSP_I2C1_WriteReg

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_MEMS_CONF_H*/
