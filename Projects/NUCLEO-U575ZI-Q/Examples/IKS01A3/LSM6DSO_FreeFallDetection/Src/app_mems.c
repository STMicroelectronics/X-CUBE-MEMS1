/**
  ******************************************************************************
  * File Name          : app_mems.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.11.2.0 instances.
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

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_mems.h"
#include "main.h"
#include <stdio.h>

#include "app_mems_int_pin_a_interface.h"
#include "stm32u5xx_hal.h"
#include "stm32u5xx_hal_exti.h"
#include "stm32u5xx_nucleo.h"
#include "iks01a3_motion_sensors.h"
#include "iks01a3_motion_sensors_ex.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INDICATION_DELAY  100 /* LED is ON for this period [ms]. */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t MemsEventDetected = 0;
static volatile uint8_t PushButtonDetected = 0;
static uint8_t FreeFallEnableRequest = 1;
static uint8_t FreeFallDisableRequest = 0;
static uint8_t FreeFallEnabled = 0;
static int32_t PushButtonState = GPIO_PIN_RESET;

/* Private function prototypes -----------------------------------------------*/
static void MX_IKS01A3_LSM6DSO_FreeFall_Init(void);
static void MX_IKS01A3_LSM6DSO_FreeFall_Process(void);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS01A3_LSM6DSO_FreeFall_Init();

  /* USER CODE BEGIN MEMS_Init_PostTreatment */

  /* USER CODE END MEMS_Init_PostTreatment */
}

/*
 * LM background task
 */
void MX_MEMS_Process(void)
{
  /* USER CODE BEGIN MEMS_Process_PreTreatment */

  /* USER CODE END MEMS_Process_PreTreatment */

  MX_IKS01A3_LSM6DSO_FreeFall_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the LSM6DSO Free Fall Detection application
  * @retval None
  */
void MX_IKS01A3_LSM6DSO_FreeFall_Init(void)
{
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Check what is the Push Button State when the button is not pressed. It can change across families */
  PushButtonState = (BSP_PB_GetState(BUTTON_KEY)) ?  0 : 1;

  /* Set EXTI settings for Interrupt A */
  set_mems_int_pin_a_exti();

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  IKS01A3_MOTION_SENSOR_Init(IKS01A3_LSM6DSO_0, MOTION_ACCELERO | MOTION_GYRO);
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  PushButtonDetected = 1;
}

/**
  * @brief  Process of the LSM6DSO Free Fall Detection application
  * @retval None
  */
void MX_IKS01A3_LSM6DSO_FreeFall_Process(void)
{
  IKS01A3_MOTION_SENSOR_Event_Status_t status;

  if (PushButtonDetected != 0U)
  {
    /* Debouncing */
    HAL_Delay(50);

    /* Wait until the button is released */
    while ((BSP_PB_GetState(BUTTON_KEY) == PushButtonState));

    /* Debouncing */
    HAL_Delay(50);

    /* Reset Interrupt flag */
    PushButtonDetected = 0;

    /* Toggle enable/disable free-fall detection */
    if (FreeFallEnabled != 0U)
    {
      FreeFallDisableRequest = 1;
    }
    else
    {
      FreeFallEnableRequest = 1;
    }
  }

  if (MemsEventDetected != 0U)
  {
    MemsEventDetected = 0;

    if (IKS01A3_MOTION_SENSOR_Get_Event_Status(IKS01A3_LSM6DSO_0, &status) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    if (status.FreeFallStatus != 0U)
    {
      BSP_LED_On(LED2);
      HAL_Delay(INDICATION_DELAY);
      BSP_LED_Off(LED2);
    }
  }

  if (FreeFallEnableRequest != 0U)
  {
    if (IKS01A3_MOTION_SENSOR_Enable_Free_Fall_Detection(IKS01A3_LSM6DSO_0,
                                                         IKS01A3_MOTION_SENSOR_INT1_PIN) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    FreeFallEnabled = 1;
    FreeFallEnableRequest = 0;
  }

  if (FreeFallDisableRequest != 0U)
  {
    if (IKS01A3_MOTION_SENSOR_Disable_Free_Fall_Detection(IKS01A3_LSM6DSO_0) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    FreeFallEnabled = 0;
    FreeFallDisableRequest = 0;
  }
}

#ifdef __cplusplus
}
#endif
