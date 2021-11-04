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
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_exti.h"
#include "stm32l4xx_nucleo.h"
#include "iks01a3_env_sensors.h"
#include "iks01a3_env_sensors_ex.h"
#include <math.h> /* trunc */

/* Private typedef -----------------------------------------------------------*/
/**
 * @brief  Float to Integer type
 */
typedef struct displayFloatToInt_s
{
  int8_t sign; /* 0 means positive, 1 means negative*/
  uint32_t out_int;
  uint32_t out_dec;
} displayFloatToInt_t;

/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE 256
#define INDICATION_DELAY             100  /* LED is ON for this period [ms]. */
#define SEND_STEP_COUNT_TIMEOUT    2000U  /* Send step count to UART after this timeout [ms]. */
#define LOW_TEMPERATURE_THRESHOLD  10.0f
#define HIGH_TEMPERATURE_THRESHOLD 28.0f

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t MemsEventDetected = 0;
static volatile uint8_t PushButtonDetected = 0;
static char dataOut1[MAX_BUF_SIZE];
static char dataOut2[MAX_BUF_SIZE];
static uint32_t previous_tick = 0;
static uint8_t HighTemp = 0;
static uint8_t LowTemp = 0;
static float Temperature;
static int32_t PushButtonState = GPIO_PIN_RESET;

/* Private function prototypes -----------------------------------------------*/
static void MX_IKS01A3_STTS751_TemperatureLimit_Init(void);
static void MX_IKS01A3_STTS751_TemperatureLimit_Process(void);
static uint32_t user_currentTimeGetTick(void);
static uint32_t user_currentTimeGetElapsedMS(uint32_t Tick1);
static void floatToInt(float in, displayFloatToInt_t *out_value, int32_t dec_prec);
static void Send_Temperature(void);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS01A3_STTS751_TemperatureLimit_Init();

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

  MX_IKS01A3_STTS751_TemperatureLimit_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the STTS751 Temperature Limit application
  * @retval None
  */
void MX_IKS01A3_STTS751_TemperatureLimit_Init(void)
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

  /* Sensor init */
  (void)IKS01A3_ENV_SENSOR_Init(IKS01A3_STTS751_0, ENV_TEMPERATURE);

  /* Setup output data rate */
  (void)IKS01A3_ENV_SENSOR_SetOutputDataRate(IKS01A3_STTS751_0, ENV_TEMPERATURE, 4.0f);

  /* Setup temperature limits */
  (void)IKS01A3_ENV_SENSOR_Set_Low_Temperature_Threshold(IKS01A3_STTS751_0, LOW_TEMPERATURE_THRESHOLD);
  (void)IKS01A3_ENV_SENSOR_Set_High_Temperature_Threshold(IKS01A3_STTS751_0, HIGH_TEMPERATURE_THRESHOLD);

  /* Enable interrupt on STTS751 EVENT pin */
  (void)IKS01A3_ENV_SENSOR_Set_Event_Pin(IKS01A3_STTS751_0, 1U);

  /* Read status register to clear previous value */
  (void)IKS01A3_ENV_SENSOR_Get_Temperature_Limit_Status(IKS01A3_STTS751_0, NULL, NULL, NULL);

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
  * @brief  Process of the STTS751 Temperature Limit application
  * @retval None
  */
void MX_IKS01A3_STTS751_TemperatureLimit_Process(void)
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

    /* Do nothing */
  }

  /* EVENT interrupt */
  if (MemsEventDetected != 0U)
  {
    MemsEventDetected = 0;

    /* Read temperature limits status */
    /* Reading the status byte clear the interrupt and status bits */
    if (IKS01A3_ENV_SENSOR_Get_Temperature_Limit_Status(IKS01A3_STTS751_0, &HighTemp, &LowTemp, NULL) == BSP_ERROR_NONE)
    {
      /* If high or low limit was reached read actual temperature and blink with the LED */
      if ((HighTemp != 0U) || (LowTemp != 0U))
      {
        (void)IKS01A3_ENV_SENSOR_GetValue(IKS01A3_STTS751_0, ENV_TEMPERATURE, &Temperature);

        BSP_LED_On(LED2);
        HAL_Delay(INDICATION_DELAY);
        BSP_LED_Off(LED2);
      }
    }
  }

  /* After defined timeout send temperature value to UART. */
  if (user_currentTimeGetElapsedMS(previous_tick) >= SEND_STEP_COUNT_TIMEOUT)
  {
    Send_Temperature();
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
  * @brief  Splits a float into two integer values.
  * @param  in the float value as input
  * @param  out_value the pointer to the output integer structure
  * @param  dec_prec the decimal precision to be used
  * @retval None
  */
static void floatToInt(float in, displayFloatToInt_t *out_value, int32_t dec_prec)
{
  if(in >= 0.0f)
  {
    out_value->sign = 0;
  }else
  {
    out_value->sign = 1;
    in = -in;
  }

  in = in + (0.5 / pow(10, dec_prec));
  out_value->out_int = (int32_t)in;
  in = in - (float)(out_value->out_int);
  out_value->out_dec = (int32_t)trunc(in * pow(10, dec_prec));
}

/**
  * @brief  Send temperature value and temperature limits status to UART.
  * @retval None
  */
static void Send_Temperature(void)
{
  displayFloatToInt_t out_value;

  /* If no interrupt read actual temperature */
  if ((HighTemp == 0U) && (LowTemp == 0U))
  {
    (void)IKS01A3_ENV_SENSOR_GetValue(IKS01A3_STTS751_0, ENV_TEMPERATURE, &Temperature);
  }

  /* Print temperature */
  floatToInt(Temperature, &out_value, 2);
  (void)snprintf(dataOut1, MAX_BUF_SIZE, "Temperature: %c%d.%02d degC ", ((out_value.sign > 0) ? '-' : '+'), (int)out_value.out_int, (int)out_value.out_dec);

  if (HighTemp != 0U)
  {
    floatToInt(HIGH_TEMPERATURE_THRESHOLD, &out_value, 2);
    (void)snprintf(dataOut2, MAX_BUF_SIZE, "- High Temperature Limit! [%c%d.%02d degC]\r\n", ((out_value.sign > 0) ? '-' : '+'), (int)out_value.out_int, (int)out_value.out_dec);
    HighTemp = 0;
  }
  else if (LowTemp != 0U)
  {
    floatToInt(LOW_TEMPERATURE_THRESHOLD, &out_value, 2);
    (void)snprintf(dataOut2, MAX_BUF_SIZE, "- Low Temperature Limit! [%c%d.%02d degC]\r\n", ((out_value.sign > 0) ? '-' : '+'), (int)out_value.out_int, (int)out_value.out_dec);
    LowTemp = 0;
  }
  else
  {
    (void)snprintf(dataOut2, MAX_BUF_SIZE, "\r\n");
  }

  (void)strcat(dataOut1, dataOut2);

  printf("%s", dataOut1);
}

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
