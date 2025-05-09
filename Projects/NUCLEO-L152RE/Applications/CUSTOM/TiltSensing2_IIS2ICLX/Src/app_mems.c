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

#include "stm32l1xx_hal.h"
#include "stm32l1xx_nucleo.h"
#include "com.h"
#include "demo_serial.h"
#include "bsp_ip_conf.h"
#include "fw_version.h"
#include "motion_ac2_manager.h"
#include "motion_tl2_manager.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define ALGO_FREQ  100U /* Algorithm frequency 100Hz */
#define ACC_ODR  ((float)ALGO_FREQ)
#define ACC_FS  2000 /* FS = <-2g, 2g> */

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

/* Extern variables ----------------------------------------------------------*/
MTL2_knobs_t MTL2_Knobs;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MOTION_SENSOR_Axes_t AccValue;
static const uint32_t ReportInterval = 1000U / ALGO_FREQ;
static int64_t Timestamp = 0;
static MAC2_knobs_t MAC2_Knobs;
static float AccCalibrated[2] = {0.0f, 0.0f};

/* Private function prototypes -----------------------------------------------*/
static void AC2_Data_Handler(Msg_t *Msg);
static void MX_TiltSensing2_Init(void);
static void MX_TiltSensing2_Process(void);
static void TL2_Data_Handler(Msg_t *Msg);
static void Init_Sensors(void);
static void RTC_Handler(Msg_t *Msg);
static void Accelero2_Sensor_Handler(Msg_t *Msg);
static void TIM_Config(uint32_t Freq);
static void DWT_Init(void);
static void DWT_Start(void);
static uint32_t DWT_Stop(void);

#ifdef BSP_IP_MEMS_INT1_PIN_NUM
static void MEMS_INT1_Force_Low(void);
static void MEMS_INT1_Init(void);
#endif

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_TiltSensing2_Init();

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

  MX_TiltSensing2_Process();

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
static void MX_TiltSensing2_Init(void)
{
  float algo_freq;

#ifdef BSP_IP_MEMS_INT1_PIN_NUM
  /* Force MEMS INT1 pin of the sensor low during startup in order to disable I3C and enable I2C. This function needs
   * to be called only if user wants to disable I3C / enable I2C and didn't put the pull-down resistor to MEMS INT1 pin
   * on his HW setup. This is also the case of usage X-NUCLEO-IKS4A1 or X-NUCLEO-IKS01A3 expansion board together with
   * sensor in DIL24 adapter board where the LDO with internal pull-up is used.
   */
  MEMS_INT1_Force_Low();
#endif

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

#ifdef BSP_IP_MEMS_INT1_PIN_NUM
  /* Initialize MEMS INT1 pin back to it's default state after I3C disable / I2C enable */
  MEMS_INT1_Init();
#endif

  algo_freq = (float)ALGO_FREQ;
  /* AccelerometerCalibration2 API initialization function */
  MotionAC2_manager_init(&algo_freq);

  /* Get current settings and set desired ones */
  MotionAC2_GetKnobs(&MAC2_Knobs);
  MAC2_Knobs.FullScale = (float)ACC_FS;
  MAC2_Knobs.CalDuration_s = 120;
  MAC2_Knobs.XlNoiseScale = 1.0f;
  (void)MotionAC2_SetKnobs(&MAC2_Knobs);

  /* TiltSensing2 API initialization function */
  MotionTL2_manager_init();

  /* Get current settings and set desired ones */
  MotionTL2_GetKnobs(&MTL2_Knobs);
  MTL2_Knobs.fullscale = (float)ACC_FS;
  MTL2_Knobs.k = ACC_ODR;
  char orientation[2] = "en";
  MTL2_Knobs.orn[0] = orientation[0];
  MTL2_Knobs.orn[1] = orientation[1];
  MTL2_Knobs.mode = MTL2_DUAL_PLANE;
  (void)MotionTL2_SetKnobs(&MTL2_Knobs);

  /* OPTIONAL */
  /* Get library version */
  MotionTL2_manager_get_version(LibVersion, &LibVersionLen);

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
static void MX_TiltSensing2_Process(void)
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
    Accelero2_Sensor_Handler(&msg_dat);

    /* AccelerometerCalibration2 specific part */
    AC2_Data_Handler(&msg_dat);
    /* TiltSensing2 specific part */
    TL2_Data_Handler(&msg_dat);

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
  * @brief  Accelerometer Calibration 2-axis data handler
  * @param  Msg the Accelerometer Calibration data part of the stream
  * @retval None
  */
static void AC2_Data_Handler(Msg_t *Msg)
{
  uint8_t is_calibrated = 0U;
  MAC2_input_t data_in = {.Acc_X = 0.0f, .Acc_Y = 0.0f};
  MAC2_cal_params_t data_out;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    /* Convert raw accelero data from [mg] to [g] */
    data_in.Acc_X = (float)AccValue.x / 1000.0f;
    data_in.Acc_Y = (float)AccValue.y / 1000.0f;

    /* Run Accelerometer Calibration 2-axis algorithm */
    BSP_LED_On(LED2);
    MotionAC2_manager_update(&data_in, (uint64_t)Timestamp, &is_calibrated);
    BSP_LED_Off(LED2);

    /* Get the accelerometer compensation data */
    MotionAC2_manager_get_params(&data_out);

    /* Do offset & scale factor calibration */
    MotionAC2_manager_compensate(&AccValue, AccCalibrated);

    /* Calibrated data */
    FloatToArray(&Msg->Data[15], AccCalibrated[0]);
    FloatToArray(&Msg->Data[19], AccCalibrated[1]);

    /* Offset coefficients */
    FloatToArray(&Msg->Data[23], data_out.Bias[0]);
    FloatToArray(&Msg->Data[27], data_out.Bias[1]);

    /* Scale factor coefficients */
    FloatToArray(&Msg->Data[31], data_out.SF[0]);
    FloatToArray(&Msg->Data[35], data_out.SF[1]);

    /* Calibration quality */
    Serialize_s32(&Msg->Data[39], (int32_t)data_out.CalStatus, 4);

  }
}

/**
  * @brief  Tilt Sensing 2-axis data handler
  * @param  Msg the Tilt Sensing data part of the stream
  * @retval None
  */
static void TL2_Data_Handler(Msg_t *Msg)
{
  uint32_t elapsed_time_us = 0U;
  MTL2_input_t data_in = {.acc_x = 0.0f, .acc_y = 0.0f};
  static MTL2_output_t data_out = {.theta_2x = 0.0f, .psi_2x = 0.0f, .phi_2x = 0.0f, .tilt_1x = 0.0f, .err_deg = 0.0f, .valid = 0U};

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    /* Use calibrated acceleration data from AccelCalibration2 part of the application */
    data_in.acc_x = AccCalibrated[0];
    data_in.acc_y = AccCalibrated[1];

    /* Run Tilt Sensing algorithm */
    BSP_LED_On(LED2);
    DWT_Start();
    MotionTL2_manager_run(&data_in, Timestamp, &data_out);
    elapsed_time_us = DWT_Stop();
    BSP_LED_Off(LED2);

    Timestamp += (int64_t)ReportInterval;

    FloatToArray(&Msg->Data[43], data_out.tilt_1x);
    FloatToArray(&Msg->Data[47], data_out.theta_2x);
    FloatToArray(&Msg->Data[51], data_out.psi_2x);
    FloatToArray(&Msg->Data[55], data_out.phi_2x);
    FloatToArray(&Msg->Data[59], data_out.err_deg);
    Msg->Data[63] = (uint8_t)data_out.valid;
    Msg->Data[64] = (uint8_t)MTL2_Knobs.mode;

    Serialize_s32(&Msg->Data[65], (int32_t)elapsed_time_us, 4);
  }
}

/**
  * @brief  Handles the ACC 2-axis data getting/sending
  * @param  Msg the ACC part of the stream
  * @retval None
  */
static void Accelero2_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    if (UseOfflineData == 1)
    {
      AccValue.x = OfflineData[OfflineDataReadIndex].acceleration_x_mg;
      AccValue.y = OfflineData[OfflineDataReadIndex].acceleration_y_mg;
    }
    else
    {
      BSP_SENSOR_ACC_GetAxes(&AccValue);
    }

    FloatToArray(&Msg->Data[7], (float)AccValue.x / 1000.0f);
    FloatToArray(&Msg->Data[11], (float)AccValue.y / 1000.0f);
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

#ifdef BSP_IP_MEMS_INT1_PIN_NUM
/**
  * @brief  Force MEMS INT1 pin low
  * @param  None
  * @retval None
  */
static void MEMS_INT1_Force_Low(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = BSP_IP_MEMS_INT1_PIN_NUM;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_IP_MEMS_INT1_GPIOX, &GPIO_InitStruct);

  HAL_GPIO_WritePin(BSP_IP_MEMS_INT1_GPIOX, BSP_IP_MEMS_INT1_PIN_NUM, GPIO_PIN_RESET);
}

/**
  * @brief  Configure MEMS INT1 pin to the default state
  * @param  None
  * @retval None
  */
static void MEMS_INT1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = BSP_IP_MEMS_INT1_PIN_NUM;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_IP_MEMS_INT1_GPIOX, &GPIO_InitStruct);
}
#endif

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
