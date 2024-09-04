/**
  ******************************************************************************
  * @file    main.c
  * @author  MEMS Software Solutions Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/**
  * @mainpage Documentation for MotionSP package of X-CUBE-MEMS1 Software for
  * X-NUCLEO-IKS02A1 expansion board
  *
  * @image html st_logo.png
  *
  * <b>Introduction</b>
  *
  * MotionSP software is an add-on for the X-CUBE-MEMS1 software and provides
  * the FFT and  vibration monitoring related functions calculated from accelerometer data.
  * The expansion is built on top of STM32Cube software technology that eases
  * portability across different STM32 microcontrollers.
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "com.h"
#include <string.h> /* strlen */
#include <stdio.h>  /* snprintf */
#include <math.h>   /* trunc */
#include "DemoSerial.h"
#include "MotionSP.h"
#include "sensor_def.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup VIBRATION_MONITORING VIBRATION MONITORING
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ISM330DHCX_HP_DISABLE        0xFBU  /* Disable HP filter */
#define ISM330DHCX_HP_ENABLE_DIV400  0xC4U  /* Enable HP filter, DIV/400 */
#define ISM330DHCX_DEFAULT_ODR       417.0f /* Default output/batch data rate */
#define ISM330DHCX_DEFAULT_FS        2      /* Default full scale */

/* Extern variables ----------------------------------------------------------*/
/* These "redundant" lines are here to fulfil MISRA C-2012 rule 8.4 */
extern volatile uint32_t SensorsEnabled;

volatile uint8_t DataLoggerActive = 0;
extern sAxesMagBuff_t AccAxesAvgMagBuff;
gui_settings_t GuiSettings = {.hp_filter = 0, .switch_HP_to_DC_null = 0};
uint8_t SensorList[NUM_SENSORS + 1];
volatile uint32_t SensorsEnabled = 0;
extern int32_t UseLSI;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static RTC_HandleTypeDef RtcHandle;
static volatile uint32_t IntCurrentTime1 = 0;
static volatile uint32_t IntCurrentTime2 = 0;

static volatile uint8_t AccIntReceived = 0;
static uint8_t RestartFlag = 1;
static uint32_t StartTick = 0;

/* Private function prototypes -----------------------------------------------*/
static void Init_Sensors(void);
static void RTC_Config(void);
static void RTC_TimeStampConfig(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static uint8_t Collect_Data(void);
static uint8_t Meas_Odr(void);
static void HP_DC_Changer(void);
static uint8_t Enable_DRDY(void);
static uint8_t Disable_DRDY(void);
static uint8_t Enable_FIFO(void);
static uint8_t Disable_FIFO(void);
static uint8_t Get_Available_Sensors(void);
static void En_Dis_HP_Or_DCnull(void);

/* Public functions ----------------------------------------------------------*/
/**
 * @brief  Main function is to show how to use X_NUCLEO_IKS02A1
 *         expansion board to calculate FFT from accelerometer data
 *         and send it from a Nucleo board to a connected PC, using UART,
 *         displaying it on Unicleo-GUI.
 *         Graphical User Interface, developed by STMicroelectronics and provided
 *         with X-CUBE-MEMS1 package.
 *         After connection has been established with GUI, the user can visualize
 *         the data.
 *         See User Manual for details.
 * @param  None
 * @retval None
 */
int main(void); /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
int main(void)
{
  Msg_t msg_dat;
  Msg_t msg_cmd;
  ACTIVE_AXIS_t axis_active;

  /* STM32xxxx HAL library initialization:
   *   - Configure the Flash prefetch, instruction and Data caches
   *   - Configure the Systick to generate an interrupt each 1 msec
   *   - Set NVIC Group Priority to 4
   *   - Global MSP (MCU Support Package) initialization
   */
  (void)HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the SysTick IRQ priority - set the second lowest priority */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0x0E, 0);

  /* Initialize GPIOs */
  MX_GPIO_Init();

  /* Initialize CRC */
  MX_CRC_Init();

  /* Initialize (disabled) Sensors */
  Init_Sensors();

  /* Initialize Communication Peripheral for data log */
  USARTConfig();

  /* RTC Initialization */
  RTC_Config();
  RTC_TimeStampConfig();

  BSP_LED_On(LED2);
  HAL_Delay(500);
  BSP_LED_Off(LED2);

  if (Init_Demo() != 0)
  {
    SEND_BOARD_RESTARTED_MSG((Msg_t *)&msg_cmd);
  }
  else
  {
    SEND_INIT_ERR_MSG((Msg_t *)&msg_cmd);
    Error_Handler();
  }

  for (;;)
  {
    if (UART_ReceivedMSG((Msg_t *)&msg_cmd) != 1)
    {
      if (msg_cmd.Data[0] == DEV_ADDR)
      {
        (void)HandleMSG((Msg_t *)&msg_cmd);
      }
    }

    /* Data processing started */
    if (DataLoggerActive)
    {
      BSP_LED_On(LED2);

      if (Collect_Data() != 1)
      {
        Error_Handler();
      }

      /* Perform Frequency Domain analysis if buffer is full */
      if (fftIsEnabled == 1)
      {
        fftIsEnabled = 0;

        if ((HAL_GetTick() - StartTick) >= MotionSP_Parameters.tacq)
        {
          FinishAvgFlag = 1;
          StartTick = HAL_GetTick();
        }

        MotionSP_FrequencyDomainProcess();
      }

      /* Send data to GUI if total acquisition time is reached */
      if (FinishAvgFlag == 1)
      {
        FinishAvgFlag = 0;

        /* Send all 3 axes data to Unicleo-GUI */
        for (axis_active = X_AXIS; axis_active < NUM_AXES; axis_active++)
        {
          INIT_STREAMING_HEADER(&msg_dat);
          Get_Msg(&msg_dat, axis_active);
          UART_SendMsg(&msg_dat);
        }
        RestartFlag = 1;
      }
    }
  }
}

/**
  * @brief  Initialize demo
  * @param  None
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t Init_Demo(void)
{
  uint8_t data;

  /* Disable HP filter if needed */
  if (SensorSetting.hp_filter_available == 1)
  {
    if (IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, &data) != BSP_ERROR_NONE)
    {
      return 0;
    }

    data &= ISM330DHCX_HP_DISABLE;

    if (IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, data) != BSP_ERROR_NONE)
    {
      return 0;
    }
  }

  /* Turn-on time delay */
  HAL_Delay(40);

  if (Enable_DRDY() == 0)
  {
    return 0;
  }

  GuiSettings.switch_HP_to_DC_null = 0;
  GuiSettings.hp_filter = 0;
  fftIsEnabled = 0;

  /* Set parameters for MotionSP library */
  MotionSP_Parameters.FftSize = FFT_SIZE_DEFAULT;
  MotionSP_Parameters.tau = TAU_DEFAULT;
  MotionSP_Parameters.window = WINDOW_DEFAULT;
  MotionSP_Parameters.td_type = TD_DEFAULT;
  MotionSP_Parameters.tacq = TACQ_DEFAULT;

  /* Create circular buffer and initialize result variables */
  AccCircBuffer.Size = MotionSP_Parameters.FftSize;
  AccCircBuffer.IdPos = 0;
  AccCircBuffer.Ovf = 0;

  magSize = MotionSP_Parameters.FftSize / 2;

  /* Reset circular buffer for storing accelerometer values */
  memset(AccCircBuffer.Data.AXIS_X, 0x00, (AccCircBuffer.Size) * (sizeof(float)));
  memset(AccCircBuffer.Data.AXIS_Y, 0x00, (AccCircBuffer.Size) * (sizeof(float)));
  memset(AccCircBuffer.Data.AXIS_Z, 0x00, (AccCircBuffer.Size) * (sizeof(float)));

  /* Reset the TimeDomain parameter values */
  sTimeDomain.AccRms.AXIS_X = 0.0f;
  sTimeDomain.AccRms.AXIS_Y = 0.0f;
  sTimeDomain.AccRms.AXIS_Z = 0.0f;
  sTimeDomain.AccPeak.AXIS_X = 0.0f;
  sTimeDomain.AccPeak.AXIS_Y = 0.0f;
  sTimeDomain.AccPeak.AXIS_Z = 0.0f;
  sTimeDomain.SpeedRms.AXIS_X = 0.0f;
  sTimeDomain.SpeedRms.AXIS_Y = 0.0f;
  sTimeDomain.SpeedRms.AXIS_Z = 0.0f;

  /* Reset the counters of the number of sums about the calculation of the average */
  AccSumCnt.AXIS_X = 0;
  AccSumCnt.AXIS_Y = 0;
  AccSumCnt.AXIS_Z = 0;

  MotionSP_SetWindFiltArray(Filter_Params, MotionSP_Parameters.FftSize, (Filt_Type_t)MotionSP_Parameters.window);

  /* Reset the flag to enable FFT computation */
  fftIsEnabled = 0;

  arm_rfft_fast_init_f32(&fftS, MotionSP_Parameters.FftSize);

  accCircBuffIndexForFft = MotionSP_Parameters.FftSize - 1; /* It is the minimum value to do the first FFT */

  /* Set FIFO BDR (has to be equal to or lower than accelerometer ODR) */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_BDR(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, ISM330DHCX_DEFAULT_ODR) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Set FIFO watermark level */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_Watermark_Level(IKS02A1_ISM330DHCX_0, MotionSP_Parameters.FftSize - 1) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Set FIFO to stop on FIFO threshold */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(IKS02A1_ISM330DHCX_0, PROPERTY_ENABLE) != BSP_ERROR_NONE)
  {
    return 0;
  }

  if (SensorSetting.hp_filter_available == 0)
  {
    HP_DC_Changer();
  }

  /* Measure and calculate ODR */
  if (Meas_Odr() == 0)
  {
    return 0;
  }

  return 1;
}

/**
  * @brief  Restart FIFO
  * @param  None
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t Restart_FIFO(void)
{
  AccIntReceived = 0;

  /* FIFO Bypass Mode */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_Mode(IKS02A1_ISM330DHCX_0, ISM330DHCX_BYPASS_MODE) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* FIFO Mode*/
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_Mode(IKS02A1_ISM330DHCX_0, ISM330DHCX_FIFO_MODE) != BSP_ERROR_NONE)
  {
    return 0;
  }

  return 1;
}

/**
  * @brief  Set accelerometer ODR
  * @param  value the index of ODR to be set
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t Set_ODR(uint8_t value)
{
  if ((value <= SensorSetting.odr_list[0]) && value != 0)
  {
    /* Set accelerometer ODR */
    if (IKS02A1_MOTION_SENSOR_SetOutputDataRate(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, SensorSetting.odr_list[value]) != BSP_ERROR_NONE)
    {
      return 0;
    }

    /* Set FIFO BDR (has to be equal to or lower than accelerometer ODR) */
    if (IKS02A1_MOTION_SENSOR_FIFO_Set_BDR(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, SensorSetting.odr_list[value]) != BSP_ERROR_NONE)
    {
      return 0;
    }

    Meas_Odr();
    fftIsEnabled = 0;

    if (GuiSettings.hp_filter == 1)
    {
      HAL_Delay((uint32_t)(320 / AcceleroODR.Frequency));
      Restart_FIFO();
    }
  }

  return 1;
}

/**
  * @brief  Get HP filter status
  * @param  None
  * @retval HP filter status
  */
uint8_t Get_HP_Filter(void)
{
  return GuiSettings.hp_filter;
}

/**
  * @brief  Get number of FFT samples
  * @param  None
  * @retval FFT samples
  */
uint16_t Get_Samples(void)
{
  return MotionSP_Parameters.FftSize;
}

/**
  * @brief  Get list of available sensors
  * @param  None
  * @retval Sensor list
  */
uint8_t *Get_Sensor_List(void)
{
  Get_Available_Sensors();

  return SensorList;
}

/**
  * @brief  Get message
  * @param  Msg the pointer to the message to be handled
  * @param  AxisActive currently active axis index
  * @retval None
  */
void Get_Msg(Msg_t *Msg, ACTIVE_AXIS_t AxisActive)
{
  uint32_t k = 0;
  float err = -1.0f;
  float temp;

  memcpy(&Msg->Data[3], (void *) &AcceleroODR.Frequency, sizeof(float));
  Msg->Len = 3 + sizeof(float);

  /* Store FFT values to Msg */
  /* Store TimeDomain parameter values to Msg */
  switch (AxisActive)
  {
    case X_AXIS:
      for (k = 0; k < MotionSP_Parameters.FftSize / 2; k++)
      {
        temp = AccAxesAvgMagBuff.AXIS_X[k] * 1000; // * 1000 for Unicleo-GUI
        memcpy(&Msg->Data[Msg->Len + k * sizeof(float)], (void *) &temp, sizeof(float));
      }
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.AccRms.AXIS_X, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.AccPeak.AXIS_X, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.SpeedRms.AXIS_X, sizeof(float));
      break;

    case Y_AXIS:
      for (k = 0; k < MotionSP_Parameters.FftSize / 2; k++)
      {
        temp = AccAxesAvgMagBuff.AXIS_Y[k] * 1000; // * 1000 for Unicleo-GUI
        memcpy(&Msg->Data[Msg->Len + k * sizeof(float)], (void *) &temp, sizeof(float));
      }
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.AccRms.AXIS_Y, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.AccPeak.AXIS_Y, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.SpeedRms.AXIS_Y, sizeof(float));
      break;

    case Z_AXIS:
      for (k = 0; k < MotionSP_Parameters.FftSize / 2; k++)
      {
        temp = AccAxesAvgMagBuff.AXIS_Z[k] * 1000; // * 1000 for Unicleo-GUI
        memcpy(&Msg->Data[Msg->Len + k * sizeof(float)], (void *) &temp, sizeof(float));
      }
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.AccRms.AXIS_Z, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.AccPeak.AXIS_Z, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &sTimeDomain.SpeedRms.AXIS_Z, sizeof(float));
      break;

    default:
      /* Store -1 to Msg to indicate error */
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &err, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &err, sizeof(float));
      memcpy(&Msg->Data[Msg->Len + k++ * sizeof(float)], (void *) &err, sizeof(float));
      break;
  }
  Msg->Len += k * sizeof(float);

  /* Store active axis index to Msg */
  Msg->Data[Msg->Len] = (uint8_t)AxisActive;
  Msg->Len += sizeof(uint8_t);
}

/**
  * @brief  Set full scale of the accelerometer
  * @param  value the index of full scale to be set
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t Set_Full_Scale(uint8_t value)
{
  /* Set chosen AXL Full scale */
  if (value <= SensorSetting.fs_list[0] && value != 0)
  {
    fftIsEnabled = 0;

    if (IKS02A1_MOTION_SENSOR_SetFullScale(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, SensorSetting.fs_list[value]) != BSP_ERROR_NONE)
    {
      return 0;
    }

    if (!Restart_FIFO())
    {
      return 0;
    }
  }

  return 1;
}

/**
  * @brief  Set number of FFT samples
  * @param  value the number of FFT samples to be set
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t Set_Samples(uint8_t value)
{
  /* Set chosen FFT Samples and calculate values for graph x axis  */
  if (value <= SensorSetting.samples_list[0] && value != 0)
  {
    fftIsEnabled = 0;
    MotionSP_Parameters.FftSize = SensorSetting.samples_list[value];
    AccCircBuffer.Size = MotionSP_Parameters.FftSize;
    magSize = MotionSP_Parameters.FftSize / 2;
    MotionSP_SetWindFiltArray(Filter_Params, MotionSP_Parameters.FftSize, (Filt_Type_t)MotionSP_Parameters.window);
    accCircBuffIndexForFft = MotionSP_Parameters.FftSize - 1; // it is the minimum value to do the first FFT
    arm_rfft_fast_init_f32(&fftS, MotionSP_Parameters.FftSize);

    /* Set FIFO watermark level */
    if (IKS02A1_MOTION_SENSOR_FIFO_Set_Watermark_Level(IKS02A1_ISM330DHCX_0, MotionSP_Parameters.FftSize - 1) != BSP_ERROR_NONE)
    {
      return 0;
    }

    if (!Restart_FIFO())
    {
      return 0;
    }
  }

  return 1;
}

/**
  * @brief  Set HP filter
  * @param  value the status of HP filter
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t Set_HP_Filter(uint8_t value)
{
  if (!SensorSetting.hp_filter_available)
  {
    return 2;
  }

  if (value != GuiSettings.hp_filter)
  {
    En_Dis_HP_Or_DCnull();
  }

  /* check if the variable GuiSettings.hp_filter is properly changed */
  if (value != GuiSettings.hp_filter)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Initialize all sensors
 * @param  None
 * @retval None
 */
static void Init_Sensors(void)
{
  (void)IKS02A1_MOTION_SENSOR_Init(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO);

  /* Set accelerometer:
   *   - ODR >= 417Hz
   *   - FS   = <-2g, 2g>
   */
  (void)IKS02A1_MOTION_SENSOR_SetOutputDataRate(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, ISM330DHCX_DEFAULT_ODR);
  (void)IKS02A1_MOTION_SENSOR_SetFullScale(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, ISM330DHCX_DEFAULT_FS);
}

/**
 * @brief  GPIO init function.
 * @param  None
 * @retval None
 * @details GPIOs initialized are User LED(PA5) and User Push Button(PC1)
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructureInt1 = {0};

  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize push button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructureInt1.Pin = GPIO_PIN_10;
  GPIO_InitStructureInt1.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructureInt1.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructureInt1.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructureInt1);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief  CRC init function.
 * @param  None
 * @retval None
 */
static void MX_CRC_Init(void)
{
  __CRC_CLK_ENABLE();
}

/**
  * @brief  Collect data from accelerometer
  * @param  None
  * @retval 1 in case of success
  * @retval 0 otherwise
  */
static uint8_t Collect_Data(void)
{
  uint8_t tag;
  uint16_t samples_in_fifo = 0;
  uint32_t start = HAL_GetTick();
  IKS02A1_MOTION_SENSOR_Axes_t acceleration;
  SensorVal_f_t single_data;
  SensorVal_f_t single_data_no_dc;

  if (FinishAvgFlag == 0 && fftIsEnabled == 0 && AccIntReceived == 1)
  {
    AccIntReceived = 0;

    IKS02A1_MOTION_SENSOR_FIFO_Get_Num_Samples(IKS02A1_ISM330DHCX_0, &samples_in_fifo);

    if (samples_in_fifo < MotionSP_Parameters.FftSize - 2)
    {
      Restart_FIFO();
      return 0;
    }

    while (fftIsEnabled == 0)
    {
      if (((HAL_GetTick() - start) > 6000))
      {
        Restart_FIFO();
        return 0;
      }

      IKS02A1_MOTION_SENSOR_FIFO_Get_Tag(IKS02A1_ISM330DHCX_0, &tag);

      if (tag == (uint8_t)ISM330DHCX_XL_NC_TAG)
      {
        IKS02A1_MOTION_SENSOR_FIFO_Get_Axes(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, &acceleration);
      }

      /* Store data */
      single_data.AXIS_X = (float)acceleration.x;
      single_data.AXIS_Y = (float)acceleration.y;
      single_data.AXIS_Z = (float)acceleration.z;

      /* Remove DC offset */
      MotionSP_accDelOffset(&single_data_no_dc, &single_data, DC_SMOOTH, RestartFlag);

      /* Fill the accelero circular buffer */
      MotionSP_CreateAccCircBuffer(&AccCircBuffer, single_data_no_dc);

      if (AccCircBuffer.Ovf == 1)
      {
        fftIsEnabled = 1;
        AccCircBuffer.Ovf = 0;
      }

      MotionSP_TimeDomainProcess(&sTimeDomain, (Td_Type_t)MotionSP_Parameters.td_type, RestartFlag);
      RestartFlag = 0;
    }

    if (!Restart_FIFO())
    {
      return 0;
    }
  }

  return 1;
}

/**
 * @brief  Measure accelerometer real ODR
 * @param  None
 * @retval 1 in case of success 0 otherwise
 */
static uint8_t Meas_Odr(void)
{
  uint8_t  odr_meas_enable = 1;
  uint16_t odr_meas_iter = 0;
  uint16_t odr_meas_start_time = 0;
  uint16_t odr_meas_stop_time = 0;
  uint16_t odr_meas_samples = 150; /* number of measured samples for calculating ODR */
  uint32_t start = 0;

  if (!Disable_FIFO())
  {
    return 0;
  }

  /* Set DRDY pulsed mode */
  if (IKS02A1_MOTION_SENSOR_DRDY_Set_Mode(IKS02A1_ISM330DHCX_0, ISM330DHCX_DRDY_PULSED) != BSP_ERROR_NONE)
  {
    return 0;
  }

  if (!Enable_DRDY())
  {
    return 0;
  }

  start = HAL_GetTick();

  while (odr_meas_enable)
  {
    if (((HAL_GetTick() - start) > 1000))
    {
      return 0;
    }

    if (AccIntReceived)
    {
      AccIntReceived = 0;

      /* Get start time */
      if (odr_meas_iter == 0)
      {
        IntCurrentTime1 = HAL_GetTick();
        odr_meas_start_time = IntCurrentTime1;
      }

      /* Get stop time */
      if (odr_meas_iter == odr_meas_samples - 1)
      {
        IntCurrentTime2 = HAL_GetTick();
        odr_meas_stop_time = IntCurrentTime2;
        odr_meas_enable = 0;
      }

      /* Stop after measuring "odr_meas_samples" values */
      if (odr_meas_iter < odr_meas_samples)
      {
        odr_meas_iter++;
      }
    }
  }

  /* Calculate measured ODR */
  AcceleroODR.Frequency = ((float)(1000 * odr_meas_samples) / (odr_meas_stop_time - odr_meas_start_time));

  if (AcceleroODR.Frequency != 0)
  {
    AcceleroODR.Period = 1 / AcceleroODR.Frequency;
  }

  AcceleroODR.Tau = exp(-(float)(1000 * AcceleroODR.Period) / MotionSP_Parameters.tau);

  /* Disable accelerometer to avoid interrupt conflicts on highest ODRs */
  if (IKS02A1_MOTION_SENSOR_Disable(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO) != BSP_ERROR_NONE)
  {
    return 0;
  }

  if (!Disable_DRDY())
  {
    return 0;
  }

  /* Set DRDY latched mode */
  if (IKS02A1_MOTION_SENSOR_DRDY_Set_Mode(IKS02A1_ISM330DHCX_0, ISM330DHCX_DRDY_LATCHED) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Enable accelerometer */
  if (IKS02A1_MOTION_SENSOR_Enable(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Enable FIFO full flag interrupt */
  if (!Enable_FIFO())
  {
    return 0;
  }

  if (!Restart_FIFO())
  {
    return 0;
  }

  return 1;
}

/**
 * @brief  Changes HP to DCnull and vice versa in main menu options
 * @param  None
 * @retval void
 */
static void HP_DC_Changer(void)
{
  uint8_t ret_err = 0;
  uint8_t data;

  if (GuiSettings.switch_HP_to_DC_null)
  {
    GuiSettings.switch_HP_to_DC_null = 0;
    GuiSettings.hp_filter = 0;
  }
  else
  {
    /* Disable HP filter */
    if (SensorSetting.hp_filter_available == 1)
    {
      if (IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, &data) != BSP_ERROR_NONE)
      {
        ret_err = 1;
      }

      data &= ISM330DHCX_HP_DISABLE;

      if (IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, data) != BSP_ERROR_NONE)
      {
        ret_err = 1;
      }
    }

    if (ret_err == 0)
    {
      GuiSettings.switch_HP_to_DC_null = 1;
      GuiSettings.hp_filter = 0;
      fftIsEnabled = 0;
    }
  }
}

/**
  * @brief  Enable DRDY
  * @param  None
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
static uint8_t Enable_DRDY(void)
{
  IKS02A1_MOTION_SENSOR_AxesRaw_t axes;

  AccIntReceived = 0;

  /* Enable DRDY */
  if (IKS02A1_MOTION_SENSOR_DRDY_Enable_Interrupt(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, IKS02A1_MOTION_SENSOR_INT1_PIN) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Clear DRDY */
  if (IKS02A1_MOTION_SENSOR_GetAxesRaw(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, &axes) != BSP_ERROR_NONE)
  {
    return 0;
  }

  return 1;
}

/**
  * @brief  Disable DRDY
  * @param  None
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
static uint8_t Disable_DRDY(void)
{
  /* Disable DRDY */
  if (IKS02A1_MOTION_SENSOR_DRDY_Disable_Interrupt(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, IKS02A1_MOTION_SENSOR_INT1_PIN) != BSP_ERROR_NONE)
  {
    return 0;
  }

  return 1;
}

/**
  * @brief  Enable FIFO measuring
  * @param  None
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
static uint8_t Enable_FIFO(void)
{
  /* Enable FIFO full flag interrupt */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(IKS02A1_ISM330DHCX_0, PROPERTY_ENABLE) != BSP_ERROR_NONE)
  {
    return 0;
  }

  return 1;
}

/**
  * @brief  Disable FIFO measuring
  * @param  None
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
static uint8_t Disable_FIFO(void)
{
  /* Set FIFO to bypass mode */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_Mode(IKS02A1_ISM330DHCX_0, ISM330DHCX_BYPASS_MODE) != BSP_ERROR_NONE)
  {
    return 0;
  }

  /* Disable FIFO full flag interrupt */
  if (IKS02A1_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(IKS02A1_ISM330DHCX_0, PROPERTY_DISABLE) != BSP_ERROR_NONE)
  {
    return 0;
  }

  return 1;
}

/**
  * @brief  Find available accelerometers that can be used for FFT demo
  * @param  None
  * @retval Number of found sensors
  */
static uint8_t Get_Available_Sensors(void)
{
  uint8_t sensors_available = 1;

  /* NOTE: Currently done for just one sensor but possible to add mechanism for available sensors detection here */

  SensorList[sensors_available] = IKS02A1_ISM330DHCX_0;

  SensorList[0] = sensors_available;

  return SensorList[0];
}

/**
  * @brief  Enable/Disable HP or DCnull
  * @param  void
  * @retval void
  */
static void En_Dis_HP_Or_DCnull(void)
{
  uint8_t ret_err = 0;
  uint8_t data;

  if (GuiSettings.switch_HP_to_DC_null)
  {
    if (GuiSettings.hp_filter)
    {
      GuiSettings.hp_filter = 0;
    }
    else
    {
      GuiSettings.hp_filter = 1;
    }

    fftIsEnabled = 0;
  }
  else
  {
    if (GuiSettings.hp_filter)
    {
      /* Disable HP filter */
      if (IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, &data) != BSP_ERROR_NONE)
      {
        ret_err = 1;
      }

      data &= ISM330DHCX_HP_DISABLE;

      if (IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, data) != BSP_ERROR_NONE)
      {
        ret_err = 1;
      }

      if (ret_err == 0)
      {
        GuiSettings.hp_filter = 0;
        fftIsEnabled = 0;
      }
    }
    else
    {
      /* Enable HP filter */
      if (IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, &data) != BSP_ERROR_NONE)
      {
        ret_err = 1;
      }

      data |= ISM330DHCX_HP_ENABLE_DIV400;

      if (IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_CTRL8_XL, data) != BSP_ERROR_NONE)
      {
        ret_err = 1;
      }

      if (ret_err == 0)
      {
        GuiSettings.hp_filter = 1;
        fftIsEnabled = 0;
      }
    }
    HAL_Delay(40);
    Restart_FIFO();
  }
}

/**
 * @brief  Configures the RTC
 * @param  None
 * @retval None
 */
static void RTC_Config(void)
{
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Check if LSE can be used */
  RCC_OscInitTypeDef rcc_osc_init_struct;

  /*##-2- Configure LSE as RTC clock source ##################################*/
  rcc_osc_init_struct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  rcc_osc_init_struct.PLL.PLLState   = RCC_PLL_NONE;
  rcc_osc_init_struct.LSEState       = RCC_LSE_ON;
  rcc_osc_init_struct.LSIState       = RCC_LSI_OFF;

  if (HAL_RCC_OscConfig(&rcc_osc_init_struct) != HAL_OK)
  {
    /* LSE not available, we use LSI */
    UseLSI = 1;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV_LSI;
    RtcHandle.Init.SynchPrediv  = RTC_SYNCH_PREDIV_LSI;
  }
  else
  {
    /* We use LSE */
    UseLSI = 0;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV_LSE;
    RtcHandle.Init.SynchPrediv  = RTC_SYNCH_PREDIV_LSE;
  }

  RtcHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
       - Hour Format    = Format 12
       - Asynch Prediv  = Value according to source clock
       - Synch Prediv   = Value according to source clock
       - OutPut         = Output Disable
       - OutPutPolarity = High Polarity
       - OutPutType     = Open Drain
   */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
 * @brief  Configures the current time and date
 * @param  None
 * @retval None
 */
static void RTC_TimeStampConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /* Configure the Date */
  /* Set Date: Monday January 1st 2001 */
  sdatestructure.Year    = 0x01;
  sdatestructure.Month   = RTC_MONTH_JANUARY;
  sdatestructure.Date    = 0x01;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;

  if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configure the Time */
  /* Set Time: 00:00:00 */
  stimestructure.Hours          = 0x00;
  stimestructure.Minutes        = 0x00;
  stimestructure.Seconds        = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
 * @brief  Configures the current date
 * @param  y the year value to be set
 * @param  m the month value to be set
 * @param  d the day value to be set
 * @param  dw the day-week value to be set
 * @retval None
 */
void RTC_DateRegulate(uint8_t y, uint8_t m, uint8_t d, uint8_t dw)
{
  RTC_DateTypeDef sdatestructure;

  sdatestructure.Year    = y;
  sdatestructure.Month   = m;
  sdatestructure.Date    = d;
  sdatestructure.WeekDay = dw;

  if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
 * @brief  Configures the current time
 * @param  hh the hour value to be set
 * @param  mm the minute value to be set
 * @param  ss the second value to be set
 * @retval None
 */
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss)
{
  RTC_TimeTypeDef stimestructure;

  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.Hours          = hh;
  stimestructure.Minutes        = mm;
  stimestructure.Seconds        = ss;
  stimestructure.SubSeconds     = 0;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
 * @brief  This function is executed in case of error occurrence
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
  for (;;)
  {
    BSP_LED_On(LED2);
    HAL_Delay(100);
    BSP_LED_Off(LED2);
    HAL_Delay(100);
  }
}

/**
 * @brief  EXTI line detection callbacks
 * @param  GpioPin the pin connected to EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GpioPin)
{
  if (GpioPin == KEY_BUTTON_PIN)
  {
    if (BSP_PB_GetState(BUTTON_KEY) == (uint32_t)GPIO_PIN_RESET)
    {
      /* NOTE: Possible to add functionality here evoked by button pressing */
    }
  }

  if (GpioPin == GPIO_PIN_10)
  {
    AccIntReceived = 1;
  }
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred
 * @param  file pointer to the source file name
 * @param  line assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  for (;;)
  {}
}
#endif

/**
 * @}
 */

/**
 * @}
 */
