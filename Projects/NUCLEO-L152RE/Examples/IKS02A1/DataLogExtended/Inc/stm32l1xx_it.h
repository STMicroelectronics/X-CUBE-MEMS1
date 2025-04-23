/**
  ******************************************************************************
  * @file    stm32l1xx_it.h
  * @author  MEMS Software Solutions Team
  * @brief   header for stm32l1xx_it.c.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L1xx_IT_H
#define STM32L1xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void DebugMon_Handler(void);
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32L1xx_IT_H */
