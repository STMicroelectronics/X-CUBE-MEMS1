/**
 ******************************************************************************
 * @file    app_mems_int_pin_a_interface.h
 * @author  MEMS Application Team
 * @brief   This file contains definitions for the MEMS INT pin A interface
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the "License". You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_MEMS_INT_PIN_A_INTERFACE_H__
#define __APP_MEMS_INT_PIN_A_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_exti.h"

/* Exported variables --------------------------------------------------------*/
extern EXTI_HandleTypeDef hexti0;
#define H_EXTI_0 hexti0

/* Exported Functions --------------------------------------------------------*/
void set_mems_int_pin_a_exti(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_MEMS_INT_PIN_A_INTERFACE_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

