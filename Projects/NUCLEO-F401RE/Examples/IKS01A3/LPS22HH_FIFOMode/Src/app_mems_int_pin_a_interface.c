/**
  ******************************************************************************
  * @file    app_mems_int_pin_a_interface.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains the MEMS INT pin A interface
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

/* Includes ------------------------------------------------------------------*/
#include "app_mems_int_pin_a_interface.h"

EXTI_HandleTypeDef hexti10 = {.Line = EXTI_LINE_10};
extern volatile uint8_t MemsEventDetected;

static void mems_int_pin_a_hardware_event_isr(void);

void set_mems_int_pin_a_exti(void)
{
  /* register event irq handler */
  HAL_EXTI_GetHandle(&hexti10, EXTI_LINE_10);
  HAL_EXTI_RegisterCallback(&hexti10, HAL_EXTI_COMMON_CB_ID, mems_int_pin_a_hardware_event_isr);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void mems_int_pin_a_hardware_event_isr(void)
{
  MemsEventDetected = 1;
}

