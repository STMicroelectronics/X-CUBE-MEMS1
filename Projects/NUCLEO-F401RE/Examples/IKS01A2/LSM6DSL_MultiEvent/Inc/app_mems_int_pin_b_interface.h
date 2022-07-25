/**
  ******************************************************************************
  * @file    app_mems_int_pin_b_interface.h
  * @author  MEMS Application Team
  * @brief   This file contains definitions for the MEMS INT pin B interface
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_MEMS_INT_PIN_B_INTERFACE_H__
#define __APP_MEMS_INT_PIN_B_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_exti.h"

/* Exported variables --------------------------------------------------------*/
extern EXTI_HandleTypeDef hexti4;
#define H_EXTI_4 hexti4

/* Exported Functions --------------------------------------------------------*/
void set_mems_int_pin_b_exti(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_MEMS_INT_PIN_B_INTERFACE_H__*/

