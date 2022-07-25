/**
  ******************************************************************************
  * @file    bsp_ip_conf.h
  * @author  MEMS Application Team
  * @brief   BSP IP configuration file
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
#ifndef __BSP_IP_CONF_H__
#define __BSP_IP_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "iks01a2_mems_control.h"
#include "iks01a2_mems_control_ex.h"

/* Exported defines ----------------------------------------------------------*/

#define BOARD_NAME "IKS01A2"

/* Exported variables --------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
#define BSP_IP_TIM_Handle htim3
#define BSP_IP_TIM_Init MX_TIM3_Init

extern RTC_HandleTypeDef hrtc;

#ifdef __cplusplus
}
#endif

#endif /* __BSP_IP_CONF_H__ */

