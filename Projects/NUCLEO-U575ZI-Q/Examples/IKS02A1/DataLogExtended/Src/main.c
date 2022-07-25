/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include "main.h"
#include "com.h"
#include "demo_serial.h"
#include "sensor_commands.h"

/** @addtogroup X_NUCLEO_IKS02A1_Examples X_NUCLEO_IKS02A1 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
typedef struct displayFloatToInt_s
{
  int8_t sign; /* 0 means positive, 1 means negative*/
  uint32_t out_int;
  uint32_t out_dec;
} displayFloatToInt_t;

/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE 256

/* Private macro -------------------------------------------------------------*/
#define DELAY(x)  for (__IO uint32_t counter = (x); counter > 0U; counter--) {}

/* Public variables ----------------------------------------------------------*/
int UseLSI = 0;
uint8_t UpdateInterrupt = 0;
uint32_t SensorsEnabled = 0;
uint32_t StartTime = 0;

/* Private variables ---------------------------------------------------------*/
static char DataOut[MAX_BUF_SIZE];
static CRC_HandleTypeDef CrcHandle;
static RTC_HandleTypeDef RtcHandle;
static uint32_t PreviousSensorsEnabled = 0;
static volatile uint8_t AutoInit = 0;
static volatile uint32_t IntCurrentTime1 = 0;
static volatile uint32_t IntCurrentTime2 = 0;
static uint8_t NewData = 0;
static uint16_t NewDataFlags = 0;
static uint8_t MsgIndex = 0;
static uint32_t LastUpdate = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_CRC_Init(void);
static void MX_RTC_Init(void);

static void Enable_Disable_Sensors(void);

static void Time_Handler(TMsg *Msg);
static void Accelero_Sensor_Handler(TMsg *Msg, uint32_t Instance);
static void Gyro_Sensor_Handler(TMsg *Msg, uint32_t Instance);
static void Magneto_Sensor_Handler(TMsg *Msg, uint32_t Instance);
static void Sensors_Interrupt_Handler(TMsg *Msg);
static void MLC_Handler(TMsg *Msg);
static void FSM_Handler(TMsg *Msg);

static void DIL24_INT1_Force_Low(void);
static uint32_t DWT_Delay_Init(void);

/* Public functions ----------------------------------------------------------*/
/**
 * @brief  Main function is to show how to use sensor expansion board to send data from a Nucleo board
 *         using UART to a connected PC or Desktop and display it on generic applications like
 *         TeraTerm and specific application Unicleo-GUI, which is developed by STMicroelectronics
 *         and provided with a separated package.
 *
 *         After connection has been established:
 *         - the user can view the data from various on-board environment sensors like Temperature, Humidity and Pressure
 *         - the user can also view data from various on-board MEMS sensors as well like Accelerometer, Gyroscope and Magnetometer
 *         - the user can also visualize this data as graphs using Unicleo-GUI application
 * @param  None
 * @retval Integer
 */
int main(void)
{
  TMsg msg_dat;
  TMsg msg_cmd;

  /* STM32F4xx HAL library initialization:
  - Configure the Flash prefetch, instruction and Data caches
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  (void)HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the System Power */
  SystemPower_Config();

  /* Pull the INT1 pin low to avoid enabling I3C */
  DIL24_INT1_Force_Low();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_CRC_Init();
  MX_ICACHE_Init();
  MX_RTC_Init();

  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  /* Initialize cycle counter */
  DWT_Delay_Init();

  /* Blink with LED */
  BSP_LED_On(LED2);
  HAL_Delay(100);
  BSP_LED_Off(LED2);

  /* Start receiving messages via DMA */
  UART_StartReceiveMsg();

  for (;;)
  {
    /* Process incoming messages */
    if (UART_ReceivedMSG((TMsg *)&msg_cmd) == 1)
    {
      if (msg_cmd.Data[0] == DEV_ADDR)
      {
        (void)HandleMSG((TMsg *)&msg_cmd);
        if (DataLoggerActive != 0U)
        {
          AutoInit = 0;
        }
      }
    }

    /* Enable/Disable sensors */
    if (PreviousSensorsEnabled != SensorsEnabled)
    {
      PreviousSensorsEnabled = SensorsEnabled;
      Enable_Disable_Sensors();
    }

    /* For terminal output enable all sensors */
    if (AutoInit == 1U)
    {
      SensorsEnabled = 0xFFFFFFFF;
    }

    /* LED control */
    if (DataLoggerActive != 0U)
    {
      BSP_LED_Toggle(LED2);
    }
    else if (AutoInit != 0U)
    {
      BSP_LED_Toggle(LED2);
    }
    else
    {
      BSP_LED_Off(LED2);
    }

    /* Read data from enable sensors and prepare data stream */
    if (DataLoggerActive != 0U)
    {
      Time_Handler(&msg_dat);

      if ((SensorsEnabled & ACCELEROMETER_SENSOR_ENABLED) == ACCELEROMETER_SENSOR_ENABLED)
      {
        Accelero_Sensor_Handler(&msg_dat, AccInstance);
      }

      if ((SensorsEnabled & GYROSCOPE_SENSOR_ENABLED) == GYROSCOPE_SENSOR_ENABLED)
      {
        Gyro_Sensor_Handler(&msg_dat, GyrInstance);
      }

      if ((SensorsEnabled & MAGNETIC_SENSOR_ENABLED) == MAGNETIC_SENSOR_ENABLED)
      {
        Magneto_Sensor_Handler(&msg_dat, MagInstance);
      }

      if ((SensorsEnabled & INTERRUPTS_ENABLED) == INTERRUPTS_ENABLED)
      {
        Sensors_Interrupt_Handler(&msg_dat);
      }

      if ((SensorsEnabled & MLC_ENABLED) == MLC_ENABLED)
      {
        MLC_Handler(&msg_dat);
      }

      if ((SensorsEnabled & FSM_ENABLED) == FSM_ENABLED)
      {
        FSM_Handler(&msg_dat);
      }

      /* Send data stream in a new data are available */
      if (NewData != 0U)
      {
        INIT_STREAMING_HEADER(&msg_dat);
        (void)memcpy(&msg_dat.Data[7], (void *)&NewDataFlags, 2);
        msg_dat.Len = MsgIndex;
        UART_SendMsg(&msg_dat);
        NewData = 0;
        NewDataFlags = 0;
      }
    }

    /* For terminal output reduce speed */
    if (AutoInit != 0U)
    {
      HAL_Delay(500);
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Interrupt pins X-NUCLEO-IKS02A1 */
  // ISM330DHCX INT1                     PF14
  // ISM330DHCX INT2                     PE11
  // IIS2DLPC   INT2                     PE9
  // DIL24 INT1                          PC0 (initialized separately)
  // IIS2MDC DRDY/IIS2DLPC INT           PC3
  // IIS2DLPC INT/IIS2MDC DRDY           PB0
  // USER INT                            PF15

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure GPIO pins : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure GPIO pin : PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure GPIO pin : PE9 PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* Configure GPIO pin : PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{
  HAL_PWREx_EnableVddIO2();

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{
  CrcHandle.Instance = CRC;
  CrcHandle.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  CrcHandle.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  CrcHandle.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  CrcHandle.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPDMA1_Init(void)
{
  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
  HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
  HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{
  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  Enable/disable desired sensors
 * @param  None
 * @retval None
 */
static void Enable_Disable_Sensors(void)
{
  if ((SensorsEnabled & ACCELEROMETER_SENSOR_ENABLED) == ACCELEROMETER_SENSOR_ENABLED)
  {
    (void)IKS02A1_MOTION_SENSOR_Enable(AccInstance, MOTION_ACCELERO);
  }
  else
  {
    (void)IKS02A1_MOTION_SENSOR_Disable(AccInstance, MOTION_ACCELERO);
  }

  if ((SensorsEnabled & GYROSCOPE_SENSOR_ENABLED) == GYROSCOPE_SENSOR_ENABLED)
  {
    (void)IKS02A1_MOTION_SENSOR_Enable(GyrInstance, MOTION_GYRO);
  }
  else
  {
    (void)IKS02A1_MOTION_SENSOR_Disable(GyrInstance, MOTION_GYRO);
  }

  if ((SensorsEnabled & MAGNETIC_SENSOR_ENABLED) == MAGNETIC_SENSOR_ENABLED)
  {
    (void)IKS02A1_MOTION_SENSOR_Enable(MagInstance, MOTION_MAGNETO);
  }
  else
  {
    (void)IKS02A1_MOTION_SENSOR_Disable(MagInstance, MOTION_MAGNETO);
  }
}

/**
 * @brief  Handles the precise time
 * @param  Msg the time part of the stream
 * @retval None
 */
static void Time_Handler(TMsg *Msg)
{
  uint32_t time_us;

  if (DataLoggerActive != 0U)
  {
    time_us = DWT_GetTickUS() - StartTime;
    Serialize(&Msg->Data[3], time_us, 4);
    MsgIndex = 9;
  }
  else if (AutoInit != 0U)
  {
    time_us = DWT_GetTickUS() - StartTime;
    (void)snprintf(DataOut, MAX_BUF_SIZE, "TimeStamp: %lu\r\n", (unsigned long)time_us);
    (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
  }
  else
  {
    /* Nothing to do */
  }
}

/**
 * @brief  Handles the ACCELERO axes data getting/sending
 * @param  Msg the ACCELERO part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Accelero_Sensor_Handler(TMsg *Msg, uint32_t Instance)
{
  int32_t data[6];
  IKS02A1_MOTION_SENSOR_Axes_t acceleration;
  uint8_t status = 0;

  if (IKS02A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &status) == BSP_ERROR_NONE && status == 1U)
  {
    NewData++;
    NewDataFlags |= 8U;

    (void)IKS02A1_MOTION_SENSOR_GetAxes(Instance, MOTION_ACCELERO, &acceleration);

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], acceleration.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], acceleration.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], acceleration.z, 4);
      MsgIndex = MsgIndex + 12;
    }
    else if (AutoInit != 0U)
    {
      data[0] = acceleration.x;
      data[1] = acceleration.y;
      data[2] = acceleration.z;

      (void)snprintf(DataOut, MAX_BUF_SIZE, "ACC_X: %d, ACC_Y: %d, ACC_Z: %d\r\n", (int)data[0], (int)data[1], (int)data[2]);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the GYRO axes data getting/sending
 * @param  Msg the GYRO part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Gyro_Sensor_Handler(TMsg *Msg, uint32_t Instance)
{
  int32_t data[6];
  IKS02A1_MOTION_SENSOR_Axes_t angular_velocity;
  uint8_t status = 0;

  if (IKS02A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_GYRO, &status) == BSP_ERROR_NONE && status == 1U)
  {
    NewData++;
    NewDataFlags |= 16U;

    (void)IKS02A1_MOTION_SENSOR_GetAxes(Instance, MOTION_GYRO, &angular_velocity);

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], angular_velocity.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], angular_velocity.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], angular_velocity.z, 4);
      MsgIndex = MsgIndex + 12;
    }
    else if (AutoInit != 0U)
    {
      data[0] = angular_velocity.x;
      data[1] = angular_velocity.y;
      data[2] = angular_velocity.z;

      (void)snprintf(DataOut, MAX_BUF_SIZE, "GYR_X: %d, GYR_Y: %d, GYR_Z: %d\r\n", (int)data[0], (int)data[1], (int)data[2]);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the MAGNETO axes data getting/sending
 * @param  Msg the MAGNETO part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Magneto_Sensor_Handler(TMsg *Msg, uint32_t Instance)
{
  int32_t data[3];
  IKS02A1_MOTION_SENSOR_Axes_t magnetic_field;
  uint8_t status = 0;

  if (IKS02A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_MAGNETO, &status) == BSP_ERROR_NONE && status == 1U)
  {
    NewData++;
    NewDataFlags |= 32U;

    (void)IKS02A1_MOTION_SENSOR_GetAxes(Instance, MOTION_MAGNETO, &magnetic_field);

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], (int32_t)magnetic_field.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], (int32_t)magnetic_field.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], (int32_t)magnetic_field.z, 4);
      MsgIndex = MsgIndex + 12;
    }
    else if (AutoInit != 0U)
    {
      data[0] = magnetic_field.x;
      data[1] = magnetic_field.y;
      data[2] = magnetic_field.z;

      (void)snprintf(DataOut, MAX_BUF_SIZE, "MAG_X: %d, MAG_Y: %d, MAG_Z: %d\r\n", (int)data[0], (int)data[1], (int)data[2]);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the sensors interrupts
 * @param  Msg the INTERRUPT part of the stream
 * @retval None
 */
static void Sensors_Interrupt_Handler(TMsg *Msg)
{
  uint8_t int_status = 0;
  static uint8_t mem_int_status = 0;

  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) int_status |= (1 << 0); else int_status &= ~(1 << 0);
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET) int_status |= (1 << 1); else int_status &= ~(1 << 1);
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) int_status |= (1 << 2); else int_status &= ~(1 << 2);
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET) int_status |= (1 << 3); else int_status &= ~(1 << 3);
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_SET) int_status |= (1 << 4); else int_status &= ~(1 << 4);
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET) int_status |= (1 << 5); else int_status &= ~(1 << 5);
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET) int_status |= (1 << 6); else int_status &= ~(1 << 6);
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) int_status |= (1 << 7); else int_status &= ~(1 << 7);

  if ((mem_int_status != int_status) || (UpdateInterrupt == 1) || ((HAL_GetTick() - LastUpdate) > 100))
  {
    UpdateInterrupt = 0;
    NewData++;
    NewDataFlags |= 64U;
    Msg->Data[MsgIndex] = int_status;
    MsgIndex = MsgIndex + 1;

    mem_int_status = int_status;
    LastUpdate = HAL_GetTick();
  }
}

/**
 * @brief  Handles the MLC status data
 * @param  Msg the MLC part of the stream
 * @retval None
 */
static void MLC_Handler(TMsg *Msg)
{
  int32_t i = 0;
  uint8_t new_status = 0;
  uint8_t mlc_status[8];
  static uint8_t mem_mlc_status[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  if ((AccInstance == IKS02A1_ISM330DHCX_0) && (GyrInstance == IKS02A1_ISM330DHCX_0))
  {
    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FUNC_CFG_ACCESS, ISM330DHCX_EMBEDDED_FUNC_BANK << 6);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC0_SRC, &mlc_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC1_SRC, &mlc_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC2_SRC, &mlc_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC3_SRC, &mlc_status[3]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC4_SRC, &mlc_status[4]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC5_SRC, &mlc_status[5]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC6_SRC, &mlc_status[6]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_MLC7_SRC, &mlc_status[7]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FUNC_CFG_ACCESS, ISM330DHCX_USER_BANK << 6);
  }
  else if (AccInstance == IKS02A1_IIS2ICLX_0)
  {
    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FUNC_CFG_ACCESS, IIS2ICLX_EMBEDDED_FUNC_BANK << 6);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC0_SRC, &mlc_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC1_SRC, &mlc_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC2_SRC, &mlc_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC3_SRC, &mlc_status[3]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC4_SRC, &mlc_status[4]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC5_SRC, &mlc_status[5]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC6_SRC, &mlc_status[6]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_MLC7_SRC, &mlc_status[7]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FUNC_CFG_ACCESS, IIS2ICLX_USER_BANK << 6);
  }
  else
  {
    mlc_status[0] = 0;
    mlc_status[1] = 0;
    mlc_status[2] = 0;
    mlc_status[3] = 0;
    mlc_status[4] = 0;
    mlc_status[5] = 0;
    mlc_status[6] = 0;
    mlc_status[7] = 0;
  }

  for (i = 0; i < 8; i++)
  {
    if (mlc_status[i] != mem_mlc_status[i])
    {
      new_status = 1;
      break;
    }
  }

  if (new_status == 1)
  {
    memcpy((void *)&Msg->Data[MsgIndex], (void *)&mlc_status, 8);
    memcpy((void *)&mem_mlc_status, (void *)mlc_status, 8);
    MsgIndex = MsgIndex + 8;

    NewData++;
    NewDataFlags |= 128U;
  }
}

/**
 * @brief  Handles the FSM status data
 * @param  Msg the FSM part of the stream
 * @retval None
 */
static void FSM_Handler(TMsg *Msg)
{
  int32_t i = 0;
  uint8_t new_status = 0;
  uint8_t fsm_status[16];
  static uint8_t mem_fsm_status[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  if ((AccInstance == IKS02A1_ISM330DHCX_0) && (GyrInstance == IKS02A1_ISM330DHCX_0))
  {
    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FUNC_CFG_ACCESS, ISM330DHCX_EMBEDDED_FUNC_BANK << 6);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS8, &fsm_status[7]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS9, &fsm_status[8]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS10, &fsm_status[9]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS11, &fsm_status[10]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS12, &fsm_status[11]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS13, &fsm_status[12]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS14, &fsm_status[13]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS15, &fsm_status[14]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS16, &fsm_status[15]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FUNC_CFG_ACCESS, ISM330DHCX_USER_BANK << 6);
  }
  else if (AccInstance == IKS02A1_IIS2ICLX_0)
  {
    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FUNC_CFG_ACCESS, IIS2ICLX_EMBEDDED_FUNC_BANK << 6);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS8, &fsm_status[7]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS9, &fsm_status[8]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS10, &fsm_status[9]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS11, &fsm_status[10]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS12, &fsm_status[11]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS13, &fsm_status[12]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS14, &fsm_status[13]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS15, &fsm_status[14]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS16, &fsm_status[15]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FUNC_CFG_ACCESS, IIS2ICLX_USER_BANK << 6);
  }
  else
  {
    fsm_status[0] = 0;
    fsm_status[1] = 0;
    fsm_status[2] = 0;
    fsm_status[3] = 0;
    fsm_status[4] = 0;
    fsm_status[5] = 0;
    fsm_status[6] = 0;
    fsm_status[7] = 0;
    fsm_status[8] = 0;
    fsm_status[9] = 0;
    fsm_status[10] = 0;
    fsm_status[11] = 0;
    fsm_status[12] = 0;
    fsm_status[13] = 0;
    fsm_status[14] = 0;
    fsm_status[15] = 0;
  }

  for (i = 0; i < 8; i++)
  {
    if (fsm_status[i] != mem_fsm_status[i])
    {
      new_status = 1;
      break;
    }
  }

  if (new_status == 1)
  {
    memcpy((void *)&Msg->Data[MsgIndex], (void *)&fsm_status, 16);
    memcpy((void *)&mem_fsm_status, (void *)fsm_status, 16);
    MsgIndex = MsgIndex + 16;

    NewData++;
    NewDataFlags |= 256U;
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{
  RTC_PrivilegeStateTypeDef privilegeState = {0};
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only
  */
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = 127;
  RtcHandle.Init.SynchPrediv = 255;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  RtcHandle.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  RtcHandle.Init.BinMode = RTC_BINARY_NONE;
  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
  privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
  privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
  privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
  if (HAL_RTCEx_PrivilegeModeSet(&RtcHandle, &privilegeState) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&RtcHandle, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&RtcHandle, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  EXTI line detection callbacks
  * @param  GPIOPin the pin connected to EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIOPin)
{
  int do_operation = 0;
  uint32_t int_current_time2_local;

  if (GPIOPin == KEY_BUTTON_PIN)
  {
    int_current_time2_local = IntCurrentTime2;

    /* Manage software debouncing*/
    if (IntCurrentTime1 == 0U && int_current_time2_local == 0U)
    {
      IntCurrentTime1 = HAL_GetTick();
      do_operation = 1;
    }
    else
    {
      IntCurrentTime2 = HAL_GetTick();
      int_current_time2_local = IntCurrentTime2;

      /* If I receive a button interrupt after more than 300 ms from the first one I get it, otherwise I discard it */
      if ((int_current_time2_local - IntCurrentTime1) > 300U)
      {
        IntCurrentTime1 = IntCurrentTime2;
        do_operation = 1;
      }
    }

    if (do_operation != 0)
    {
      if (DataLoggerActive != 0U)
      {
        AutoInit = 0; /* always off */
      }
      else
      {
        AutoInit = (AutoInit != 0U) ? 0U : 1U; /* toggle on each button pressed */
      }
    }
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

  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.Hours      = hh;
  stimestructure.Minutes    = mm;
  stimestructure.Seconds    = ss;
  stimestructure.SubSeconds = 0;
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
  if (AutoInit != 0U)
  {
    (void)snprintf(DataOut, MAX_BUF_SIZE, "Error");
    (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
  }

  for (;;)
  {
    BSP_LED_On(LED2);
    HAL_Delay(100);
    BSP_LED_Off(LED2);
    HAL_Delay(100);
  }
}

/**
 * @brief  Force DIL24 INT1 pin low
 * @param  None
 * @retval None
 */
static void DIL24_INT1_Force_Low(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  BSP_I2C1_Init();
}

/**
 * @brief  Initializes DWT_Clock_Cycle_Count for DWT_GetTickUS function
 * @retval Error DWT counter (1: clock cycle counter not started, 0: clock cycle counter works)
 */
static uint32_t DWT_Delay_Init(void)
{
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;

  /* Enable TRC */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;

  /* Enable clock cycle counter */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

  /* Delay 1ms */
  HAL_Delay(1);

  /* Check if clock cycle counter has started */
  if (DWT->CYCCNT)
  {
     return 0; /* Clock cycle counter started */
  }
  else
  {
    return 1; /* Clock cycle counter not started */
  }
}

/**
 * @brief  Get relative time in micro seconds
 * @note   Call at least every 2^32 cycles. Do not call from interrupt context!
 * @retval Relative time in micro seconds
 */
uint32_t DWT_GetTickUS(void)
{
  static uint64_t last_cycle_count_64 = 0;
  uint32_t clock_MHz = HAL_RCC_GetHCLKFreq() / 1000000;

  last_cycle_count_64 += DWT->CYCCNT - (uint32_t)(last_cycle_count_64);

  return (uint32_t)(last_cycle_count_64 / clock_MHz);
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
