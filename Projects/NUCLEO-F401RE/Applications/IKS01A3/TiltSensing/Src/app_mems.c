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

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "com.h"
#include "demo_serial.h"
#include "bsp_ip_conf.h"
#include "fw_version.h"
#include "motion_tl_manager.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define ALGO_FREQ  50U /* Algorithm frequency >= 50Hz */
#define ACC_ODR  ((float)ALGO_FREQ)
#define ACC_FS  4 /* FS = <-4g, 4g> */

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
MTL_angle_mode_t AngleMode = MODE_PITCH_ROLL_GRAVITY_INCLINATION;

/* Extern variables ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MOTION_SENSOR_Axes_t AccValue;
static MOTION_SENSOR_Axes_t GyrValue;
static MOTION_SENSOR_Axes_t MagValue;
static float PressValue;
static float TempValue;
static float HumValue;

/* Private function prototypes -----------------------------------------------*/
static void MX_TiltSensing_Init(void);
static void MX_TiltSensing_Process(void);
static void TL_Data_Handler(Msg_t *Msg);
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

  MX_TiltSensing_Init();

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

  MX_TiltSensing_Process();

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

/**
  * @brief  Collect accelerometer data
  * @param  cal_data Pointer to 2D array of calibration data cal_data[num_records][3]
  * @param  num_records Number of records to be taken (3 axes per record)
  * @retval 0  Ok
  * @retval 1  Accelerometer error
  */
uint8_t CollectData(float cal_data[][3], uint32_t num_records)
{
  uint32_t i = 0;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) != ACCELEROMETER_SENSOR)
  {
    return 1;
  }

  /* Discard previous data */
  SensorReadRequest = 0;

  while (i < num_records)
  {
    if (SensorReadRequest == 1)
    {
      SensorReadRequest = 0;

      BSP_SENSOR_ACC_GetAxes(&AccValue);

      cal_data[i][0] = (float)AccValue.x / 1000.0f;
      cal_data[i][1] = (float)AccValue.y / 1000.0f;
      cal_data[i][2] = (float)AccValue.z / 1000.0f;
      i++;
    }
  }

  return 0;
}

/**
  * @brief  Get estimated measurement time
  * @param  time_s Pointer to time in [s]
  * @param  num_records Number of records taken
  * @retval None
  */
void GetEstimatedMeasTime(float *time_s, uint32_t num_records)
{
  float odr = 0.0f;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) != ACCELEROMETER_SENSOR)
  {
    *time_s = 0.0f;
    return;
  }

  BSP_SENSOR_ACC_Enable();

  BSP_SENSOR_ACC_GetOutputDataRate(&odr);

  if (odr > 0.001f)
  {
    *time_s = (float)num_records / odr;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the application
  * @retval None
  */
static void MX_TiltSensing_Init(void)
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

  /* TiltSensing API initialization function */
  MotionTL_manager_init();

  /* OPTIONAL */
  /* Get library version */
  MotionTL_manager_get_version(LibVersion, &LibVersionLen);

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
static void MX_TiltSensing_Process(void)
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

    /* TiltSensing specific part */
    TL_Data_Handler(&msg_dat);

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
  * @brief  Tilt Sensing data handler
  * @param  Msg the Tilt Sensing data part of the stream
  * @retval None
  */
static void TL_Data_Handler(Msg_t *Msg)
{
  uint32_t elapsed_time_us = 0U;
  MTL_input_t data_in =
  {
    .acc_x = 0.0f,
    .acc_y = 0.0f,
    .acc_z = 0.0f,
  };

  static MTL_output_t data_out =
  {
    .theta_3x = 0.0f,
    .psi_3x   = 0.0f,
    .phi_3x   = 0.0f,
    .roll_3x  = 0.0f,
    .pitch_3x = 0.0f,
    .err_deg  = 0.0f,
    .valid    = 0,
  };

  uint64_t timestamp_ms
    = (uint8_t)Msg->Data[3] * 3600000U
      + (uint8_t)Msg->Data[4] * 60000U
      + (uint8_t)Msg->Data[5] * 1000U
      + (uint8_t)Msg->Data[6] * 10U;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    /* Convert acceleration from [mg] to [g] */
    data_in.acc_x = (float)AccValue.x / 1000.0f;
    data_in.acc_y = (float)AccValue.y / 1000.0f;
    data_in.acc_z = (float)AccValue.z / 1000.0f;

    /* Run Tilt Sensing algorithm */
    BSP_LED_On(LED2);
    DWT_Start();
    MotionTL_manager_run(&data_in, timestamp_ms, &data_out);
    elapsed_time_us = DWT_Stop();
    BSP_LED_Off(LED2);

    switch (AngleMode)
    {
      default:
      case MODE_PITCH_ROLL_GRAVITY_INCLINATION:
        /* Get angles (pitch, roll and gravity inclination) */
        FloatToArray(&Msg->Data[55], data_out.pitch_3x);
        FloatToArray(&Msg->Data[59], data_out.roll_3x);
        FloatToArray(&Msg->Data[63], data_out.phi_3x);  /* Gravity Inclination */
        break;

      case MODE_THETA_PSI_PHI:
        /* Get angles (theta, psi and phi) */
        FloatToArray(&Msg->Data[55], data_out.theta_3x);
        FloatToArray(&Msg->Data[59], data_out.psi_3x);
        FloatToArray(&Msg->Data[63], data_out.phi_3x);
        break;
    }

    Serialize_s32(&Msg->Data[67], (int32_t)elapsed_time_us, 4);
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
