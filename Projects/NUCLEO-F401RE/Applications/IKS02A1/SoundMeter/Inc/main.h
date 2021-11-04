/**
  ******************************************************************************
  * @file    main.h
  * @author  SRA - Central Labs
  * @version v5.0.0
  * @date    6-May-19
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed under Software License Agreement
  * SLA0077, (the "License"). You may not use this file except in compliance
  * with the License. You may obtain a copy of the License at:
  *
  *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"

#if (defined (USE_STM32F4XX_NUCLEO))
#include "nucleo_f401re_bus.h"
#include "nucleo_f401re_errno.h"

#elif (defined (USE_STM32L0XX_NUCLEO))
#include "nucleo_l073rz_bus.h"
#include "nucleo_l073rz_errno.h"

#elif (defined (USE_STM32L1XX_NUCLEO))
#include "nucleo_l152re_bus.h"
#include "nucleo_l152re_errno.h"

#elif (defined (USE_STM32L4XX_NUCLEO))
#include "nucleo_l476rg_bus.h"
#include "nucleo_l476rg_errno.h"

#else
#error Not supported platform
#endif


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_DateRegulate(uint8_t y, uint8_t m, uint8_t d, uint8_t dw);
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss);
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
