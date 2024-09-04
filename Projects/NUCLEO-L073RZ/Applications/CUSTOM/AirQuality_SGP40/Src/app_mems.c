/**
  ******************************************************************************
  * File Name          : app_mems.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.11.0.0 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"
#include "com.h"
#include "demo_serial.h"
#include "bsp_ip_conf.h"
#include "fw_version.h"
#include "env_aq_manager.h"
#include "sensirion_gas_index_algorithm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define APP_FREQ  10U /* Application report frequency 10Hz */
#define PRESS_ODR  ((float)APP_FREQ)

/* Public variables ----------------------------------------------------------*/
volatile uint8_t DataLoggerActive = 0;
volatile uint32_t SensorsEnabled = 0;
char LibVersion[35];
int32_t LibVersionLen;
volatile uint8_t SensorReadRequest = 0;
uint8_t UseOfflineData = 0;
offline_data_t OfflineData[OFFLINE_DATA_SIZE];
int32_t OfflineDataReadIndex = 0;
int32_t OfflineDataWriteIndex = 0;
int32_t OfflineDataCount = 0;
uint32_t AppFreq = APP_FREQ;

/* Extern variables ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static float PressValue;
static float TempValue;
static float HumValue;
static float GasValue;
static const uint32_t ReportInterval = 1000U / APP_FREQ;
static const uint32_t SamplingInterval = GasIndexAlgorithm_DEFAULT_SAMPLING_INTERVAL * 1000.0f;

/* Private function prototypes -----------------------------------------------*/
static void MX_AirQuality_Init(void);
static void MX_AirQuality_Process(void);
static void AQ_Data_Handler(Msg_t *Msg);
static void Init_Sensors(void);
static void RTC_Handler(Msg_t *Msg);
static void Pressure_Sensor_Handler(Msg_t *Msg);
static void Temperature_Sensor_Handler(Msg_t *Msg);
static void Humidity_Sensor_Handler(Msg_t *Msg);
static void Gas_Sensor_Handler(Msg_t *Msg);
static void TIM_Config(uint32_t Freq);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_AirQuality_Init();

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

  MX_AirQuality_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Period elapsed callback
  * @param  htim pointer to a TIM_HandleTypeDef structure that contains
  *              the configuration information for TIM module.
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == BSP_IP_TIM_HANDLE.Instance)
  {
    SensorReadRequest = 1;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the application
  * @retval None
  */
static void MX_AirQuality_Init(void)
{
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  /* Initialize Timer */
  BSP_IP_TIM_INIT();

  /* Configure Timer to run with desired algorithm frequency */
  TIM_Config(APP_FREQ);

  /* Initialize (disabled) sensors */
  Init_Sensors();

  /* AirQuality API initialization function */
  EnvAQ_manager_init();

  /* OPTIONAL */
  /* Get library version */
  EnvAQ_manager_get_version(LibVersion, &LibVersionLen);

  BSP_LED_On(LED2);
  HAL_Delay(500);
  BSP_LED_Off(LED2);

  /* Start receiving messages via DMA */
  UART_StartReceiveMsg();
}

/**
  * @brief  Process of the application
  * @retval None
  */
static void MX_AirQuality_Process(void)
{
  static Msg_t msg_dat;
  static Msg_t msg_cmd;

  if (UART_ReceivedMSG((Msg_t *)&msg_cmd) == 1)
  {
    if (msg_cmd.Data[0] == DEV_ADDR)
    {
      (void)HandleMSG((Msg_t *)&msg_cmd);
    }
  }

  if (SensorReadRequest == 1U)
  {
    SensorReadRequest = 0;

    /* Acquire data from enabled sensors and fill Msg stream */
    RTC_Handler(&msg_dat);
    Humidity_Sensor_Handler(&msg_dat);
    Temperature_Sensor_Handler(&msg_dat);
    Pressure_Sensor_Handler(&msg_dat);
    Gas_Sensor_Handler(&msg_dat);

    /* AirQuality specific part */
    AQ_Data_Handler(&msg_dat);

    /* Send data stream */
    INIT_STREAMING_HEADER(&msg_dat);
    msg_dat.Len = STREAMING_MSG_LENGTH;

    if (UseOfflineData == 1U)
    {
      OfflineDataCount--;
      if (OfflineDataCount < 0)
      {
        OfflineDataCount = 0;
      }

      OfflineDataReadIndex++;
      if (OfflineDataReadIndex >= OFFLINE_DATA_SIZE)
      {
        OfflineDataReadIndex = 0;
      }

      if (OfflineDataCount > 0)
      {
        SensorReadRequest = 1;
      }
    }

    UART_SendMsg(&msg_dat);
  }
}

/**
  * @brief  Initialize all sensors
  * @param  None
  * @retval None
  */
static void Init_Sensors(void)
{
  BSP_SENSOR_PRESS_Init();
  BSP_SENSOR_TEMP_Init();
  BSP_SENSOR_HUM_Init();
  BSP_SENSOR_GAS_Init();

  BSP_SENSOR_PRESS_SetOutputDataRate(PRESS_ODR);
}

/**
  * @brief  Handles the time+date getting/sending
  * @param  Msg the time+date part of the stream
  * @retval None
  */
static void RTC_Handler(Msg_t *Msg)
{
  uint8_t sub_sec = 0;
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructure;
  uint32_t ans_uint32;
  int32_t ans_int32;
  uint32_t RtcSynchPrediv = hrtc.Init.SynchPrediv;

  if (UseOfflineData == 1)
  {
    Msg->Data[3] = (uint8_t)OfflineData[OfflineDataReadIndex].hours;
    Msg->Data[4] = (uint8_t)OfflineData[OfflineDataReadIndex].minutes;
    Msg->Data[5] = (uint8_t)OfflineData[OfflineDataReadIndex].seconds;
    Msg->Data[6] = (uint8_t)OfflineData[OfflineDataReadIndex].subsec;
  }
  else
  {
    (void)HAL_RTC_GetTime(&hrtc, &stimestructure, FORMAT_BIN);
    (void)HAL_RTC_GetDate(&hrtc, &sdatestructureget, FORMAT_BIN);

    /* To be MISRA C-2012 compliant the original calculation:
       sub_sec = ((((((int)RtcSynchPrediv) - ((int)stimestructure.SubSeconds)) * 100) / (RtcSynchPrediv + 1)) & 0xFF);
       has been split to separate expressions */
    ans_int32 = (RtcSynchPrediv - (int32_t)stimestructure.SubSeconds) * 100;
    ans_int32 /= RtcSynchPrediv + 1;
    ans_uint32 = (uint32_t)ans_int32 & 0xFFU;
    sub_sec = (uint8_t)ans_uint32;

    Msg->Data[3] = (uint8_t)stimestructure.Hours;
    Msg->Data[4] = (uint8_t)stimestructure.Minutes;
    Msg->Data[5] = (uint8_t)stimestructure.Seconds;
    Msg->Data[6] = sub_sec;
  }
}

/**
  * @brief  Air Quality data handler
  * @param  Msg the Air Quality data part of the stream
  * @retval None
  */
static void AQ_Data_Handler(Msg_t *Msg)
{
  uint32_t elapsed_time_us = 0U;
  uint32_t prev_tick = 0U;
  static uint32_t sampling_time = 0U;
  static int32_t air_quality_index = 0;

  if ((SensorsEnabled & GAS_SENSOR) == GAS_SENSOR)
  {
    sampling_time += ReportInterval;

    /* Run Air Quality algorithm */
    BSP_LED_On(LED2);
    if (sampling_time >= SamplingInterval)
    {
      sampling_time = 0U;
      prev_tick = HAL_GetTick();
      EnvAQ_manager_run((int32_t)GasValue, &air_quality_index);
      elapsed_time_us = (HAL_GetTick() - prev_tick) * 1000;
    }
    BSP_LED_Off(LED2);

    Serialize_s32(&Msg->Data[23], (int32_t)air_quality_index, 4);

    Serialize_s32(&Msg->Data[27], (int32_t)elapsed_time_us, 4);
  }
}

/**
  * @brief  Handles the PRESS sensor data getting/sending.
  * @param  Msg the PRESS part of the stream
  * @retval None
  */
static void Pressure_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & PRESSURE_SENSOR) == PRESSURE_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      PressValue = OfflineData[OfflineDataReadIndex].pressure;
    }
    else
    {
      BSP_SENSOR_PRESS_GetValue(&PressValue);
    }

    (void)memcpy(&Msg->Data[7], (void *)&PressValue, sizeof(float));
  }
}

/**
  * @brief  Handles the TEMP axes data getting/sending
  * @param  Msg the TEMP part of the stream
  * @retval None
  */
static void Temperature_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & TEMPERATURE_SENSOR) == TEMPERATURE_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      TempValue = OfflineData[OfflineDataReadIndex].temperature;
    }
    else
    {
      BSP_SENSOR_TEMP_GetValue(&TempValue);
    }

    (void)memcpy(&Msg->Data[11], (void *)&TempValue, sizeof(float));
  }
}

/**
  * @brief  Handles the HUM axes data getting/sending
  * @param  Msg the HUM part of the stream
  * @retval None
  */
static void Humidity_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & HUMIDITY_SENSOR) == HUMIDITY_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      HumValue = OfflineData[OfflineDataReadIndex].humidity;
    }
    else
    {
      BSP_SENSOR_HUM_GetValue(&HumValue);
    }

    (void)memcpy(&Msg->Data[15], (void *)&HumValue, sizeof(float));;
  }
}

/**
  * @brief  Handles the GAS data getting/sending
  * @param  Msg the GAS part of the stream
  * @retval None
  */
static void Gas_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & GAS_SENSOR) == GAS_SENSOR)
  {
    BSP_SENSOR_Gas_Set_Humidity(HumValue);
    BSP_SENSOR_Gas_Set_Temperature(TempValue);

    if (UseOfflineData == 1)
    {
      GasValue = OfflineData[OfflineDataReadIndex].gas;
    }
    else
    {
      BSP_SENSOR_GAS_GetValue(&GasValue);
    }

    (void)memcpy(&Msg->Data[19], (void *)&GasValue, sizeof(float));
  }
}

/**
  * @brief  Timer configuration
  * @param  Freq the desired Timer frequency
  * @retval None
  */
static void TIM_Config(uint32_t Freq)
{
  uint32_t tim_counter_clock;

  if (SystemCoreClock > 120000000)
  {
    tim_counter_clock = 4000; /* TIM counter clock 4 kHz */
  }
  else
  {
    tim_counter_clock = 2000; /* TIM counter clock 2 kHz */
  }
  uint32_t prescaler_value = (uint32_t)((SystemCoreClock / tim_counter_clock) - 1);
  uint32_t period = (tim_counter_clock / Freq) - 1;

  BSP_IP_TIM_HANDLE.Init.Prescaler = prescaler_value;
  BSP_IP_TIM_HANDLE.Init.CounterMode = TIM_COUNTERMODE_UP;
  BSP_IP_TIM_HANDLE.Init.Period = period;
  BSP_IP_TIM_HANDLE.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  BSP_IP_TIM_HANDLE.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&BSP_IP_TIM_HANDLE) != HAL_OK)
  {
    Error_Handler();
  }
}

#ifdef __cplusplus
}
#endif
