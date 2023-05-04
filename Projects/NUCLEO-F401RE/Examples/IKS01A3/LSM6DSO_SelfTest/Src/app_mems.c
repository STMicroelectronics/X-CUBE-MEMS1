/**
  ******************************************************************************
  * File Name          : app_mems.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.9.6.0 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_nucleo.h"
#include "iks01a3_motion_sensors.h"
#include "iks01a3_motion_sensors_ex.h"
#include <stdlib.h> /* abs */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  STATUS_SELFTEST,
  STATUS_SLEEP
} DEMO_STATUS;

/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE 256
#define INDICATION_DELAY  1000 /* LED is ON for this period [ms]. */

#define X_POWER_UP_DELAY    100 /*!< Delay after accelero power-up [ms] */
#define X_ST_ENABLED_DELAY  100 /*!< Delay after accelero self-test enabled [ms] */
#define G_POWER_UP_DELAY    150 /*!< Delay after gyro power-up [ms] */
#define G_ST_ENABLED_DELAY   50 /*!< Delay after gyro self-test enabled [ms] */

#define N_SAMPLES  5 /*!< Number of samples */

#define X_LO_LIM      90 /*!< Accelero low test limit [mg] */
#define X_HI_LIM    1700 /*!< Accelero high test limit [mg] */
#define G_LO_LIM  150000 /*!< Gyro low test limit [mdps] */
#define G_HI_LIM  700000 /*!< Gyro high test limit [mdps] */

#define ST_REG_COUNT  (sizeof(reg_addr) / sizeof(uint8_t))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint8_t PushButtonDetected = 0;
static char dataOut[MAX_BUF_SIZE];
static DEMO_STATUS DemoStatus = STATUS_SLEEP;
/* Refer to Datasheet / Application Note documents for details about following register settings */
static uint8_t reg_addr[]        = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19};
static uint8_t x_st_reg_values[] = {0x38, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t g_st_reg_values[] = {0x00, 0x5C, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static int32_t PushButtonState = GPIO_PIN_RESET;

/* Private function prototypes -----------------------------------------------*/
static void MX_IKS01A3_LSM6DSO_SelfTest_Init(void);
static void MX_IKS01A3_LSM6DSO_SelfTest_Process(void);
static void Sleep_Mode(void);
static int32_t LSM6DSO_X_SelfTest(void);
static int32_t LSM6DSO_G_SelfTest(void);
static int32_t LSM6DSO_X_Get_Data(IKS01A3_MOTION_SENSOR_Axes_t *data);
static int32_t LSM6DSO_G_Get_Data(IKS01A3_MOTION_SENSOR_Axes_t *data);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS01A3_LSM6DSO_SelfTest_Init();

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

  MX_IKS01A3_LSM6DSO_SelfTest_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the LSM6DSO Self Test application
  * @retval None
  */
void MX_IKS01A3_LSM6DSO_SelfTest_Init(void)
{
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Check what is the Push Button State when the button is not pressed. It can change across families */
  PushButtonState = (BSP_PB_GetState(BUTTON_KEY)) ?  0 : 1;

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  (void)IKS01A3_MOTION_SENSOR_Init(IKS01A3_LSM6DSO_0, MOTION_ACCELERO | MOTION_GYRO);

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n------ LSM6DSO self-test DEMO ------\r\n");
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
  * @brief  Process of the LSM6DSO Self Test application
  * @retval None
  */
void MX_IKS01A3_LSM6DSO_SelfTest_Process(void)
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

    /* _NOTE_: Pushing button creates interrupt/event and wakes up MCU from sleep mode */
    DemoStatus = STATUS_SELFTEST;
  }

  /* Handle DEMO State Machine */
  switch (DemoStatus)
  {
    case STATUS_SELFTEST:
      if (LSM6DSO_X_SelfTest() != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      if (LSM6DSO_G_SelfTest() != BSP_ERROR_NONE)
      {
        Error_Handler();
      }
      DemoStatus = STATUS_SLEEP;
      break;

    case STATUS_SLEEP:
      (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nPress USER button to start the DEMO ...\r\n");
      printf("%s", dataOut);
      /* Enter sleep mode */
      Sleep_Mode();
      break;

    default:
      Error_Handler();
      break;
  }
}

/**
  * @brief  Enter sleep mode and wait for interrupt
  * @retval None
  */
static void Sleep_Mode(void)
{
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; /* Systick IRQ OFF */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; /* Systick IRQ ON */
}

/**
  * @brief  Performs LSM6DSO accelerometer self-test
  * @retval BSP status
  */
static int32_t LSM6DSO_X_SelfTest(void)
{
  int32_t test_result = BSP_ERROR_NONE;
  uint32_t i;
  IKS01A3_MOTION_SENSOR_Axes_t data_nost;
  IKS01A3_MOTION_SENSOR_Axes_t data_st;
  IKS01A3_MOTION_SENSOR_Axes_t data;
  uint8_t prev_reg_values[ST_REG_COUNT];
  int32_t ret;

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nStarting LSM6DSO accelerometer self-test ...\r\nKeep the device still!!!\r\n");
  printf("%s", dataOut);

  HAL_Delay(INDICATION_DELAY);
  BSP_LED_On(LED2);

  /* Store current settings of the sensor */
  for (i = 0; i < ST_REG_COUNT; i++)
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LSM6DSO_0, reg_addr[i], &prev_reg_values[i])) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  /* Set the sensor for self-test */
  for (i = 0; i < ST_REG_COUNT; i++)
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, reg_addr[i], x_st_reg_values[i])) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  /* Wait defined time for stable output */
  HAL_Delay(X_POWER_UP_DELAY);

  /* Read first data and discard it */
  if (LSM6DSO_X_Get_Data(&data) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  data_nost.x = 0;
  data_nost.y = 0;
  data_nost.z = 0;

  /* Read valid data multiple times and average it */
  for (i = 0; i < (uint32_t)N_SAMPLES; i++)
  {
    if (LSM6DSO_X_Get_Data(&data) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    data_nost.x += data.x;
    data_nost.y += data.y;
    data_nost.z += data.z;
  }
  data_nost.x /= N_SAMPLES;
  data_nost.y /= N_SAMPLES;
  data_nost.z /= N_SAMPLES;

  /* Enable self-test */
  if ((ret = IKS01A3_MOTION_SENSOR_Set_SelfTest(IKS01A3_LSM6DSO_0, MOTION_ACCELERO, (uint8_t)LSM6DSO_XL_ST_POSITIVE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Wait defined time for stable output */
  HAL_Delay(X_ST_ENABLED_DELAY);

  /* Read first data and discard it */
  if (LSM6DSO_X_Get_Data(&data) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  data_st.x = 0;
  data_st.y = 0;
  data_st.z = 0;

  /* Read valid data multiple times and average it */
  for (i = 0; i < (uint32_t)N_SAMPLES; i++)
  {
    if (LSM6DSO_X_Get_Data(&data) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    data_st.x += data.x;
    data_st.y += data.y;
    data_st.z += data.z;
  }
  data_st.x /= N_SAMPLES;
  data_st.y /= N_SAMPLES;
  data_st.z /= N_SAMPLES;

  /* Restore previous settings of the sensor */
  for (i = 0; i < ST_REG_COUNT; i++)
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, reg_addr[i], prev_reg_values[i])) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  /* Evaluate the test */
  if (abs(data_st.x - data_nost.x) < X_LO_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.x - data_nost.x) > X_HI_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.y - data_nost.y) < X_LO_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.y - data_nost.y) > X_HI_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.z - data_nost.z) < X_LO_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.z - data_nost.z) > X_HI_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Print measured data */
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nMeasured acceleration [mg]:\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n     AXIS     | PRE-SELFTEST |   SELFTEST\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "--------------|--------------|--------------\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "       X      | %8ld     | %8ld\r\n", (long)data_nost.x, (long)data_st.x);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "       Y      | %8ld     | %8ld\r\n", (long)data_nost.y, (long)data_st.y);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "       Z      | %8ld     | %8ld\r\n", (long)data_nost.z, (long)data_st.z);
  printf("%s", dataOut);

  /* Print test limits and data */
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nTest limits and data [mg]:\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n  LOW LIMIT   |  DIFFERENCE  |  HIGH LIMIT\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "--------------|--------------|--------------\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "%8d      | %8d     | %8d\r\n", X_LO_LIM, (int)abs(data_st.x - data_nost.x), X_HI_LIM);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "%8d      | %8d     | %8d\r\n", X_LO_LIM, (int)abs(data_st.y - data_nost.y), X_HI_LIM);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "%8d      | %8d     | %8d\r\n", X_LO_LIM, (int)abs(data_st.z - data_nost.z), X_HI_LIM);
  printf("%s", dataOut);

  /* Print the test result */
  if (test_result == BSP_ERROR_NONE)
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nLSM6DSO accelerometer self-test PASSED!\r\n");
  }
  else
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nLSM6DSO accelerometer self-test FAILED!\r\n");
  }
  printf("%s", dataOut);

  BSP_LED_Off(LED2);

  return ret;
}

/**
  * @brief  Performs LSM6DSO gyroscope self-test
  * @retval BSP status
  */
static int32_t LSM6DSO_G_SelfTest(void)
{
  uint32_t i;
  int32_t test_result = BSP_ERROR_NONE;
  IKS01A3_MOTION_SENSOR_Axes_t data_nost;
  IKS01A3_MOTION_SENSOR_Axes_t data_st;
  IKS01A3_MOTION_SENSOR_Axes_t data;
  uint8_t prev_reg_values[ST_REG_COUNT];
  int32_t ret;

  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nStarting LSM6DSO gyroscope self-test ...\r\nKeep the device still!!!\r\n");
  printf("%s", dataOut);

  HAL_Delay(INDICATION_DELAY);
  BSP_LED_On(LED2);

  /* Store current settings of the sensor */
  for (i = 0; i < ST_REG_COUNT; i++)
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LSM6DSO_0, reg_addr[i], &prev_reg_values[i])) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  /* Set the sensor for self-test */
  for (i = 0; i < ST_REG_COUNT; i++)
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, reg_addr[i], g_st_reg_values[i])) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  /* Wait defined time for stable output */
  HAL_Delay(G_POWER_UP_DELAY);

  /* Read first data and discard it */
  if (LSM6DSO_G_Get_Data(&data) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  data_nost.x = 0;
  data_nost.y = 0;
  data_nost.z = 0;

  /* Read valid data multiple times and average it */
  for (i = 0; i < (uint32_t)N_SAMPLES; i++)
  {
    if (LSM6DSO_G_Get_Data(&data) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    data_nost.x += data.x;
    data_nost.y += data.y;
    data_nost.z += data.z;
  }
  data_nost.x /= N_SAMPLES;
  data_nost.y /= N_SAMPLES;
  data_nost.z /= N_SAMPLES;

  /* Enable self-test */
  if ((ret = IKS01A3_MOTION_SENSOR_Set_SelfTest(IKS01A3_LSM6DSO_0, MOTION_GYRO, (uint8_t)LSM6DSO_GY_ST_POSITIVE)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  /* Wait defined time for stable output */
  HAL_Delay(G_ST_ENABLED_DELAY);

  /* Read first data and discard it */
  if (LSM6DSO_G_Get_Data(&data) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  data_st.x = 0;
  data_st.y = 0;
  data_st.z = 0;

  /* Read valid data multiple times and average it */
  for (i = 0; i < (uint32_t)N_SAMPLES; i++)
  {
    if (LSM6DSO_G_Get_Data(&data) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    data_st.x += data.x;
    data_st.y += data.y;
    data_st.z += data.z;
  }
  data_st.x /= N_SAMPLES;
  data_st.y /= N_SAMPLES;
  data_st.z /= N_SAMPLES;

  /* Restore previous settings of the sensor */
  for (i = 0; i < ST_REG_COUNT; i++)
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, reg_addr[i], prev_reg_values[i])) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }

  /* Evaluate the test */
  if (abs(data_st.x - data_nost.x) < G_LO_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.x - data_nost.x) > G_HI_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.y - data_nost.y) < G_LO_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.y - data_nost.y) > G_HI_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.z - data_nost.z) < G_LO_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }
  if (abs(data_st.z - data_nost.z) > G_HI_LIM)
  {
    test_result = BSP_ERROR_COMPONENT_FAILURE;
  }

  /* Print measured data */
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nMeasured angular velocity [mdps]:\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n     AXIS     | PRE-SELFTEST |   SELFTEST\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "--------------|--------------|--------------\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "       X      |  %8ld    |  %8ld\r\n", (long)data_nost.x, (long)data_st.x);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "       Y      |  %8ld    |  %8ld\r\n", (long)data_nost.y, (long)data_st.y);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "       Z      |  %8ld    |  %8ld\r\n", (long)data_nost.z, (long)data_st.z);
  printf("%s", dataOut);

  /* Print test limits and data */
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nTest limits and data [mdps]:\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\n  LOW LIMIT   |  DIFFERENCE  |  HIGH LIMIT\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "--------------|--------------|--------------\r\n");
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "  %8d    |  %8d    |  %8d\r\n", G_LO_LIM, (int)abs(data_st.x - data_nost.x), G_HI_LIM);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "  %8d    |  %8d    |  %8d\r\n", G_LO_LIM, (int)abs(data_st.y - data_nost.y), G_HI_LIM);
  printf("%s", dataOut);
  (void)snprintf(dataOut, MAX_BUF_SIZE, "  %8d    |  %8d    |  %8d\r\n", G_LO_LIM, (int)abs(data_st.z - data_nost.z), G_HI_LIM);
  printf("%s", dataOut);

  /* Print the test result */
  if (test_result == BSP_ERROR_NONE)
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nLSM6DSO gyroscope self-test PASSED!\r\n");
  }
  else
  {
    (void)snprintf(dataOut, MAX_BUF_SIZE, "\r\nLSM6DSO gyroscope self-test FAILED!\r\n");
  }
  printf("%s", dataOut);

  BSP_LED_Off(LED2);

  return ret;
}

/**
  * @brief  Wait for data ready and get data
  * @param  data the sensor data
  * @retval None
  */
static int32_t LSM6DSO_X_Get_Data(IKS01A3_MOTION_SENSOR_Axes_t *data)
{
  uint8_t status;
  int32_t ret;

  /* Wait for data ready */
  do
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Get_DRDY_Status(IKS01A3_LSM6DSO_0, MOTION_ACCELERO, &status)) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }
  while (status == 0U);

  /* Read accelero data */
  if ((ret = IKS01A3_MOTION_SENSOR_GetAxes(IKS01A3_LSM6DSO_0, MOTION_ACCELERO, data)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  return ret;
}

/**
  * @brief  Wait for data ready and get data
  * @param  data the sensor data
  * @retval BSP status
  */
static int32_t LSM6DSO_G_Get_Data(IKS01A3_MOTION_SENSOR_Axes_t *data)
{
  uint8_t status;
  int32_t ret;

  /* Wait for data ready */
  do
  {
    if ((ret = IKS01A3_MOTION_SENSOR_Get_DRDY_Status(IKS01A3_LSM6DSO_0, MOTION_GYRO, &status)) != BSP_ERROR_NONE)
    {
      return ret;
    }
  }
  while (status == 0U);

  /* Read accelero data */
  if ((ret = IKS01A3_MOTION_SENSOR_GetAxes(IKS01A3_LSM6DSO_0, MOTION_GYRO, data)) != BSP_ERROR_NONE)
  {
    return ret;
  }

  return ret;
}

#ifdef __cplusplus
}
#endif
