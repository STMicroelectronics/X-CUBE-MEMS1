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
#include "stm32u5xx_hal.h"
#include "stm32u5xx_hal_exti.h"
#include "stm32u5xx_nucleo.h"
#include "iks01a2_motion_sensors.h"
#include "iks01a2_motion_sensors_ex.h"

/* Private typedef -----------------------------------------------------------*/
/**
 * @brief  Handle DEMO State Machine
 */
typedef enum
{
  STATUS_IDLE,
  STATUS_SET_FIFO_CONTINUOUS_MODE,
  STATUS_FIFO_RUN,
  STATUS_FIFO_DOWNLOAD,
  STATUS_SET_FIFO_BYPASS_MODE
} DEMO_FIFO_STATUS_t;

/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE 256
#define FIFO_WATERMARK   91 /*!< FIFO size limit */
#define SAMPLE_LIST_MAX  10U /*!< Max. number of values (X,Y,Z) to be printed to UART */

#define LSM6DSL_SAMPLE_ODR      10.0f /*!< Sample Output Data Rate [Hz] */
#define LSM6DSL_FIFO_MAX_ODR  6600.0f /*!< LSM6DSL FIFO maximum ODR */

#define ENABLE  1 /*!< Enable LSM6DSL FIFO functions */

#define INDICATION_DELAY  100 /* LED is ON for this period [ms]. */

#define PATTERN_GYR_X_AXIS  0 /*!< Pattern of gyro X axis */
#define PATTERN_GYR_Y_AXIS  1 /*!< Pattern of gyro Y axis */
#define PATTERN_GYR_Z_AXIS  2 /*!< Pattern of gyro Z axis */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t MemsEventDetected = 0;
static volatile uint8_t PushButtonDetected = 0;
static DEMO_FIFO_STATUS_t DemoFifoStatus = STATUS_SET_FIFO_BYPASS_MODE;
static char dataOut[MAX_BUF_SIZE];
static uint8_t fifo_full_status = 0;
static uint16_t num_samples = 0;
static uint16_t prev_num_samples = 0;
static int32_t PushButtonState = GPIO_PIN_RESET;

/* Private function prototypes -----------------------------------------------*/
static void MX_IKS01A2_LSM6DSL_FIFOContinuousMode_Init(void);
static void MX_IKS01A2_LSM6DSL_FIFOContinuousMode_Process(void);
static int32_t LSM6DSL_FIFO_Set_Bypass_Mode(void);
static int32_t LSM6DSL_FIFO_Set_Continuous_Mode(void);
static int32_t LSM6DSL_Read_All_FIFO_Data(void);
static int32_t LSM6DSL_Read_Single_FIFO_Pattern_Cycle(uint16_t SampleIndex);
static int32_t LSM6DSL_FIFO_Demo_Config(void);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS01A2_LSM6DSL_FIFOContinuousMode_Init();

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

  MX_IKS01A2_LSM6DSL_FIFOContinuousMode_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the LSM6DSL FIFO Continuous Mode application
  * @retval None
  */
void MX_IKS01A2_LSM6DSL_FIFOContinuousMode_Init(void)
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

  (void)IKS01A2_MOTION_SENSOR_Init(IKS01A2_LSM6DSL_0, MOTION_GYRO);

  /* Configure LSM6DSL sensor for the DEMO application */
  if (LSM6DSL_FIFO_Demo_Config() != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n------ LSM6DSL FIFO Continuous Mode DEMO ------\r\n");
  printf("%s", dataOut);
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
  * @brief  Process of the LSM6DSL FIFO Continuous Mode application
  * @retval None
  */
void MX_IKS01A2_LSM6DSL_FIFOContinuousMode_Process(void)
{
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

    switch (DemoFifoStatus)
    {
      /* If FIFO is in Bypass mode switch to Continuous mode */
      case STATUS_IDLE:
        DemoFifoStatus = STATUS_SET_FIFO_CONTINUOUS_MODE;
        break;
      /* If FIFO is in Continuous mode switch to Bypass mode */
      case STATUS_FIFO_RUN:
        DemoFifoStatus = STATUS_SET_FIFO_BYPASS_MODE;
        break;
      /* Otherwise do nothing */
      case STATUS_SET_FIFO_CONTINUOUS_MODE:
      case STATUS_FIFO_DOWNLOAD:
      case STATUS_SET_FIFO_BYPASS_MODE:
        break;
      default:
        Error_Handler();
        break;
    }
  }

  /* Handle DEMO State Machine */
  switch (DemoFifoStatus)
  {
    case STATUS_IDLE:
      break;

    case STATUS_SET_FIFO_CONTINUOUS_MODE:
      if (LSM6DSL_FIFO_Set_Continuous_Mode() != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      DemoFifoStatus = STATUS_FIFO_RUN;
      break;

    case STATUS_FIFO_RUN:
      /* Get num of unread FIFO samples before reading data */
      if (IKS01A2_MOTION_SENSOR_FIFO_Get_Num_Samples(IKS01A2_LSM6DSL_0, &num_samples) != BSP_ERROR_NONE)
      {
        Error_Handler();
      }

      /* Print dot realtime whenever new data is stored in FIFO */
      if (num_samples != prev_num_samples)
      {
        prev_num_samples = num_samples;
        (void)snprintf(dataOut, MAX_BUF_SIZE, ".");
        printf("%s", dataOut);
        fflush(stdout);
      }
      if (MemsEventDetected == 1U)
      {
        DemoFifoStatus = STATUS_FIFO_DOWNLOAD;
        MemsEventDetected = 0;
      }
      break;

    case STATUS_FIFO_DOWNLOAD:
      /* Print data if FIFO is full */
      if (IKS01A2_MOTION_SENSOR_FIFO_Get_Full_Status(IKS01A2_LSM6DSL_0, &fifo_full_status) != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      if (fifo_full_status == 1U)
      {
        BSP_LED_On(LED2);
        if (LSM6DSL_Read_All_FIFO_Data() != BSP_ERROR_NONE)
        {
          Error_Handler();
        }
        BSP_LED_Off(LED2);
        DemoFifoStatus = STATUS_FIFO_RUN;
      }
      break;

    case STATUS_SET_FIFO_BYPASS_MODE:
      if (LSM6DSL_FIFO_Set_Bypass_Mode() != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      MemsEventDetected = 0;
      num_samples = 0;
      prev_num_samples = 0;
      DemoFifoStatus = STATUS_IDLE;
      break;

    default:
      Error_Handler();
      break;
  }
}

/**
  * @brief  Configure FIFO
  * @retval BSP status
  */
static int32_t LSM6DSL_FIFO_Demo_Config(void)
{
  int32_t ret;

  if ((ret = IKS01A2_MOTION_SENSOR_SetOutputDataRate(IKS01A2_LSM6DSL_0, MOTION_GYRO, LSM6DSL_SAMPLE_ODR)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set gyro FIFO decimation */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_Decimation(IKS01A2_LSM6DSL_0, MOTION_GYRO, (uint8_t)LSM6DSL_FIFO_GY_NO_DEC)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO ODR to highest value */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_ODR_Value(IKS01A2_LSM6DSL_0, LSM6DSL_FIFO_MAX_ODR)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO_FULL on INT1 */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(IKS01A2_LSM6DSL_0, ENABLE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO watermark */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_Watermark_Level(IKS01A2_LSM6DSL_0, FIFO_WATERMARK)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO depth to be limited to watermark threshold level  */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(IKS01A2_LSM6DSL_0, ENABLE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  return ret;
}

/**
  * @brief  Set FIFO bypass mode
  * @retval BSP status
  */
static int32_t LSM6DSL_FIFO_Set_Bypass_Mode(void)
{
  int32_t ret;

  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_Mode(IKS01A2_LSM6DSL_0, (uint8_t)LSM6DSL_BYPASS_MODE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nFIFO is stopped in Bypass mode.\r\n");
  printf("%s", dataOut);

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nPress USER button to start the DEMO...\r\n");
  printf("%s", dataOut);

  return ret;
}

/**
  * @brief  Set FIFO to Continuous mode
  * @retval BSP status
  */
static int32_t LSM6DSL_FIFO_Set_Continuous_Mode(void)
{
  int32_t ret;

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nLSM6DSL starts to store the data into FIFO...\r\n\r\n");
  printf("%s", dataOut);

  HAL_Delay(1000);

  /* Set FIFO mode to Continuous */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Set_Mode(IKS01A2_LSM6DSL_0, (uint8_t)LSM6DSL_STREAM_MODE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  return ret;
}

/**
  * @brief  Read all unread FIFO data in cycle
  * @retval BSP status
  */
static int32_t LSM6DSL_Read_All_FIFO_Data(void)
{
  uint16_t samples_to_read = 0;
  uint16_t i;
  int32_t ret;

  /* Get num of unread FIFO samples before reading data */
  if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Get_Num_Samples(IKS01A2_LSM6DSL_0, &samples_to_read)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* 'samples_to_read' actually contains number of words in FIFO but each FIFO sample (data set) consists of 3 words
  so the 'samples_to_read' has to be divided by 3 */
  samples_to_read /= 3U;

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n\r\n%d samples in FIFO.\r\n\r\nStarted downloading data from FIFO...\r\n\r\n", samples_to_read);
  printf("%s", dataOut);

  (void)snprintf(dataOut, MAX_BUF_SIZE, "[DATA ##]     GYR_X     GYR_Y     GYR_Z\r\n");
  printf("%s", dataOut);

  for (i = 0; i < samples_to_read; i++)
  {
    if ((ret = LSM6DSL_Read_Single_FIFO_Pattern_Cycle(i)) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  if (samples_to_read > SAMPLE_LIST_MAX)
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nSample list limited to: %d\r\n\r\n", SAMPLE_LIST_MAX);
    printf("%s", dataOut);
  }

  return ret;
}

/**
  * @brief  Read single FIFO pattern cycle
  * @param  SampleIndex Current sample index.
  * @retval BSP status
  */
static int32_t LSM6DSL_Read_Single_FIFO_Pattern_Cycle(uint16_t SampleIndex)
{
  uint16_t pattern = 0;
  int32_t angular_velocity = 0;
  int32_t gyr_x = 0, gyr_y = 0, gyr_z = 0;
  int32_t ret = BSP_ERROR_NONE;
  int i;

  /* Read one whole FIFO pattern cycle. Pattern: Gx, Gy, Gz */
  for (i = 0; i <= 2; i++)
  {
    /* Read FIFO pattern number */
    if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Get_Pattern(IKS01A2_LSM6DSL_0, &pattern)) != BSP_ERROR_NONE)
    {
      return ret;
    }

    /* Read single FIFO data (angular velocity in one axis) */
    if ((ret = IKS01A2_MOTION_SENSOR_FIFO_Get_Axis(IKS01A2_LSM6DSL_0, MOTION_GYRO, &angular_velocity)) != BSP_ERROR_NONE)
    {
      return ret;
    }

    /* Decide which axis has been read from FIFO based on pattern number */
    switch (pattern)
    {
      case PATTERN_GYR_X_AXIS:
        gyr_x = angular_velocity;
        break;

      case PATTERN_GYR_Y_AXIS:
        gyr_y = angular_velocity;
        break;

      case PATTERN_GYR_Z_AXIS:
        gyr_z = angular_velocity;
        break;

      default:
        ret = BSP_ERROR_UNKNOWN_FAILURE;
        break;
    }
  }

  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  if (SampleIndex < SAMPLE_LIST_MAX)
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "[DATA %02d]  %8ld  %8ld  %8ld\r\n", SampleIndex + 1U, (long)gyr_x, (long)gyr_y, (long)gyr_z);
    printf("%s", dataOut);
  }

  return ret;
}

#ifdef __cplusplus
}
#endif

