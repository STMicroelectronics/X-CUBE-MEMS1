/**
  ******************************************************************************
  * @file    main.h
  * @author  SRA - Central Labs
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
void Error_Handler(void);

#endif /* __MAIN_H */
