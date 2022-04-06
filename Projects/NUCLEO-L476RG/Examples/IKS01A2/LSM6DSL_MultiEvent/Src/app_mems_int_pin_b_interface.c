/**
 ******************************************************************************
 * @file    app_mems_int_pin_b_interface.c
 * @author  MEMS Application Team
 * @brief   This file contains the MEMS INT pin B interface
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

/* Includes ------------------------------------------------------------------*/
#include "app_mems_int_pin_b_interface.h"

EXTI_HandleTypeDef hexti4 = {.Line = EXTI_LINE_4};
extern volatile uint8_t MemsEventDetected;

static void mems_int_pin_b_hardware_event_isr(void);

void set_mems_int_pin_b_exti(void)
{
  /* register event irq handler */
  HAL_EXTI_GetHandle(&hexti4, EXTI_LINE_4);
  HAL_EXTI_RegisterCallback(&hexti4, HAL_EXTI_COMMON_CB_ID, mems_int_pin_b_hardware_event_isr);
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

static void mems_int_pin_b_hardware_event_isr(void)
{
  MemsEventDetected = 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
