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

#include "stm32l1xx_hal.h"
#include "stm32l1xx_nucleo.h"
#include "com.h"
#include "demo_serial.h"
#include "bsp_ip_conf.h"
#include "fw_version.h"
#include "motion_ac_manager.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define ALGO_FREQ  50U /* Algorithm frequency >= 50Hz */
#define ACC_ODR  ((float)ALGO_FREQ)
#define ACC_FS  4 /* FS = <-4g, 4g> */
/* MOVE_THR_G recommended between 0.15 - 0.30 g, higher value will relax condition on data selection for calibration but
   reduce the accuracy which will be around (moveThresh_g / 10) */
#define MOVE_THR_G  0.2f

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
uint32_t AlgoFreq = ALGO_FREQ;
MAC_calibration_mode_t CalibrationMode = DYNAMIC_CALIBRATION;

/* Extern variables ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MOTION_SENSOR_Axes_t AccValue;
static MOTION_SENSOR_Axes_t GyrValue;
static MOTION_SENSOR_Axes_t MagValue;
static float PressValue;
static float TempValue;
static float HumValue;
static const uint32_t ReportInterval = 1000U / ALGO_FREQ;
static volatile uint32_t TimeStamp = 0;
static MAC_knobs_t Knobs;

/* Private function prototypes -----------------------------------------------*/
static void MX_AccelerometerCalibration_Init(void);
static void MX_AccelerometerCalibration_Process(void);
static void AC_Data_Handler(Msg_t *Msg);
static void Init_Sensors(void);
static void RTC_Handler(Msg_t *Msg);
static void Accelero_Sensor_Handler(Msg_t *Msg);
static void Gyro_Sensor_Handler(Msg_t *Msg);
static void Magneto_Sensor_Handler(Msg_t *Msg);
static void Pressure_Sensor_Handler(Msg_t *Msg);
static void Temperature_Sensor_Handler(Msg_t *Msg);
static void Humidity_Sensor_Handler(Msg_t *Msg);
static void TIM_Config(uint32_t Freq);
static void DWT_Init(void);
static void DWT_Start(void);
static uint32_t DWT_Stop(void);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_AccelerometerCalibration_Init();

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

  MX_AccelerometerCalibration_Process();

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
static void MX_AccelerometerCalibration_Init(void)
{
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  /* Initialize Timer */
  BSP_IP_TIM_INIT();

  /* Configure Timer to run with desired algorithm frequency */
  TIM_Config(ALGO_FREQ);

  /* Initialize (disabled) sensors */
  Init_Sensors();

  /* AccelerometerCalibration API initialization function */
  MotionAC_manager_init(MAC_ENABLE_LIB);

  /* Get current settings and set desired ones */
  MotionAC_GetKnobs(&Knobs);
  Knobs.MoveThresh_g = MOVE_THR_G;
  Knobs.Run6PointCal = (uint8_t)CalibrationMode;
  Knobs.Sample_ms = ReportInterval;
  (void)MotionAC_SetKnobs(&Knobs);

  /* OPTIONAL */
  /* Get library version */
  MotionAC_manager_get_version(LibVersion, &LibVersionLen);

  DWT_Init();

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
static void MX_AccelerometerCalibration_Process(void)
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
    Accelero_Sensor_Handler(&msg_dat);
    Gyro_Sensor_Handler(&msg_dat);
    Magneto_Sensor_Handler(&msg_dat);
    Humidity_Sensor_Handler(&msg_dat);
    Temperature_Sensor_Handler(&msg_dat);
    Pressure_Sensor_Handler(&msg_dat);

    /* AccelerometerCalibration specific part */
    AC_Data_Handler(&msg_dat);

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
  BSP_SENSOR_ACC_Init();
  BSP_SENSOR_GYR_Init();
  BSP_SENSOR_MAG_Init();
  BSP_SENSOR_PRESS_Init();
  BSP_SENSOR_TEMP_Init();
  BSP_SENSOR_HUM_Init();

  BSP_SENSOR_ACC_SetOutputDataRate(ACC_ODR);
  BSP_SENSOR_ACC_SetFullScale(ACC_FS);
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
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
  uint32_t ans_uint32;
  int32_t ans_int32;
  uint32_t RtcSynchPrediv = hrtc.Init.SynchPrediv;
#endif

  if (UseOfflineData == 1)
  {
    Msg->Data[3] = (uint8_t)OfflineData[OfflineDataReadIndex].hours;
    Msg->Data[4] = (uint8_t)OfflineData[OfflineDataReadIndex].minutes;
    Msg->Data[5] = (uint8_t)OfflineData[OfflineDataReadIndex].seconds;
    Msg->Data[6] = (uint8_t)OfflineData[OfflineDataReadIndex].subsec;
  }
  else
  {
    (void)HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
    (void)HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
    /* To be MISRA C-2012 compliant the original calculation:
       sub_sec = ((((((int)RtcSynchPrediv) - ((int)stimestructure.SubSeconds)) * 100) / (RtcSynchPrediv + 1)) & 0xFF);
       has been split to separate expressions */
    ans_int32 = (RtcSynchPrediv - (int32_t)stimestructure.SubSeconds) * 100;
    ans_int32 /= RtcSynchPrediv + 1;
    ans_uint32 = (uint32_t)ans_int32 & 0xFFU;
    sub_sec = (uint8_t)ans_uint32;
#endif

    Msg->Data[3] = (uint8_t)stimestructure.Hours;
    Msg->Data[4] = (uint8_t)stimestructure.Minutes;
    Msg->Data[5] = (uint8_t)stimestructure.Seconds;
    Msg->Data[6] = sub_sec;
  }
}

/**
  * @brief  Accelerometer Calibration data handler
  * @param  Msg the Accelerometer Calibration data part of the stream
  * @retval None
  */
static void AC_Data_Handler(Msg_t *Msg)
{
  uint32_t elapsed_time_us = 0U;
  uint8_t is_calibrated = 0;
  MAC_input_t data_in = {.Acc[0] = 0.0f, .Acc[1] = 0.0f, .Acc[2] = 0.0f, .TimeStamp = 0};
  MAC_output_t data_out;
  MOTION_SENSOR_Axes_t acc_comp;
  static MAC_calibration_mode_t prev_calibration_mode = DYNAMIC_CALIBRATION;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    if (CalibrationMode != prev_calibration_mode)
    {
      /* Reset library */
      MotionAC_manager_init(MAC_DISABLE_LIB);
      MotionAC_manager_init(MAC_ENABLE_LIB);

      /* Update knobs */
      Knobs.Run6PointCal = (uint8_t)CalibrationMode;
      (void)MotionAC_SetKnobs(&Knobs);

      prev_calibration_mode = CalibrationMode;
    }

    /* Convert acceleration from [mg] to [g] */
    data_in.Acc[0] = (float)AccValue.x / 1000.0f;
    data_in.Acc[1] = (float)AccValue.y / 1000.0f;
    data_in.Acc[2] = (float)AccValue.z / 1000.0f;
    data_in.TimeStamp = (int)TimeStamp;
    TimeStamp += ReportInterval;

    /* Run Accelerometer Calibration algorithm */
    BSP_LED_On(LED2);
    DWT_Start();
    MotionAC_manager_update(&data_in, &is_calibrated);
    elapsed_time_us = DWT_Stop();
    BSP_LED_Off(LED2);

    /* Get the accelerometer compensation */
    MotionAC_manager_get_params(&data_out);

    /* Do offset & scale factor calibration */
    MotionAC_manager_compensate(&AccValue, &acc_comp);

    /* Offset coefficients */
    Serialize_s32(&Msg->Data[55], (int32_t)acc_bias_to_mg(data_out.AccBias[0]), 4);
    Serialize_s32(&Msg->Data[59], (int32_t)acc_bias_to_mg(data_out.AccBias[1]), 4);
    Serialize_s32(&Msg->Data[63], (int32_t)acc_bias_to_mg(data_out.AccBias[2]), 4);

    /* Scale factor coefficients */
    FloatToArray(&Msg->Data[67], data_out.SF_Matrix[0][0]);
    FloatToArray(&Msg->Data[71], data_out.SF_Matrix[0][1]);
    FloatToArray(&Msg->Data[75], data_out.SF_Matrix[0][2]);

    FloatToArray(&Msg->Data[79], data_out.SF_Matrix[1][0]);
    FloatToArray(&Msg->Data[83], data_out.SF_Matrix[1][1]);
    FloatToArray(&Msg->Data[87], data_out.SF_Matrix[1][2]);

    FloatToArray(&Msg->Data[91], data_out.SF_Matrix[2][0]);
    FloatToArray(&Msg->Data[95], data_out.SF_Matrix[2][1]);
    FloatToArray(&Msg->Data[99], data_out.SF_Matrix[2][2]);

    /* Calibrated data */
    Serialize_s32(&Msg->Data[103], (int32_t) acc_comp.x, 4);
    Serialize_s32(&Msg->Data[107], (int32_t) acc_comp.y, 4);
    Serialize_s32(&Msg->Data[111], (int32_t) acc_comp.z, 4);

    /* Calibration quality */
    Serialize_s32(&Msg->Data[115], (int32_t) data_out.CalQuality, 4);

    Serialize_s32(&Msg->Data[119], (int32_t)elapsed_time_us, 4);
  }
}

/**
  * @brief  Handles the ACC axes data getting/sending
  * @param  Msg the ACC part of the stream
  * @retval None
  */
static void Accelero_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      AccValue.x = OfflineData[OfflineDataReadIndex].acceleration_x_mg;
      AccValue.y = OfflineData[OfflineDataReadIndex].acceleration_y_mg;
      AccValue.z = OfflineData[OfflineDataReadIndex].acceleration_z_mg;
    }
    else
    {
      BSP_SENSOR_ACC_GetAxes(&AccValue);
    }

    Serialize_s32(&Msg->Data[19], (int32_t)AccValue.x, 4);
    Serialize_s32(&Msg->Data[23], (int32_t)AccValue.y, 4);
    Serialize_s32(&Msg->Data[27], (int32_t)AccValue.z, 4);
  }
}

/**
  * @brief  Handles the GYR axes data getting/sending
  * @param  Msg the GYR part of the stream
  * @retval None
  */
static void Gyro_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & GYROSCOPE_SENSOR) == GYROSCOPE_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      GyrValue.x = OfflineData[OfflineDataReadIndex].angular_rate_x_mdps;
      GyrValue.y = OfflineData[OfflineDataReadIndex].angular_rate_y_mdps;
      GyrValue.z = OfflineData[OfflineDataReadIndex].angular_rate_z_mdps;
    }
    else
    {
      BSP_SENSOR_GYR_GetAxes(&GyrValue);
    }

    Serialize_s32(&Msg->Data[31], GyrValue.x, 4);
    Serialize_s32(&Msg->Data[35], GyrValue.y, 4);
    Serialize_s32(&Msg->Data[39], GyrValue.z, 4);
  }
}

/**
  * @brief  Handles the MAG axes data getting/sending
  * @param  Msg the MAG part of the stream
  * @retval None
  */
static void Magneto_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & MAGNETIC_SENSOR) == MAGNETIC_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      MagValue.x = OfflineData[OfflineDataReadIndex].magnetic_field_x_mgauss;
      MagValue.y = OfflineData[OfflineDataReadIndex].magnetic_field_y_mgauss;
      MagValue.z = OfflineData[OfflineDataReadIndex].magnetic_field_z_mgauss;
    }
    else
    {
      BSP_SENSOR_MAG_GetAxes(&MagValue);
    }

    Serialize_s32(&Msg->Data[43], MagValue.x, 4);
    Serialize_s32(&Msg->Data[47], MagValue.y, 4);
    Serialize_s32(&Msg->Data[51], MagValue.z, 4);
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

/**
  * @brief  Initialize DWT register for counting clock cycles purpose
  * @param  None
  * @retval None
  */
static void DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; /* Disable counter */
}

/**
  * @brief  Start counting clock cycles
  * @param  None
  * @retval None
  */
static void DWT_Start(void)
{
  DWT->CYCCNT = 0; /* Clear count of clock cycles */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; /* Enable counter */
}

/**
  * @brief  Stop counting clock cycles and calculate elapsed time in [us]
  * @param  None
  * @retval Elapsed time in [us]
  */
static uint32_t DWT_Stop(void)
{
  volatile uint32_t cycles_count = 0U;
  uint32_t system_core_clock_mhz = 0U;

  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; /* Disable counter */
  cycles_count = DWT->CYCCNT; /* Read count of clock cycles */

  /* Calculate elapsed time in [us] */
  system_core_clock_mhz = SystemCoreClock / 1000000U;
  return cycles_count / system_core_clock_mhz;
}

#ifdef __cplusplus
}
#endif
