/**
  ******************************************************************************
  * File Name          : app_mems.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.9.3.0 instances.
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

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_mems.h"
#include "main.h"
#include <stdio.h>

#include "app_mems_int_pin_a_interface.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_nucleo.h"
#include "iks01a2_motion_sensors.h"
#include "iks01a2_motion_sensors_ex.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE 256
#define INDICATION_DELAY           100  /* LED is ON for this period [ms]. */
#define SEND_STEP_COUNT_TIMEOUT  3000U  /* Send step count to UART after this timeout [ms]. */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t MemsEventDetected = 0;
static volatile uint8_t PushButtonDetected = 0;
static uint8_t StepCountResetRequest = 0;
static char dataOut[MAX_BUF_SIZE];
static uint16_t StepCount = 0;
static uint32_t previous_tick = 0;
static int32_t PushButtonState = GPIO_PIN_RESET;

/* Private function prototypes -----------------------------------------------*/
static void MX_IKS01A2_LSM6DSL_Pedometer_Init(void);
static void MX_IKS01A2_LSM6DSL_Pedometer_Process(void);
static uint32_t user_currentTimeGetTick(void);
static uint32_t user_currentTimeGetElapsedMS(uint32_t Tick1);
static void Send_Step_Count(void);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS01A2_LSM6DSL_Pedometer_Init();

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

  MX_IKS01A2_LSM6DSL_Pedometer_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the LSM6DSL Pedometer application
  * @retval None
  */
void MX_IKS01A2_LSM6DSL_Pedometer_Init(void)
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

  (void)IKS01A2_MOTION_SENSOR_Init(IKS01A2_LSM6DSL_0, MOTION_ACCELERO | MOTION_GYRO);

  /* Enable pedometer */
  (void)IKS01A2_MOTION_SENSOR_Enable_Pedometer(IKS01A2_LSM6DSL_0);

  /* Store current tick. */
  previous_tick = user_currentTimeGetTick();
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
  * @brief  Process of the LSM6DSL Pedometer application
  * @retval None
  */
void MX_IKS01A2_LSM6DSL_Pedometer_Process(void)
{
  IKS01A2_MOTION_SENSOR_Event_Status_t status;

  if (PushButtonDetected != 0U)
  {
    /* Debouncing */
    HAL_Delay(50);

    /* Wait until the button is released */
    while ((BSP_PB_GetState( BUTTON_KEY ) == PushButtonState));

    /* Debouncing */
    HAL_Delay(50);

    /* Reset Interrupt flag */
    PushButtonDetected = 0;

    /* Reset step counter of pedometer */
    StepCountResetRequest = 1;
  }

  if (StepCountResetRequest != 0U)
  {
    StepCountResetRequest = 0;

    if (IKS01A2_MOTION_SENSOR_Reset_Step_Counter(IKS01A2_LSM6DSL_0) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
  }

  if (MemsEventDetected != 0U)
  {
    MemsEventDetected = 0;

    if (IKS01A2_MOTION_SENSOR_Get_Event_Status(IKS01A2_LSM6DSL_0, &status) == BSP_ERROR_NONE)
    {
      if (status.StepStatus != 0U)
      {
        Send_Step_Count();
        BSP_LED_On(LED2);
        HAL_Delay(INDICATION_DELAY);
        BSP_LED_Off(LED2);
      }
    }
  }

  /* After defined timeout send time stamp and step count to UART. */
  if (user_currentTimeGetElapsedMS(previous_tick) >= SEND_STEP_COUNT_TIMEOUT)
  {
    Send_Step_Count();
    previous_tick = user_currentTimeGetTick();
  }
}

/**
  * @brief  Provides SysTick time in milliseconds
  * @retval SysTick time
  */
static uint32_t user_currentTimeGetTick(void)
{
  return HAL_GetTick();
}

/**
  * @brief  Provides a SysTick time elapsed from 'Tick1' start value in milliseconds
  * @param  Tick1 Elapsed time start value
  * @retval SysTick time elapsed
  */
static uint32_t user_currentTimeGetElapsedMS(uint32_t Tick1)
{
  uint32_t delta;
  uint32_t tick2;

  tick2 = HAL_GetTick();
  delta = tick2 - Tick1;
  return delta;
}

/**
  * @brief  Send time stamp and step count to UART
  * @retval None
  */
static void Send_Step_Count(void)
{
  if (IKS01A2_MOTION_SENSOR_Get_Step_Count(IKS01A2_LSM6DSL_0, &StepCount) != BSP_ERROR_NONE)
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "Error getting step count from LSM6DSL - accelerometer.\r\n");
  }
  else
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "Step count: %d\r\n", StepCount);
  }

  printf("%s", dataOut);
}

#ifdef __cplusplus
}
#endif

