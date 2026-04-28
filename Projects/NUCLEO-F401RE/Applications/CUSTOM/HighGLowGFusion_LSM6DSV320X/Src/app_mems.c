/**
  ******************************************************************************
  * File Name          : app_mems.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.12.1.0 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "motion_xlf_manager.h"
#include "highglowg_fsm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define ALGO_FREQ  ((float)ACCEL_FUSION_ODR_TEMP) /* Algorithm frequency Hz */
#define ACC_ODR  ((float)ALGO_FREQ)
#define ACC_FS  16 /* FS = <-16g, 16g> */
#define ACC_HG_FS  256 /* FS = <-256g, 256g> */
#define FROM_S_TO_MS  1000U
#define CLOCK_4KHZ    4000 /* TIM counter clock 4 kHz */
#define CLOCK_2KHZ    2000 /* TIM counter clock 2 kHz */

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
int8_t HighGEnable = 1;
uint16_t LowGODR = ACCEL_FUSION_ODR_TEMP;
uint16_t HighGODR = ACCEL_FUSION_ODR_TEMP;
uint8_t FSM_OUT1 = 0;
uint8_t FSM_OUT2 = 0;
uint16_t ReportRate = 30;
int32_t ContinuousTracking = 1;

/* Extern variables ----------------------------------------------------------*/
extern void *MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR];
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MOTION_SENSOR_Axes_t AccValue;
CUSTOM_MOTION_SENSOR_Axes_t HGAccValue;

/* Private function prototypes -----------------------------------------------*/
static void MX_HighGLowGFusion_Init(void);
static void MX_HighGLowGFusion_Process(void);
static void XLF_Data_Handler(Msg_t *Msg);
static void Init_Sensors(void);
static void Time_Handler(Msg_t *Msg);
static void Acc_Sensor_Handler(Msg_t *Msg);
static void TIM_Config(uint32_t Freq);
static void DWT_Init(void);
static void DWT_Start(void);
static uint64_t DWT_GetTickUS(void);
static void Disable_HighG(void);
static void Enable_HighG(void);
static void FSM_Init(void);
static void FSM_Handler(void);

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_HighGLowGFusion_Init();

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

  MX_HighGLowGFusion_Process();

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
static void MX_HighGLowGFusion_Init(void)
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

  /* HighGLowGFusion API initialization function */
  MotionXLF_manager_init();

  /* OPTIONAL */
  /* Get library version */
  MotionXLF_manager_get_version(LibVersion, &LibVersionLen);

  DWT_Init();
  DWT_Start();

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
static void MX_HighGLowGFusion_Process(void)
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
    Time_Handler(&msg_dat);
    Acc_Sensor_Handler(&msg_dat);
    FSM_Handler();

    /* High-g Low-g specific part */
    XLF_Data_Handler(&msg_dat);

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

  FSM_Init();

  BSP_SENSOR_ACC_SetFullScale(ACC_FS);

  LSM6DSV320X_ACC_HG_SetOutputDataRate(MotionCompObj[CUSTOM_LSM6DSV320X_0], HighGODR);
  LSM6DSV320X_ACC_HG_SetFullScale(MotionCompObj[CUSTOM_LSM6DSV320X_0], ACC_HG_FS);
  LSM6DSV320X_ACC_SetOutputDataRate_With_Mode(MotionCompObj[CUSTOM_LSM6DSV320X_0], LowGODR, LSM6DSV320X_ACC_HIGH_PERFORMANCE_MODE);

  LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_MD1_CFG, 0x02);
  LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FUNC_CFG_ACCESS, LSM6DSV320X_EMBED_FUNC_MEM_BANK << 7);
  LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FSM_INT1, 0x01);
  LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FUNC_CFG_ACCESS, LSM6DSV320X_MAIN_MEM_BANK << 7);

  Enable_HighG();
}

/**
  * @brief  Handles the precise time
  * @param  Msg the time part of the stream
  * @retval None
  */
static void Time_Handler(Msg_t *Msg)
{
  uint64_t time_us;

  time_us = DWT_GetTickUS();
  Serialize_s32(&Msg->Data[3], (int32_t)time_us, 4);
}

/**
  * @brief  High G Low G Fusion data handler
  * @param  Msg the High G Low G Fusion data part of the stream
  * @retval None
  */
static void XLF_Data_Handler(Msg_t *Msg)
{
  uint64_t start_time_us = 0U;
  uint64_t elapsed_time_us = 0U;
  XLF_in_t data_in;
  XLF_out_t data_out;
  XLF_algo_settings algo_set;

  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    /* Fill input data */
    data_in.low_g_data_mg.x = (float)AccValue.x;
    data_in.low_g_data_mg.y = (float)AccValue.y;
    data_in.low_g_data_mg.z = (float)AccValue.z;

    data_in.high_g_data_mg.x = (float)HGAccValue.x;
    data_in.high_g_data_mg.y = (float)HGAccValue.y;
    data_in.high_g_data_mg.z = (float)HGAccValue.z;

    data_in.FSM_OUT1 = FSM_OUT1;
    data_in.FSM_OUT2 = FSM_OUT2;

    algo_set.ACCEL_FUSION_CALIB_WINDOW_SIZE = 50;
    algo_set.ACCEL_FUSION_DIGITAL_OUTPUT = 0;
    algo_set.ACCEL_FUSION_ENABLE_NOISE_REMOVAL = 1;
    algo_set.ACCEL_FUSION_ENABLE_DISCONTINUITY_REMOVAL = 1;
    algo_set.ACCEL_FUSION_ENABLE_OFFSET_CALCULATOR = 1;
    algo_set.ACCEL_FUSION_CONTINUOUS_TRACKING = ContinuousTracking;
    algo_set.ACCEL_FUSION_LOWER_THRESHOLD = 16000;

    /* Run High-g Low-g Fusion algorithm */
    BSP_LED_On(LED2);
    start_time_us = DWT_GetTickUS();
    MotionXLF_manager_run(&data_in, &data_out, Enable_HighG, Disable_HighG, &algo_set);
    elapsed_time_us = DWT_GetTickUS() - start_time_us;
    BSP_LED_Off(LED2);

    FloatToArray(&Msg->Data[31], (float)data_out.fused_imu.x / 1000);
    FloatToArray(&Msg->Data[35], (float)data_out.fused_imu.y / 1000);
    FloatToArray(&Msg->Data[39], (float)data_out.fused_imu.z / 1000);

    Msg->Data[43] = (uint8_t)HighGEnable;

    Serialize_s32(&Msg->Data[44], (int32_t)LowGODR, 2);
    Serialize_s32(&Msg->Data[46], (int32_t)HighGODR, 2);

    Msg->Data[48] = (uint8_t)FSM_OUT1;
    Msg->Data[49] = (uint8_t)FSM_OUT2;

    Serialize_s32(&Msg->Data[50], (int32_t)elapsed_time_us, 4);
  }
}

/**
  * @brief  Handles the high-g and low-g ACC data getting/sending
  * @param  Msg the ACC part of the stream
  * @retval None
  */
static void Acc_Sensor_Handler(Msg_t *Msg)
{
  if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
  {
    BSP_SENSOR_ACC_GetAxes((MOTION_SENSOR_Axes_t *)&AccValue);
    LSM6DSV320X_HG_GetAxes((CUSTOM_MOTION_SENSOR_Axes_t *)&HGAccValue);

    FloatToArray(&Msg->Data[7], (float)((MOTION_SENSOR_Axes_t *)&AccValue)->x / 1000.0f);
    FloatToArray(&Msg->Data[11], (float)((MOTION_SENSOR_Axes_t *)&AccValue)->y / 1000.0f);
    FloatToArray(&Msg->Data[15], (float)((MOTION_SENSOR_Axes_t *)&AccValue)->z / 1000.0f);

    if (HighGEnable == 1)
    {
      FloatToArray(&Msg->Data[19], (float)((CUSTOM_MOTION_SENSOR_Axes_t *)&HGAccValue)->x / 1000.0f);
      FloatToArray(&Msg->Data[23], (float)((CUSTOM_MOTION_SENSOR_Axes_t *)&HGAccValue)->y / 1000.0f);
      FloatToArray(&Msg->Data[27], (float)((CUSTOM_MOTION_SENSOR_Axes_t *)&HGAccValue)->z / 1000.0f);
    }
    else
    {
      FloatToArray(&Msg->Data[19], (0.0f));
      FloatToArray(&Msg->Data[23], (0.0f));
      FloatToArray(&Msg->Data[27], (0.0f));
    }
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
    tim_counter_clock = CLOCK_4KHZ; /* TIM counter clock 4 kHz */
  }
  else
  {
    tim_counter_clock = CLOCK_2KHZ; /* TIM counter clock 2 kHz */
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
  * @brief  Get relative time in micro seconds
  * @note   Call at least every 2^32 cycles
  * @retval Relative time in micro seconds
  */
static uint64_t DWT_GetTickUS(void)
{
  static uint64_t last_cycle_count_64 = 0;
  uint32_t clock_MHz = HAL_RCC_GetHCLKFreq() / 1000000;

  __disable_irq();
  last_cycle_count_64 += DWT->CYCCNT - (uint32_t)(last_cycle_count_64);
  uint64_t result = last_cycle_count_64;
  __enable_irq();

  return result / clock_MHz;
}

/**
  * @brief  Enable High-g in the sensor
  * @param  None
  * @retval None
  */
static void Enable_HighG(void)
{
  (void)LSM6DSV320X_ACC_HG_Enable(MotionCompObj[CUSTOM_LSM6DSV320X_0]);
  (void)LSM6DSV320X_ACC_HG_SetOutputDataRate(MotionCompObj[CUSTOM_LSM6DSV320X_0], HighGODR);
  HighGEnable = 1; //external flag to signal high-g sensor has been enabled
}

/**
  * @brief  Disable High-g in the sensor
  * @param  None
  * @retval None
  */
static void Disable_HighG(void)
{
  (void)LSM6DSV320X_ACC_HG_Disable(MotionCompObj[CUSTOM_LSM6DSV320X_0]);
  HighGEnable = 0; //external flag to signal high-g sensor has been disabled
}

/**
  * @brief  Initialize FSM in the sensor
  * @param  None
  * @retval None
  */
static void FSM_Init(void)
{
  int i;
  int length = 0;
  length = sizeof(highglowg_fsm) / sizeof(ucf_line_ext_t);
  for (i = 0; i < length; i++)
  {
    (void)LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], highglowg_fsm[i].address, highglowg_fsm[i].data);
  }
}

/**
  * @brief  Handles the sensor FSM
  * @param  None
  * @retval None
  */
static void FSM_Handler(void)
{
  (void)LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FUNC_CFG_ACCESS,
                              LSM6DSV320X_EMBED_FUNC_MEM_BANK << 7);
  (void)LSM6DSV320X_Read_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FSM_OUTS1, &FSM_OUT1); //output of FSM 1
  (void)LSM6DSV320X_Read_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FSM_OUTS2, &FSM_OUT2); //output of FSM 2
  (void)LSM6DSV320X_Write_Reg(MotionCompObj[CUSTOM_LSM6DSV320X_0], LSM6DSV320X_FUNC_CFG_ACCESS, LSM6DSV320X_MAIN_MEM_BANK << 7);
}

/**
  * @brief  Set sensor report rate
  * @param  None
  * @retval None
  */
void Set_Sensor_Report_Rate(uint8_t index)
{
  switch (index)
  {
    case 0:
      ReportRate = 30;
      break;
    case 1:
      ReportRate = 60;
      break;
    case 2:
      ReportRate = 120;
      break;
    case 3:
      ReportRate = 240;
      break;
    case 4:
      ReportRate = 480;
      break;
    case 5:
      ReportRate = 960;
      break;
    default:
      ReportRate = 30;
      break;
  }

  TIM_Config(ReportRate);
}

/**
  * @brief  Set continuous mode value
  * @param  None
  * @retval None
  */
void Set_Continuous_Mode(uint8_t index)
{
  if (index <= 1)
  {
    ContinuousTracking = index;
    if (ContinuousTracking == 1)
    {
    	Enable_HighG();
    }
    else
    {
    	Disable_HighG();
    }
  }
}

/**
  * @brief  Set High-g and Low-g ODR
  * @param  None
  * @retval None
  */
void Set_Sensor_HighG_LowG_ODR(uint8_t index)
{
  switch (index)
  {
    case 0: /* 480Hz */
      HighGODR = 480;
      LowGODR = 480;
      break;
    case 1: /* 960Hz */
      HighGODR = 960;
      LowGODR = 960;
      break;
    case 2: /* 1.92kHz */
      HighGODR = 1920;
      LowGODR = 1920;
      break;
    case 3: /* 3.84kHz */
      HighGODR = 3840;
      LowGODR = 3840;
      break;
    case 4: /* 7.68kHz */
      HighGODR = 7680;
      LowGODR = 7680;
      break;
  }

  LSM6DSV320X_ACC_SetOutputDataRate_With_Mode(MotionCompObj[CUSTOM_LSM6DSV320X_0], LowGODR,
                                              LSM6DSV320X_ACC_HIGH_PERFORMANCE_MODE);
  HAL_Delay(100);
  if (HighGEnable == 1)
  {
    LSM6DSV320X_ACC_HG_SetOutputDataRate(MotionCompObj[CUSTOM_LSM6DSV320X_0], HighGODR);
  }
}

#ifdef __cplusplus
}
#endif
