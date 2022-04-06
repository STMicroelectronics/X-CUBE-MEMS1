/**
 ******************************************************************************
 * File Name          : app_mems.c
 * Description        : This file provides code for the configuration
 *                      of the STMicroelectronics.X-CUBE-MEMS1.9.2.0 instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the "License". You may not use this component except in compliance with the
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

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "com.h"
#include "demo_serial.h"
#include "bsp_ip_conf.h"
#include "fw_version.h"
#include "motion_di_manager.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define ALGO_FREQ  100U /* Algorithm frequency 100Hz */
#define ACC_ODR  ((float)ALGO_FREQ)
#define ACC_FS  2 /* FS = <-2g, 2g> */
#define ALGO_PERIOD  (1000000U / ALGO_FREQ) /* Algorithm period [us] */
#define FROM_MG_TO_G  0.001f
#define FROM_G_TO_MG  1000.0f
#define FROM_MDPS_TO_DPS  0.001f
#define FROM_DPS_TO_MDPS  1000.0f
#define FROM_MGAUSS_TO_UT50  (0.1f/50.0f)
#define FROM_UT50_TO_MGAUSS  500.0f

/* Public variables ----------------------------------------------------------*/
volatile uint8_t DataLoggerActive = 0;
volatile uint32_t SensorsEnabled = 0;
char LibVersion[35];
int LibVersionLen;
volatile uint8_t SensorReadRequest = 0;
uint8_t UseOfflineData = 0;
offline_data_t OfflineData[OFFLINE_DATA_SIZE];
int OfflineDataReadIndex = 0;
int OfflineDataWriteIndex = 0;
int OfflineDataCount = 0;
uint32_t AlgoFreq = ALGO_FREQ;
MDI_cal_type_t AccCalMode = MDI_CAL_NONE;
MDI_cal_type_t GyrCalMode = MDI_CAL_NONE;

/* Extern variables ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MOTION_SENSOR_Axes_t AccValue;
static MOTION_SENSOR_Axes_t GyrValue;
static MOTION_SENSOR_Axes_t MagValue;
static float PressValue;
static float TempValue;
static float HumValue;
static int64_t Timestamp = 0;

/* Private function prototypes -----------------------------------------------*/
static void MX_DynamicInclinometer_Init(void);
static void MX_DynamicInclinometer_Process(void);
static void DI_Data_Handler(TMsg *Msg, TMsg *Cmd);
static void Init_Sensors(void);
static void RTC_Handler(TMsg *Msg);
static void Accelero_Sensor_Handler(TMsg *Msg);
static void Gyro_Sensor_Handler(TMsg *Msg);
static void Magneto_Sensor_Handler(TMsg *Msg);
static void Pressure_Sensor_Handler(TMsg *Msg);
static void Temperature_Sensor_Handler(TMsg *Msg);
static void Humidity_Sensor_Handler(TMsg *Msg);
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

  MX_DynamicInclinometer_Init();

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

  MX_DynamicInclinometer_Process();

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
  if (htim->Instance == BSP_IP_TIM_Handle.Instance)
  {
    SensorReadRequest = 1;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the application
  * @retval None
  */
static void MX_DynamicInclinometer_Init(void)
{
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  /* Initialize Timer */
  BSP_IP_TIM_Init();

  /* Configure Timer to run with desired algorithm frequency */
  TIM_Config(ALGO_FREQ);

  /* Initialize (disabled) sensors */
  Init_Sensors();

  /* DynamicInclinometer API initialization function */
  MotionDI_manager_init((int)ALGO_FREQ);

  /* OPTIONAL */
  /* Get library version */
  MotionDI_manager_get_version(LibVersion, &LibVersionLen);

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
static void MX_DynamicInclinometer_Process(void)
{
  static TMsg msg_dat;
  static TMsg msg_cmd;

  if (UART_ReceivedMSG((TMsg *)&msg_cmd) == 1)
  {
    if (msg_cmd.Data[0] == DEV_ADDR)
    {
      (void)HandleMSG((TMsg *)&msg_cmd);
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

    /* DynamicInclinometer specific part */
    DI_Data_Handler(&msg_dat, &msg_cmd);

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
static void RTC_Handler(TMsg *Msg)
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
 * @brief  Dynamic Inclinometer data handler
 * @param  Msg the Dynamic Inclinometer data part of the stream
 * @param  Cmd the Dynamic Inclinometer command to GUI
 * @retval None
 */
static void DI_Data_Handler(TMsg *Msg, TMsg *Cmd)
{
  uint32_t         elapsed_time_us = 0U;
  MDI_input_t      data_in;
  MDI_output_t     data_out;
  MDI_cal_type_t   acc_cal_mode;
  MDI_cal_type_t   gyro_cal_mode;
  MDI_cal_output_t acc_cal;
  MDI_cal_output_t gyro_cal;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    if ((SensorsEnabled & GYROSCOPE_SENSOR) == GYROSCOPE_SENSOR)
    {
      /* Convert acceleration from [mg] to [g] */
      data_in.Acc[0] = (float)AccValue.x * FROM_MG_TO_G;
      data_in.Acc[1] = (float)AccValue.y * FROM_MG_TO_G;
      data_in.Acc[2] = (float)AccValue.z * FROM_MG_TO_G;

      /* Convert angular velocity from [mdps] to [dps] */
      data_in.Gyro[0] = (float)GyrValue.x * FROM_MDPS_TO_DPS;
      data_in.Gyro[1] = (float)GyrValue.y * FROM_MDPS_TO_DPS;
      data_in.Gyro[2] = (float)GyrValue.z * FROM_MDPS_TO_DPS;

      data_in.Timestamp = Timestamp;
      Timestamp += ALGO_PERIOD;

      /* Run Dynamic Inclinometer algorithm */
      BSP_LED_On(LED2);
      DWT_Start();
      MotionDI_manager_run(&data_in, &data_out);
      elapsed_time_us = DWT_Stop();
      BSP_LED_Off(LED2);

      /* Check calibration mode */
      MotionDI_get_acc_calibration_mode(&acc_cal_mode);
      MotionDI_get_gyro_calibration_mode(&gyro_cal_mode);

      if (acc_cal_mode != AccCalMode)
      {
        AccCalMode = acc_cal_mode;

        INIT_STREAMING_HEADER(Cmd);
        Cmd->Data[2] = CMD_Calibration_Mode + CMD_Reply_Add;
        Cmd->Data[3] = (uint8_t)ACCELEROMETER_SENSOR;
        Cmd->Data[4] = (uint8_t)AccCalMode;
        Cmd->Len = 5;
        UART_SendMsg(Cmd);
      }

      if (gyro_cal_mode != GyrCalMode)
      {
        GyrCalMode = gyro_cal_mode;

        INIT_STREAMING_HEADER(Cmd);
        Cmd->Data[2] = CMD_Calibration_Mode + CMD_Reply_Add;
        Cmd->Data[3] = (uint8_t)GYROSCOPE_SENSOR;
        Cmd->Data[4] = (uint8_t)GyrCalMode;
        Cmd->Len = 5;
        UART_SendMsg(Cmd);
      }

      /* Get calibration parameters */
      MotionDI_get_acc_calibration(&acc_cal);
      MotionDI_get_gyro_calibration(&gyro_cal);

      /* Convert accelerometer calibration parameters from [g] to [mg] */
      acc_cal.Bias[0] *= FROM_G_TO_MG;
      acc_cal.Bias[1] *= FROM_G_TO_MG;
      acc_cal.Bias[2] *= FROM_G_TO_MG;

      /* Convert gyroscope calibration parameters from [dps] to [mdps] */
      gyro_cal.Bias[0] *= FROM_DPS_TO_MDPS;
      gyro_cal.Bias[1] *= FROM_DPS_TO_MDPS;
      gyro_cal.Bias[2] *= FROM_DPS_TO_MDPS;

      (void)memcpy(&Msg->Data[55], (void *)data_out.quaternion, 4U * sizeof(float));
      (void)memcpy(&Msg->Data[71], (void *)data_out.rotation, 3U * sizeof(float));
      (void)memcpy(&Msg->Data[83], (void *)data_out.gravity, 3U * sizeof(float));
      (void)memcpy(&Msg->Data[95], (void *)data_out.linear_acceleration, 3U * sizeof(float));

      (void)memcpy(&Msg->Data[107], (void *)acc_cal.Bias, 3U * sizeof(float));
      (void)memcpy(&Msg->Data[119], (void *) & (acc_cal.SF_Matrix[0][0]), sizeof(float));
      (void)memcpy(&Msg->Data[123], (void *) & (acc_cal.SF_Matrix[1][1]), sizeof(float));
      (void)memcpy(&Msg->Data[127], (void *) & (acc_cal.SF_Matrix[2][2]), sizeof(float));
      Msg->Data[131] = (uint8_t)acc_cal.CalQuality;

      (void)memcpy(&Msg->Data[132], (void *)gyro_cal.Bias, 3U * sizeof(float));
      Msg->Data[144] = (uint8_t)gyro_cal.CalQuality;

      Serialize_s32(&Msg->Data[145], (int32_t)elapsed_time_us, 4);
    }
  }
}

/**
 * @brief  Handles the ACC axes data getting/sending
 * @param  Msg the ACC part of the stream
 * @retval None
 */
static void Accelero_Sensor_Handler(TMsg *Msg)
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
static void Gyro_Sensor_Handler(TMsg *Msg)
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
static void Magneto_Sensor_Handler(TMsg *Msg)
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
static void Pressure_Sensor_Handler(TMsg *Msg)
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
static void Temperature_Sensor_Handler(TMsg *Msg)
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
static void Humidity_Sensor_Handler(TMsg *Msg)
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
  const uint32_t tim_counter_clock = 2000; /* TIM counter clock 2 kHz */
  uint32_t prescaler_value = (uint32_t)((SystemCoreClock / tim_counter_clock) - 1);
  uint32_t period = (tim_counter_clock / Freq) - 1;

  BSP_IP_TIM_Handle.Init.Prescaler = prescaler_value;
  BSP_IP_TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  BSP_IP_TIM_Handle.Init.Period = period;
  BSP_IP_TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  BSP_IP_TIM_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&BSP_IP_TIM_Handle) != HAL_OK)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
