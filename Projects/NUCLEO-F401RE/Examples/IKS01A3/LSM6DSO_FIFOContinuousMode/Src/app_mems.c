/**
 ******************************************************************************
 * File Name          : app_mems.c
 * Description        : This file provides code for the configuration
 *                      of the STMicroelectronics.X-CUBE-MEMS1.9.1.0 instances.
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
#include "iks01a3_motion_sensors.h"
#include "iks01a3_motion_sensors_ex.h"

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
#define FIFO_WATERMARK   31 /*!< FIFO size limit */
#define SAMPLE_LIST_MAX  10U /*!< Max. number of values (X,Y,Z) to be printed to UART */

#define LSM6DSO_SAMPLE_ODR      10.0f /*!< Sample Output Data Rate [Hz] */
#define LSM6DSO_FIFO_MAX_ODR  6600.0f /*!< LSM6DSO FIFO maximum ODR */

#define ENABLE  1 /*!< Enable LSM6DSO FIFO functions */

#define INDICATION_DELAY  100 /* LED is ON for this period [ms]. */

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
static void MX_IKS01A3_LSM6DSO_FIFOContinuousMode_Init(void);
static void MX_IKS01A3_LSM6DSO_FIFOContinuousMode_Process(void);
static int32_t LSM6DSO_FIFO_Demo_Config(void);
static int32_t LSM6DSO_FIFO_Set_Bypass_Mode(void);
static int32_t LSM6DSO_FIFO_Set_Continuous_Mode(void);
static int32_t LSM6DSO_Read_All_FIFO_Data(void);
static int32_t LSM6DSO_Read_Single_FIFO_Data(uint16_t SampleIndex);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS01A3_LSM6DSO_FIFOContinuousMode_Init();

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

  MX_IKS01A3_LSM6DSO_FIFOContinuousMode_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the LSM6DSO FIFO Continuous Mode application
  * @retval None
  */
void MX_IKS01A3_LSM6DSO_FIFOContinuousMode_Init(void)
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

  (void)IKS01A3_MOTION_SENSOR_Init(IKS01A3_LSM6DSO_0, MOTION_GYRO);

  /* Configure LSM6DSO sensor for the DEMO application */
  if (LSM6DSO_FIFO_Demo_Config() != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n------ LSM6DSO FIFO Continuous Mode DEMO ------\r\n");
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
  * @brief  Process of the LSM6DSO FIFO Continuous Mode application
  * @retval None
  */
void MX_IKS01A3_LSM6DSO_FIFOContinuousMode_Process(void)
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
      if (LSM6DSO_FIFO_Set_Continuous_Mode() != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      DemoFifoStatus = STATUS_FIFO_RUN;
      break;

    case STATUS_FIFO_RUN:
      /* Get num of unread FIFO samples before reading data */
      if (IKS01A3_MOTION_SENSOR_FIFO_Get_Num_Samples(IKS01A3_LSM6DSO_0, &num_samples) != BSP_ERROR_NONE)
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
      if (IKS01A3_MOTION_SENSOR_FIFO_Get_Full_Status(IKS01A3_LSM6DSO_0, &fifo_full_status) != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      if (fifo_full_status == 1U)
      {
        BSP_LED_On(LED2);
        if (LSM6DSO_Read_All_FIFO_Data() != BSP_ERROR_NONE)
        {
          Error_Handler();
        }
        BSP_LED_Off(LED2);
        DemoFifoStatus = STATUS_FIFO_RUN;
      }
      break;

    case STATUS_SET_FIFO_BYPASS_MODE:
      if (LSM6DSO_FIFO_Set_Bypass_Mode() != BSP_ERROR_NONE)
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
static int32_t LSM6DSO_FIFO_Demo_Config(void)
{
  int32_t ret;

  if ((ret = IKS01A3_MOTION_SENSOR_SetOutputDataRate(IKS01A3_LSM6DSO_0, MOTION_GYRO, LSM6DSO_SAMPLE_ODR)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO gyro batch data rate to equal or higher value than gyro ODR */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Set_BDR(IKS01A3_LSM6DSO_0, MOTION_GYRO, LSM6DSO_XL_BATCHED_AT_6667Hz)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO_FULL on INT1 */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(IKS01A3_LSM6DSO_0, ENABLE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO watermark */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Set_Watermark_Level(IKS01A3_LSM6DSO_0, FIFO_WATERMARK)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Set FIFO depth to be limited to watermark threshold level  */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(IKS01A3_LSM6DSO_0, ENABLE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  return ret;
}

/**
  * @brief  Set FIFO bypass mode
  * @retval BSP status
  */
static int32_t LSM6DSO_FIFO_Set_Bypass_Mode(void)
{
  int32_t ret;

  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Set_Mode(IKS01A3_LSM6DSO_0, (uint8_t)LSM6DSO_BYPASS_MODE)) != BSP_ERROR_NONE)
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
static int32_t LSM6DSO_FIFO_Set_Continuous_Mode(void)
{
  int32_t ret;

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nLSM6DSO starts to store the data into FIFO...\r\n\r\n");
  printf("%s", dataOut);

  HAL_Delay(1000);

  /* Set FIFO mode to Continuous */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Set_Mode(IKS01A3_LSM6DSO_0, (uint8_t)LSM6DSO_STREAM_MODE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  return ret;
}

/**
  * @brief  Read all unread FIFO data in cycle
  * @retval BSP status
  */
static int32_t LSM6DSO_Read_All_FIFO_Data(void)
{
  uint16_t samples_to_read = 0;
  uint16_t i;
  int32_t ret;

  /* Get num of unread FIFO samples before reading data */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Get_Num_Samples(IKS01A3_LSM6DSO_0, &samples_to_read)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n\r\n%d samples in FIFO.\r\n\r\nStarted downloading data from FIFO...\r\n\r\n", samples_to_read);
  printf("%s", dataOut);

  (void)snprintf(dataOut, MAX_BUF_SIZE, "[DATA ##]     GYR_X     GYR_Y     GYR_Z\r\n");
  printf("%s", dataOut);

  for (i = 0; i < samples_to_read; i++)
  {
    if ((ret = LSM6DSO_Read_Single_FIFO_Data(i)) != BSP_ERROR_NONE)
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
  * @brief  Read single FIFO data
  * @param  SampleIndex Current sample index.
  * @retval BSP status
  */
static int32_t LSM6DSO_Read_Single_FIFO_Data(uint16_t SampleIndex)
{
  IKS01A3_MOTION_SENSOR_Axes_t angular_velocity;
  int32_t ret = BSP_ERROR_NONE;

  /* Read single FIFO data (angular velocity in all 3 axes) */
  if ((ret = IKS01A3_MOTION_SENSOR_FIFO_Get_Axes(IKS01A3_LSM6DSO_0, MOTION_GYRO, &angular_velocity)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  if (SampleIndex < SAMPLE_LIST_MAX)
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "[DATA %02d]  %8ld  %8ld  %8ld\r\n", SampleIndex + 1U, angular_velocity.x, angular_velocity.y, angular_velocity.z);
    printf("%s", dataOut);
  }

  return ret;
}

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
