/**
 ******************************************************************************
 * @file    bsp_ip_conf.h
 * @author  MEMS Application Team
 * @brief   BSP IP configuration file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
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

#include "custom_mems_control.h"
#include "custom_mems_control_ex.h"

/* Exported defines ----------------------------------------------------------*/

#define BOARD_NAME "CUSTOM"
#define USE_CUSTOM_BOARD

/* Exported variables --------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
#define BSP_IP_TIM_Handle htim3
#define BSP_IP_TIM_Init MX_TIM3_Init

#define BSP_IP_MEMS_INT1_PIN_NUM GPIO_PIN_0
#define BSP_IP_MEMS_INT1_GPIOX GPIOC

extern RTC_HandleTypeDef hrtc;

#ifdef __cplusplus
}
#endif

#endif /* __BSP_IP_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
