/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2024 STMicroelectronics.
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

/** @addtogroup X_NUCLEO_IKS4A1_Examples X_NUCLEO_IKS4A1 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE    256
#define MLC_STATUS_MAX    8
#define FSM_STATUS_MAX   18
#define SHT40AD1B_ODR  1000U
#define MAX_ISPU_BYTES   64U

/* Private macro -------------------------------------------------------------*/
#define DELAY(x)  for (__IO uint32_t counter = (x); counter > 0U; counter--) {}

/* Public variables ----------------------------------------------------------*/
extern void *MotionCompObj[];

int32_t UseLSI = 0;
uint8_t UpdateInterrupt = 0;
uint32_t SensorsEnabled = 0;
uint32_t StartTime = 0;
volatile uint8_t IspuBytes = MAX_ISPU_BYTES;

/* Private variables ---------------------------------------------------------*/
static CRC_HandleTypeDef CrcHandle;
static RTC_HandleTypeDef RtcHandle;
static uint32_t PreviousSensorsEnabled = 0;
static volatile uint32_t IntCurrentTime1 = 0;
static volatile uint32_t IntCurrentTime2 = 0;
static volatile uint8_t DIL24_INT1_Event = 0;
static uint8_t NewData = 0;
static uint16_t NewDataFlags = 0;
static uint8_t MsgIndex = 0;
static uint32_t LastUpdate = 0;
static volatile uint8_t IspuRunning = 0U;
static volatile uint32_t IspuResetStateTickUs = 0U;
static volatile uint32_t IspuSetStateTickUs = 0U;
static volatile uint32_t IspuElapsedTimeUs = 0U;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_CRC_Init(void);
static void MX_RTC_Init(void);

static void Enable_Disable_Sensors(void);

static void Time_Handler(Msg_t *Msg);
static void Press_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Temp_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Hum_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Accelero_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Gyro_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Magneto_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Sensors_Interrupt_Handler(Msg_t *Msg);
static void MLC_Handler(Msg_t *Msg);
static void FSM_Handler(Msg_t *Msg);
static void ISPU_Handler(Msg_t *Msg);
static void QVAR_Handler(Msg_t *Msg, uint32_t Instance);
static void Accelero_HG_Sensor_Handler(Msg_t *Msg, uint32_t Instance);

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
  Msg_t msg_dat;
  Msg_t msg_cmd;

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
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

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
    if (UART_ReceivedMSG((Msg_t *)&msg_cmd) == 1)
    {
      if (msg_cmd.Data[0] == DEV_ADDR)
      {
        (void)HandleMSG((Msg_t *)&msg_cmd);
      }
    }

    /* Enable/Disable sensors */
    if (PreviousSensorsEnabled != SensorsEnabled)
    {
      PreviousSensorsEnabled = SensorsEnabled;
      Enable_Disable_Sensors();
    }

    /* LED control */
    if (DataLoggerActive != 0U)
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

      if ((SensorsEnabled & PRESSURE_SENSOR_ENABLED) == PRESSURE_SENSOR_ENABLED)
      {
        Press_Sensor_Handler(&msg_dat, PrsInstance);
      }

      if ((SensorsEnabled & TEMPERATURE_SENSOR_ENABLED) == TEMPERATURE_SENSOR_ENABLED)
      {
        Temp_Sensor_Handler(&msg_dat, TmpInstance);
      }

      if ((SensorsEnabled & HUMIDITY_SENSOR_ENABLED) == HUMIDITY_SENSOR_ENABLED)
      {
        Hum_Sensor_Handler(&msg_dat, HumInstance);
      }

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

      if ((SensorsEnabled & ISPU_ENABLED) == ISPU_ENABLED)
      {
        ISPU_Handler(&msg_dat);
      }

      if ((SensorsEnabled & QVAR_ENABLED) == QVAR_ENABLED)
      {
        QVAR_Handler(&msg_dat, AccInstance);
      }

      if ((SensorsEnabled & ACCELEROMETER_HG_SENSOR_ENABLED) == ACCELEROMETER_HG_SENSOR_ENABLED)
      {
        Accelero_HG_Sensor_Handler(&msg_dat, AccInstance);
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

  /* Interrupt pins X-NUCLEO-IKS4A1 */
  // LSM6DSV16X  INT1      CN9.6  PWM/D5  PE11
  // LSM6DSV16X  INT2      CN9.5      D4  PF14
  // LSM6DSO16IS INT1      CN8.6      A5  PC0
  // LSM6DSO16IS INT2      CN9.8      D7  PF13
  // LIS2DUXS12  INT1      CN8.4      A3  PB0
  // LIS2MDL     INT/DRDY  CN8.3      A2  PC3
  // LPS22DF     INT_DRDY  CN9.7  PWM/D6  PE9
  // STTS22H     AL/INT1   CN8.5      A4  PC1

  // DIL24 INT1 (USER INT) CN9.3      D2  PF15 (initialized separately)

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin   = GPIO_PIN_0;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure GPIO pins : PC0 PC1 PC3 */
  GPIO_InitStruct.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure GPIO pins : PE9 PE11 */
  GPIO_InitStruct.Pin   = GPIO_PIN_9 | GPIO_PIN_11;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* Configure GPIO pin : PF13 */
  GPIO_InitStruct.Pin   = GPIO_PIN_13;
  GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* Enable and set EXTI Interrupt to the lowest priority */
  /* LSM6DSO16IS INT2 pin where the ISPU sleep state is routed for algorithm execution time measurement */
  HAL_NVIC_SetPriority(EXTI13_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

  /* Configure GPIO pin : PF14 */
  GPIO_InitStruct.Pin   = GPIO_PIN_14;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
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
    if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS4A1_LIS2DTW12_0)
    {
      (void)IKS4A1_HYBRID_MOTION_SENSOR_Enable(AccInstance, HYBRID_ACCELERO);
    }
    else
    {
      (void)IKS4A1_MOTION_SENSOR_Enable(AccInstance, MOTION_ACCELERO);
    }
  }
  else
  {
    if (IsHybridAccSensor == HYBRID_SENSOR && AccInstance == IKS4A1_LIS2DTW12_0)
    {
      (void)IKS4A1_HYBRID_MOTION_SENSOR_Disable(AccInstance, HYBRID_ACCELERO);
    }
    else
    {
      (void)IKS4A1_MOTION_SENSOR_Disable(AccInstance, MOTION_ACCELERO);
    }
  }

  if ((SensorsEnabled & GYROSCOPE_SENSOR_ENABLED) == GYROSCOPE_SENSOR_ENABLED)
  {
    (void)IKS4A1_MOTION_SENSOR_Enable(GyrInstance, MOTION_GYRO);
  }
  else
  {
    (void)IKS4A1_MOTION_SENSOR_Disable(GyrInstance, MOTION_GYRO);
  }

  if ((SensorsEnabled & MAGNETIC_SENSOR_ENABLED) == MAGNETIC_SENSOR_ENABLED)
  {
    (void)IKS4A1_MOTION_SENSOR_Enable(MagInstance, MOTION_MAGNETO);
  }
  else
  {
    (void)IKS4A1_MOTION_SENSOR_Disable(MagInstance, MOTION_MAGNETO);
  }

  if ((SensorsEnabled & HUMIDITY_SENSOR_ENABLED) == HUMIDITY_SENSOR_ENABLED)
  {
    (void)IKS4A1_ENV_SENSOR_Enable(HumInstance, ENV_HUMIDITY);
  }
  else
  {
    (void)IKS4A1_ENV_SENSOR_Disable(HumInstance, ENV_HUMIDITY);
  }

  if ((SensorsEnabled & TEMPERATURE_SENSOR_ENABLED) == TEMPERATURE_SENSOR_ENABLED)
  {
    if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS4A1_LIS2DTW12_0)
    {
      (void)IKS4A1_HYBRID_ENV_SENSOR_Enable(TmpInstance, HYBRID_TEMPERATURE);
    }
    else
    {
      (void)IKS4A1_ENV_SENSOR_Enable(TmpInstance, ENV_TEMPERATURE);
    }
  }
  else
  {
    if (IsHybridTmpSensor == HYBRID_SENSOR && TmpInstance == IKS4A1_LIS2DTW12_0)
    {
      (void)IKS4A1_HYBRID_ENV_SENSOR_Disable(TmpInstance, HYBRID_TEMPERATURE);
    }
    else
    {
      (void)IKS4A1_ENV_SENSOR_Disable(TmpInstance, ENV_TEMPERATURE);
    }
  }

  if ((SensorsEnabled & PRESSURE_SENSOR_ENABLED) == PRESSURE_SENSOR_ENABLED)
  {
    (void)IKS4A1_ENV_SENSOR_Enable(PrsInstance, ENV_PRESSURE);
  }
  else
  {
    (void)IKS4A1_ENV_SENSOR_Disable(PrsInstance, ENV_PRESSURE);
  }
}

/**
 * @brief  Handles the precise time
 * @param  Msg the time part of the stream
 * @retval None
 */
static void Time_Handler(Msg_t *Msg)
{
  uint32_t time_us;

  if (DataLoggerActive != 0U)
  {
    time_us = DWT_GetTickUS() - StartTime;
    Serialize(&Msg->Data[3], time_us, 4);
    MsgIndex = 9;
  }
  else
  {
    /* Nothing to do */
  }
}

/**
 * @brief  Handles the PRESSURE sensor data getting/sending
 * @param  Msg the PRESSURE part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Press_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  float pressure;
  uint8_t status = 0;

  if (IKS4A1_ENV_SENSOR_Get_DRDY_Status(Instance, ENV_PRESSURE, &status) == BSP_ERROR_NONE && status == 1U)
  {
    NewData++;
    NewDataFlags |= PRESSURE_SENSOR_SYNC;

    (void)IKS4A1_ENV_SENSOR_GetValue(Instance, ENV_PRESSURE, &pressure);

    if (DataLoggerActive != 0U)
    {
      (void)memcpy(&Msg->Data[MsgIndex], (void *)&pressure, 4);
      MsgIndex = MsgIndex + 4;
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the TEMPERATURE sensor data getting/sending
 * @param  Msg the TEMPERATURE part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Temp_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  float temperature;
  uint8_t status = 0;
  uint8_t drdy = 0;
  static uint8_t stts751_is_busy = 0;
  static uint32_t last_time = 0;
  uint32_t time = 0;
  float odr = 0;

  if (Instance == IKS4A1_STTS751_0)
  {
    if (IKS4A1_ENV_SENSOR_Get_DRDY_Status(Instance, ENV_TEMPERATURE, &status) == BSP_ERROR_NONE)
    {
      if (status == 0)
      {
        stts751_is_busy = 1;
        drdy = 0;
      }
      else
      {
        if (stts751_is_busy == 1)
        {
          stts751_is_busy = 0;
          drdy = 1;
        }
      }
    }
  }
  else if (Instance == IKS4A1_STTS22H_0)
  {
    (void)IKS4A1_ENV_SENSOR_GetOutputDataRate(Instance, ENV_TEMPERATURE, &odr);

    time = HAL_GetTick();
    if ((time - last_time) >= (1000.0f / odr))
    {
      drdy = 1;
      last_time = time;
    }
    else
    {
      drdy = 0;
    }
  }
  else if (IsHybridTmpSensor == HYBRID_SENSOR && Instance == IKS4A1_LIS2DTW12_0)
  {
    if (IKS4A1_HYBRID_SENSOR_Get_DRDY_Status(Instance, HYBRID_TEMPERATURE, &status) == BSP_ERROR_NONE && status == 1U)
    {
      drdy = 1;
    }
    else
    {
      drdy = 0;
    }
  }
  else if (Instance == IKS4A1_SHT40AD1B_0)
  {
    time = HAL_GetTick();
    if ((time - last_time) >= SHT40AD1B_ODR)
    {
      drdy = 1;
      last_time = time;
    }
    else
    {
      drdy = 0;
    }
  }
  else
  {
    if (IKS4A1_ENV_SENSOR_Get_DRDY_Status(Instance, ENV_TEMPERATURE, &status) == BSP_ERROR_NONE && status == 1U)
    {
      drdy = 1;
    }
    else
    {
      drdy = 0;
    }
  }

  if (drdy == 1)
  {
    NewData++;
    NewDataFlags |= TEMPERATURE_SENSOR_SYNC;

    if (IsHybridTmpSensor == HYBRID_SENSOR && Instance == IKS4A1_LIS2DTW12_0)
    {
      (void)IKS4A1_HYBRID_ENV_SENSOR_GetValue(Instance, HYBRID_TEMPERATURE, &temperature);
    }
    else
    {
      (void)IKS4A1_ENV_SENSOR_GetValue(Instance, ENV_TEMPERATURE, &temperature);
    }

    if (DataLoggerActive != 0U)
    {
      (void)memcpy(&Msg->Data[MsgIndex], (void *)&temperature, 4);
      MsgIndex = MsgIndex + 4;
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the HUMIDITY sensor data getting/sending
 * @param  Msg the HUMIDITY part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Hum_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  float humidity;
  uint8_t status = 0;
  static uint32_t last_time = 0;
  uint32_t time = 0;

  if (Instance == IKS4A1_SHT40AD1B_0)
  {
    time = HAL_GetTick();
    if ((time - last_time) >= SHT40AD1B_ODR)
    {
      status = 1;
      last_time = time;
    }
    else
    {
      status = 0;
    }
  }
  else
  {
    if (IKS4A1_ENV_SENSOR_Get_DRDY_Status(Instance, ENV_HUMIDITY, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }
  }

  if (status == 1)
  {
    NewData++;
    NewDataFlags |= HUMIDITY_SENSOR_SYNC;

    (void)IKS4A1_ENV_SENSOR_GetValue(Instance, ENV_HUMIDITY, &humidity);

    if (DataLoggerActive != 0U)
    {
      (void)memcpy(&Msg->Data[MsgIndex], (void *)&humidity, 4);
      MsgIndex = MsgIndex + 4;
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the ACCELERO axes data getting/sending
 * @param  Msg the ACCELERO part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Accelero_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  IKS4A1_MOTION_SENSOR_Axes_t acceleration;
  IKS4A1_HYBRID_MOTION_SENSOR_Axes_t hyb_acc;
  uint8_t status = 0;

  if (IsHybridAccSensor == HYBRID_SENSOR && Instance == IKS4A1_LIS2DTW12_0)
  {
    if (IKS4A1_HYBRID_SENSOR_Get_DRDY_Status(Instance, HYBRID_ACCELERO, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }
  }
  else
  {
    if (IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }
  }

  if (Instance == IKS4A1_LSM6DSV80X_0)
  {
    status &= 0x01;  /* Only low-g accelerometer data ready status */
  }

  if (Instance == IKS4A1_LSM6DSV320X_0)
  {
    status &= 0x01;  /* Only low-g accelerometer data ready status */
  }

  if (status == 1)
  {
    NewData++;
    NewDataFlags |= ACCELEROMETER_SENSOR_SYNC;

    if (IsHybridAccSensor == HYBRID_SENSOR && Instance == IKS4A1_LIS2DTW12_0)
    {
      (void)IKS4A1_HYBRID_MOTION_SENSOR_GetAxes(Instance, HYBRID_ACCELERO, &hyb_acc);
      acceleration.x = hyb_acc.x;
      acceleration.y = hyb_acc.y;
      acceleration.z = hyb_acc.z;
    }
    else
    {
      (void)IKS4A1_MOTION_SENSOR_GetAxes(Instance, MOTION_ACCELERO, &acceleration);
    }

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], acceleration.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], acceleration.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], acceleration.z, 4);
      MsgIndex = MsgIndex + 12;
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
static void Gyro_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  IKS4A1_MOTION_SENSOR_Axes_t angular_velocity;
  uint8_t status = 0;

  if (IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_GYRO, &status) == BSP_ERROR_NONE && status == 1U)
  {
    NewData++;
    NewDataFlags |= GYROSCOPE_SENSOR_SYNC;

    (void)IKS4A1_MOTION_SENSOR_GetAxes(Instance, MOTION_GYRO, &angular_velocity);

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], angular_velocity.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], angular_velocity.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], angular_velocity.z, 4);
      MsgIndex = MsgIndex + 12;
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
static void Magneto_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  IKS4A1_MOTION_SENSOR_Axes_t magnetic_field;
  uint8_t status = 0;

  if (IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_MAGNETO, &status) == BSP_ERROR_NONE && status == 1U)
  {
    NewData++;
    NewDataFlags |= MAGNETIC_SENSOR_SYNC;

    (void)IKS4A1_MOTION_SENSOR_GetAxes(Instance, MOTION_MAGNETO, &magnetic_field);

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], (int32_t)magnetic_field.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], (int32_t)magnetic_field.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], (int32_t)magnetic_field.z, 4);
      MsgIndex = MsgIndex + 12;
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
static void Sensors_Interrupt_Handler(Msg_t *Msg)
{
  uint16_t int_status = 0;
  static uint16_t mem_int_status = 0;

  /* Interrupt pins X-NUCLEO-IKS4A1 */
  // LSM6DSV16X  INT1      CN9.6  PWM/D5  PE11
  // LSM6DSV16X  INT2      CN9.5      D4  PF14
  // LSM6DSO16IS INT1      CN8.6      A5  PC0
  // LSM6DSO16IS INT2      CN9.8      D7  PF13
  // LIS2DUXS12  INT1      CN8.4      A3  PB0
  // LIS2MDL     INT/DRDY  CN8.3      A2  PC3
  // LPS22DF     INT_DRDY  CN9.7  PWM/D6  PE9
  // STTS22H     AL/INT1   CN8.5      A4  PC1

  // DIL24 INT1 (USER INT) CN9.3      D2  PF15 (initialized separately)

  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11)  == GPIO_PIN_SET) int_status |= (1 << 0); else int_status &= ~(1 << 0);
  if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_14)  == GPIO_PIN_SET) int_status |= (1 << 1); else int_status &= ~(1 << 1);
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)  == GPIO_PIN_SET) int_status |= (1 << 2); else int_status &= ~(1 << 2);
  if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_13)  == GPIO_PIN_SET) int_status |= (1 << 3); else int_status &= ~(1 << 3);
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)  == GPIO_PIN_SET) int_status |= (1 << 4); else int_status &= ~(1 << 4);
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)  == GPIO_PIN_SET) int_status |= (1 << 5); else int_status &= ~(1 << 5);
  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET) int_status |= (1 << 6); else int_status &= ~(1 << 6);
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)  == GPIO_PIN_SET) int_status |= (1 << 7); else int_status &= ~(1 << 7);

  if (DIL24_INT1_Event == 1)
  {
    DIL24_INT1_Event = 0;
    int_status |= (1 << 8);
  }
  else
  {
    int_status &= ~(1 << 8);
  }

  if ((mem_int_status != int_status) || (UpdateInterrupt == 1) || ((HAL_GetTick() - LastUpdate) > 100))
  {
    UpdateInterrupt = 0;
    NewData++;
    NewDataFlags |= INTERRUPTS_SYNC;
    Msg->Data[MsgIndex++] = (uint8_t)int_status;
    Msg->Data[MsgIndex++] = (uint8_t)(int_status >> 8);

    mem_int_status = int_status;
    LastUpdate = HAL_GetTick();
  }
}

/**
 * @brief  Handles the MLC status data
 * @param  Msg the MLC part of the stream
 * @retval None
 */
static void MLC_Handler(Msg_t *Msg)
{
  int32_t i = 0;
  uint8_t new_status = 0;
  uint8_t mlc_status[MLC_STATUS_MAX];
  uint8_t mlc_status_max = 0;
  static uint8_t mem_mlc_status[MLC_STATUS_MAX] = {0};

  if ((AccInstance == IKS4A1_LSM6DSOX_0) && (GyrInstance == IKS4A1_LSM6DSOX_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSOX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSOX_0], LSM6DSOX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC0_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC1_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC2_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC3_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC4_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC5_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC6_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_MLC7_SRC, &mlc_status[7]);

    (void)LSM6DSOX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSOX_0], LSM6DSOX_USER_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSRX_0) && (GyrInstance == IKS4A1_LSM6DSRX_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSRX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSRX_0], LSM6DSRX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC0_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC1_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC2_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC3_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC4_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC5_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC6_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_MLC7_SRC, &mlc_status[7]);

    (void)LSM6DSRX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSRX_0], LSM6DSRX_USER_BANK);
  }
  else if ((AccInstance == IKS4A1_ISM330DHCX_0) && (GyrInstance == IKS4A1_ISM330DHCX_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ISM330DHCX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330DHCX_0], ISM330DHCX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC0_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC1_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC2_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC3_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC4_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC5_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC6_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_MLC7_SRC, &mlc_status[7]);

    (void)ISM330DHCX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330DHCX_0], ISM330DHCX_USER_BANK);
  }
  else if (AccInstance == IKS4A1_IIS2ICLX_0)
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)IIS2ICLX_Set_Mem_Bank(MotionCompObj[IKS4A1_IIS2ICLX_0], IIS2ICLX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC0_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC1_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC2_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC3_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC4_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC5_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC6_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_MLC7_SRC, &mlc_status[7]);

    (void)IIS2ICLX_Set_Mem_Bank(MotionCompObj[IKS4A1_IIS2ICLX_0], IIS2ICLX_USER_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSO32X_0) && (GyrInstance == IKS4A1_LSM6DSO32X_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSO32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSO32X_0], LSM6DSO32X_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC0_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC1_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC2_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC3_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC4_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC5_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC6_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_MLC7_SRC, &mlc_status[7]);

    (void)LSM6DSO32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSO32X_0], LSM6DSO32X_USER_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV16X_0) && (GyrInstance == IKS4A1_LSM6DSV16X_0))
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV16X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16X_0], LSM6DSV16X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_MLC4_SRC, &mlc_status[3]);

    (void)LSM6DSV16X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16X_0], LSM6DSV16X_MAIN_MEM_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV16BX_0) && (GyrInstance == IKS4A1_LSM6DSV16BX_0))
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV16BX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16BX_0], LSM6DSV16BX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_MLC4_SRC, &mlc_status[3]);

    (void)LSM6DSV16BX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16BX_0], LSM6DSV16BX_MAIN_MEM_BANK);
  }
  else if ((AccInstance == IKS4A1_ISM330BX_0) && (GyrInstance == IKS4A1_ISM330BX_0))
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ISM330BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330BX_0], ISM330BX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_MLC4_SRC, &mlc_status[3]);

    (void)ISM330BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330BX_0], ISM330BX_MAIN_MEM_BANK);
  }
  else if (AccInstance == IKS4A1_LIS2DUX12_0)
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LIS2DUX12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUX12_0], LIS2DUX12_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_MLC4_SRC, &mlc_status[3]);

    (void)LIS2DUX12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUX12_0], LIS2DUX12_MAIN_MEM_BANK);
  }
  else if (AccInstance == IKS4A1_LIS2DUXS12_0)
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LIS2DUXS12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUXS12_0], LIS2DUXS12_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_MLC4_SRC, &mlc_status[3]);

    (void)LIS2DUXS12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUXS12_0], LIS2DUXS12_MAIN_MEM_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV32X_0) && (GyrInstance == IKS4A1_LSM6DSV32X_0))
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV32X_0], LSM6DSV32X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_MLC4_SRC, &mlc_status[3]);

    (void)LSM6DSV32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV32X_0], LSM6DSV32X_MAIN_MEM_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV80X_0) && (GyrInstance == IKS4A1_LSM6DSV80X_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV80X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV80X_0], LSM6DSV80X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC4_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC5_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC6_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC7_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_MLC8_SRC, &mlc_status[7]);

    (void)LSM6DSV80X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV80X_0], LSM6DSV80X_MAIN_MEM_BANK);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV320X_0) && (GyrInstance == IKS4A1_LSM6DSV320X_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV320X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV320X_0], LSM6DSV320X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC4_SRC, &mlc_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC5_SRC, &mlc_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC6_SRC, &mlc_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC7_SRC, &mlc_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_MLC8_SRC, &mlc_status[7]);

    (void)LSM6DSV320X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV320X_0], LSM6DSV320X_MAIN_MEM_BANK);
  }
  else if (AccInstance == IKS4A1_ST1VAFE3BX_0)
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ST1VAFE3BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE3BX_0], ST1VAFE3BX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_MLC4_SRC, &mlc_status[3]);

    (void)ST1VAFE3BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE3BX_0], ST1VAFE3BX_MAIN_MEM_BANK);
  }
  else if ((AccInstance == IKS4A1_ST1VAFE6AX_0) && (GyrInstance == IKS4A1_ST1VAFE6AX_0))
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ST1VAFE6AX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE6AX_0], ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_MLC1_SRC, &mlc_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_MLC2_SRC, &mlc_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_MLC3_SRC, &mlc_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_MLC4_SRC, &mlc_status[3]);

    (void)ST1VAFE6AX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE6AX_0], ST1VAFE6AX_MAIN_MEM_BANK);
  }
  else
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

    mlc_status[0] = 0;
    mlc_status[1] = 0;
    mlc_status[2] = 0;
    mlc_status[3] = 0;
    mlc_status[4] = 0;
    mlc_status[5] = 0;
    mlc_status[6] = 0;
    mlc_status[7] = 0;
  }

  for (i = 0; i < mlc_status_max; i++)
  {
    if (mlc_status[i] != mem_mlc_status[i])
    {
      new_status = 1;
      break;
    }
  }

  if (new_status == 1)
  {
    memcpy((void *)&Msg->Data[MsgIndex], (void *)&mlc_status, mlc_status_max);
    memcpy((void *)&mem_mlc_status, (void *)mlc_status, mlc_status_max);
    MsgIndex = MsgIndex + mlc_status_max;

    NewData++;
    NewDataFlags |= MLC_SYNC;
  }
}

/**
 * @brief  Handles the FSM status data
 * @param  Msg the FSM part of the stream
 * @retval None
 */
static void FSM_Handler(Msg_t *Msg)
{
  int32_t i = 0;
  uint8_t new_status = 0;
  uint8_t fsm_status[FSM_STATUS_MAX];
  uint8_t fsm_status_max = 0;
  static uint8_t mem_fsm_status[FSM_STATUS_MAX] = {0};

  if ((AccInstance == IKS4A1_LSM6DSO_0) && (GyrInstance == IKS4A1_LSM6DSO_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSO_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSO_0], LSM6DSO_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_OUTS16, &fsm_status[15]);

    (void)LSM6DSO_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSO_0], LSM6DSO_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO_0, LSM6DSO_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSOX_0) && (GyrInstance == IKS4A1_LSM6DSOX_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSOX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSOX_0], LSM6DSOX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_OUTS16, &fsm_status[15]);

    (void)LSM6DSOX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSOX_0], LSM6DSOX_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSOX_0, LSM6DSOX_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSR_0) && (GyrInstance == IKS4A1_LSM6DSR_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSR_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSR_0], LSM6DSR_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_OUTS16, &fsm_status[15]);

    (void)LSM6DSR_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSR_0], LSM6DSR_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSR_0, LSM6DSR_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSRX_0) && (GyrInstance == IKS4A1_LSM6DSRX_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSRX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSRX_0], LSM6DSRX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_OUTS16, &fsm_status[15]);

    (void)LSM6DSRX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSRX_0], LSM6DSRX_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSRX_0, LSM6DSRX_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS4A1_ISM330DHCX_0) && (GyrInstance == IKS4A1_ISM330DHCX_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ISM330DHCX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330DHCX_0], ISM330DHCX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_OUTS16, &fsm_status[15]);

    (void)ISM330DHCX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330DHCX_0], ISM330DHCX_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330DHCX_0, ISM330DHCX_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if (AccInstance == IKS4A1_IIS2ICLX_0)
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)IIS2ICLX_Set_Mem_Bank(MotionCompObj[IKS4A1_IIS2ICLX_0], IIS2ICLX_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_OUTS16, &fsm_status[15]);

    (void)IIS2ICLX_Set_Mem_Bank(MotionCompObj[IKS4A1_IIS2ICLX_0], IIS2ICLX_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_IIS2ICLX_0, IIS2ICLX_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSO32X_0) && (GyrInstance == IKS4A1_LSM6DSO32X_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSO32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSO32X_0], LSM6DSO32X_EMBEDDED_FUNC_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS8, &fsm_status[7]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS9, &fsm_status[8]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS10, &fsm_status[9]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS11, &fsm_status[10]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS12, &fsm_status[11]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS13, &fsm_status[12]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS14, &fsm_status[13]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS15, &fsm_status[14]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_OUTS16, &fsm_status[15]);

    (void)LSM6DSO32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSO32X_0], LSM6DSO32X_USER_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSO32X_0, LSM6DSO32X_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV16X_0) && (GyrInstance == IKS4A1_LSM6DSV16X_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV16X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16X_0], LSM6DSV16X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV16X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16X_0], LSM6DSV16X_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV16BX_0) && (GyrInstance == IKS4A1_LSM6DSV16BX_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV16BX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16BX_0], LSM6DSV16BX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV16BX_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16BX_0], LSM6DSV16BX_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_ISM330BX_0) && (GyrInstance == IKS4A1_ISM330BX_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ISM330BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330BX_0], ISM330BX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_OUTS8, &fsm_status[7]);

    (void)ISM330BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ISM330BX_0], ISM330BX_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if (AccInstance == IKS4A1_LIS2DUX12_0)
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LIS2DUX12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUX12_0], LIS2DUX12_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_OUTS8, &fsm_status[7]);

    (void)LIS2DUX12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUX12_0], LIS2DUX12_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUX12_0, LIS2DUX12_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if (AccInstance == IKS4A1_LIS2DUXS12_0)
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LIS2DUXS12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUXS12_0], LIS2DUXS12_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_OUTS8, &fsm_status[7]);

    (void)LIS2DUXS12_Set_Mem_Bank(MotionCompObj[IKS4A1_LIS2DUXS12_0], LIS2DUXS12_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV_0) && (GyrInstance == IKS4A1_LSM6DSV_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV_0], LSM6DSV_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV_0], LSM6DSV_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV_0, LSM6DSV_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV16B_0) && (GyrInstance == IKS4A1_LSM6DSV16B_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV16B_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16B_0], LSM6DSV16B_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV16B_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV16B_0], LSM6DSV16B_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16B_0, LSM6DSV16B_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV32X_0) && (GyrInstance == IKS4A1_LSM6DSV32X_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV32X_0], LSM6DSV32X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV32X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV32X_0], LSM6DSV32X_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV80X_0) && (GyrInstance == IKS4A1_LSM6DSV80X_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV80X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV80X_0], LSM6DSV80X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV80X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV80X_0], LSM6DSV80X_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV80X_0, LSM6DSV80X_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_LSM6DSV320X_0) && (GyrInstance == IKS4A1_LSM6DSV320X_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)LSM6DSV320X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV320X_0], LSM6DSV320X_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_OUTS8, &fsm_status[7]);

    (void)LSM6DSV320X_Set_Mem_Bank(MotionCompObj[IKS4A1_LSM6DSV320X_0], LSM6DSV320X_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV320X_0, LSM6DSV320X_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if (AccInstance == IKS4A1_ST1VAFE3BX_0)
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ST1VAFE3BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE3BX_0], ST1VAFE3BX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_OUTS8, &fsm_status[7]);

    (void)ST1VAFE3BX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE3BX_0], ST1VAFE3BX_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if ((AccInstance == IKS4A1_ST1VAFE6AX_0) && (GyrInstance == IKS4A1_ST1VAFE6AX_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)ST1VAFE6AX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE6AX_0], ST1VAFE6AX_EMBED_FUNC_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_OUTS8, &fsm_status[7]);

    (void)ST1VAFE6AX_Set_Mem_Bank(MotionCompObj[IKS4A1_ST1VAFE6AX_0], ST1VAFE6AX_MAIN_MEM_BANK);

    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

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
    fsm_status[16] = 0;
    fsm_status[17] = 0;
  }

  for (i = 0; i < fsm_status_max; i++)
  {
    if (fsm_status[i] != mem_fsm_status[i])
    {
      new_status = 1;
      break;
    }
  }

  if (new_status == 1)
  {
    memcpy((void *)&Msg->Data[MsgIndex], (void *)&fsm_status, fsm_status_max);
    memcpy((void *)&mem_fsm_status, (void *)fsm_status, fsm_status_max);
    MsgIndex = MsgIndex + fsm_status_max;

    NewData++;
    NewDataFlags |= FSM_SYNC;
  }
}

/**
 * @brief  Handles the ISPU data
 * @param  Msg the ISPU part of the stream
 * @retval None
 */
static void ISPU_Handler(Msg_t *Msg)
{
  uint8_t i;
  uint8_t new_status = 0;
  uint8_t int_status = 0;
  uint8_t ispu_status[MAX_ISPU_BYTES];
  static uint8_t mem_ispu_status[MAX_ISPU_BYTES];

  if ((AccInstance == IKS4A1_LSM6DSO16IS_0) && (GyrInstance == IKS4A1_LSM6DSO16IS_0))
  {
    /* Enable routing ISPU sleep state to INT2 pin */
    IKS4A1_MOTION_SENSOR_Write_Register(AccInstance, LSM6DSO16IS_INT2_CTRL, 0x80);

    IKS4A1_MOTION_SENSOR_Read_Register(AccInstance, LSM6DSO16IS_ISPU_INT_STATUS0_MAINPAGE, &int_status);

    if (IspuBytes > MAX_ISPU_BYTES)
    {
      IspuBytes = MAX_ISPU_BYTES;
    }

    if (int_status != 0U)
    {
      IKS4A1_MOTION_SENSOR_Write_Register(AccInstance, LSM6DSO16IS_FUNC_CFG_ACCESS, 0x80);

      for (i = 0U; i < IspuBytes; i++)
      {
        IKS4A1_MOTION_SENSOR_Read_Register(AccInstance, LSM6DSO16IS_ISPU_DOUT_00_L + i, &ispu_status[i]);
      }

      IKS4A1_MOTION_SENSOR_Write_Register(AccInstance, LSM6DSO16IS_FUNC_CFG_ACCESS, 0x00);

      for (i = 0U; i < IspuBytes; i++)
      {
        if (ispu_status[i] != mem_ispu_status[i])
        {
          new_status = 1;
          break;
        }
      }
    }
  }

  if (new_status == 1U)
  {
    (void)memcpy((void *)&Msg->Data[MsgIndex], (void *)ispu_status, IspuBytes);
    (void)memcpy((void *)mem_ispu_status, (void *)ispu_status, IspuBytes);
    MsgIndex += IspuBytes;

    /* Add ISPU algorithm execution time in [us] to the stream */
    (void)memcpy((void *)&Msg->Data[MsgIndex], (void *)&IspuElapsedTimeUs, sizeof(IspuElapsedTimeUs));
    MsgIndex += (uint8_t)sizeof(IspuElapsedTimeUs);

    NewData++;
    NewDataFlags |= ISPU_SYNC;
  }
}

/**
 * @brief  Handles the QVAR data
 * @param  Msg the QVAR part of the stream
 * @retval None
 */
static void QVAR_Handler(Msg_t *Msg, uint32_t Instance)
{
  uint8_t qvar_data_available = 0;

  if ((AccInstance == IKS4A1_LSM6DSV16X_0) && (GyrInstance == IKS4A1_LSM6DSV16X_0))
  {
    lsm6dsv16x_status_reg_t status;
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_STATUS_REG, (uint8_t *)&status);
    qvar_data_available = status.ah_qvarda;
  }
  else if ((AccInstance == IKS4A1_LSM6DSV16BX_0) && (GyrInstance == IKS4A1_LSM6DSV16BX_0))
  {
    lsm6dsv16bx_status_reg_t status;
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_STATUS_REG, (uint8_t *)&status);
    qvar_data_available = status.ah_qvarda;
  }
  else if ((AccInstance == IKS4A1_ISM330BX_0) && (GyrInstance == IKS4A1_ISM330BX_0))
  {
    ism330bx_status_reg_t status;
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_STATUS_REG, (uint8_t *)&status);
    qvar_data_available = status.ah_qvarda;
  }
  else if (AccInstance == IKS4A1_LIS2DUXS12_0)
  {
    if((SensorsEnabled & ACCELEROMETER_SENSOR_ENABLED) == ACCELEROMETER_SENSOR_ENABLED)
    {
      qvar_data_available = ((NewDataFlags & ACCELEROMETER_SENSOR_SYNC) == ACCELEROMETER_SENSOR_SYNC) ? 1 : 0;
    }
    else
    {
      IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &qvar_data_available);
    }
  }
  else if ((AccInstance == IKS4A1_LSM6DSV32X_0) && (GyrInstance == IKS4A1_LSM6DSV32X_0))
  {
    lsm6dsv32x_status_reg_t status;
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_STATUS_REG, (uint8_t *)&status);
    qvar_data_available = status.ah_qvarda;
  }
  else if (AccInstance == IKS4A1_ST1VAFE3BX_0)
  {
    if((SensorsEnabled & ACCELEROMETER_SENSOR_ENABLED) == ACCELEROMETER_SENSOR_ENABLED)
    {
      qvar_data_available = ((NewDataFlags & ACCELEROMETER_SENSOR_SYNC) == ACCELEROMETER_SENSOR_SYNC) ? 1 : 0;
    }
    else
    {
      IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &qvar_data_available);
    }
  }
  else if ((AccInstance == IKS4A1_ST1VAFE6AX_0) && (GyrInstance == IKS4A1_ST1VAFE6AX_0))
  {
    st1vafe6ax_status_reg_t status;
    (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_STATUS_REG, (uint8_t *)&status);
    qvar_data_available = status.ah_bioda;
  }
  else
  {
    qvar_data_available = 0;
  }

  if (qvar_data_available == 1)
  {
    NewData++;
    NewDataFlags |= QVAR_SYNC;

    union {
      int16_t i16bit;
      uint8_t u8bit[2];
    } value;

    float qvar_mv;

    if ((AccInstance == IKS4A1_LSM6DSV16X_0) && (GyrInstance == IKS4A1_LSM6DSV16X_0))
    {
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_AH_QVAR_OUT_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_AH_QVAR_OUT_H, &value.u8bit[1]);
      qvar_mv = value.i16bit / LSM6DSV16X_QVAR_GAIN;
    }
    else if ((AccInstance == IKS4A1_LSM6DSV16BX_0) && (GyrInstance == IKS4A1_LSM6DSV16BX_0))
    {
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_AH_QVAR_OUT_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16BX_0, LSM6DSV16BX_AH_QVAR_OUT_H, &value.u8bit[1]);
      qvar_mv = value.i16bit / LSM6DSV16BX_QVAR_GAIN;
    }
    else if ((AccInstance == IKS4A1_ISM330BX_0) && (GyrInstance == IKS4A1_ISM330BX_0))
    {
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_AH_QVAR_OUT_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ISM330BX_0, ISM330BX_AH_QVAR_OUT_H, &value.u8bit[1]);
      qvar_mv = value.i16bit / ISM330BX_QVAR_GAIN;
    }
    else if (AccInstance == IKS4A1_LIS2DUXS12_0)
    {
      lis2duxs12_ah_qvar_cfg_t cfg;
      float multiplier;

      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_OUT_T_AH_QVAR_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_OUT_T_AH_QVAR_H, &value.u8bit[1]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2DUXS12_0, LIS2DUXS12_AH_QVAR_CFG, (uint8_t *)&cfg);

      switch (cfg.ah_qvar_gain)
      {
      case LIS2DUXS12_GAIN_0_5:
        multiplier = LIS2DUXS12_QVAR_GAIN_MULTIPL_0_5X;
        break;

      case LIS2DUXS12_GAIN_1:
        multiplier = LIS2DUXS12_QVAR_GAIN_MULTIPL_1X;
        break;

      case LIS2DUXS12_GAIN_2:
        multiplier = LIS2DUXS12_QVAR_GAIN_MULTIPL_2X;
        break;

      case LIS2DUXS12_GAIN_4:
        multiplier = LIS2DUXS12_QVAR_GAIN_MULTIPL_4X;
        break;

      default:
        multiplier = -1.0f;
      }

      qvar_mv = value.i16bit / (LIS2DUXS12_QVAR_GAIN * multiplier);
    }
    else if ((AccInstance == IKS4A1_LSM6DSV32X_0) && (GyrInstance == IKS4A1_LSM6DSV32X_0))
    {
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_AH_QVAR_OUT_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV32X_0, LSM6DSV32X_AH_QVAR_OUT_H, &value.u8bit[1]);
      qvar_mv = value.i16bit / LSM6DSV32X_QVAR_GAIN;
    }
    else if (AccInstance == IKS4A1_ST1VAFE3BX_0)
    {
      st1vafe3bx_ah_bio_cfg2_t cfg;
      float multiplier;

      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_OUT_AH_BIO_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_OUT_AH_BIO_H, &value.u8bit[1]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE3BX_0, ST1VAFE3BX_AH_BIO_CFG2, (uint8_t *)&cfg);

      switch (cfg.ah_bio_gain)
      {
      case ST1VAFE3BX_GAIN_2:
        multiplier = ST1VAFE3BX_AH_BIO_GAIN_MULTIPL_2X;
        break;

      case ST1VAFE3BX_GAIN_4:
        multiplier = ST1VAFE3BX_AH_BIO_GAIN_MULTIPL_4X;
        break;

      case ST1VAFE3BX_GAIN_8:
        multiplier = ST1VAFE3BX_AH_BIO_GAIN_MULTIPL_8X;
        break;

      case ST1VAFE3BX_GAIN_16:
        multiplier = ST1VAFE3BX_AH_BIO_GAIN_MULTIPL_16X;
        break;

      default:
        multiplier = -1.0f;
      }

      qvar_mv = value.i16bit / (ST1VAFE3BX_AH_BIO_GAIN * multiplier);
    }
    else if ((AccInstance == IKS4A1_ST1VAFE6AX_0) && (GyrInstance == IKS4A1_ST1VAFE6AX_0))
    {
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_AH_BIO_OUT_L, &value.u8bit[0]);
      (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_ST1VAFE6AX_0, ST1VAFE6AX_AH_BIO_OUT_H, &value.u8bit[1]);
      qvar_mv = value.i16bit / ST1VAFE6AX_BIO_GAIN;
    }
    else
    {
      qvar_mv = 0.0f;
    }

    if (DataLoggerActive != 0U)
    {
      (void)memcpy(&Msg->Data[MsgIndex], (void *)&qvar_mv, 4);
      MsgIndex = MsgIndex + 4;
    }
    else
    {
      /* Nothing to do */
    }
  }
}

/**
 * @brief  Handles the ACCELERO High-G axes data getting/sending
 * @param  Msg the ACCELERO High-G part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Accelero_HG_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  IKS4A1_MOTION_SENSOR_Axes_t acceleration;
  uint8_t status = 0;

  if (Instance == IKS4A1_LSM6DSV80X_0)
  {
    if (IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }

    status = (status & 0x02) >> 1;  /* Only high-g accelerometer data ready status */
  }

  if (Instance == IKS4A1_LSM6DSV320X_0)
  {
    if (IKS4A1_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }

    status = (status & 0x02) >> 1;  /* Only high-g accelerometer data ready status */
  }

  if (status == 1)
  {
    NewData++;
    NewDataFlags |= ACCELEROMETER_HG_SENSOR_SYNC;

    (void)IKS4A1_MOTION_SENSOR_ACC_HG_GetAxes(Instance, MOTION_ACCELERO, &acceleration);

    if (DataLoggerActive != 0U)
    {
      Serialize_s32(&Msg->Data[MsgIndex + 0], acceleration.x, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 4], acceleration.y, 4);
      Serialize_s32(&Msg->Data[MsgIndex + 8], acceleration.z, 4);
      MsgIndex = MsgIndex + 12;
    }
    else
    {
      /* Nothing to do */
    }
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
    /* Initialization Error */
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
    /* Initialization Error */
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&RtcHandle, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
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
  if (GPIOPin == GPIO_PIN_15)
  {
    DIL24_INT1_Event = 1;
  }
  /* LSM6DSO16IS INT2 pin where the ISPU sleep state is routed for algorithm execution time measurement */
  else if (GPIOPin == GPIO_PIN_13)
  {
    IspuSetStateTickUs = DWT_GetTickUS();

    /* Calculate ISPU algorithm execution time in [us] */
    if (IspuRunning == 1U)
    {
      IspuRunning = 0U;

      uint32_t reset_us = IspuResetStateTickUs;
      uint32_t set_us   = IspuSetStateTickUs;

      IspuElapsedTimeUs = set_us - reset_us;
    }
  }
}

/**
  * @brief  EXTI line detection callbacks
  * @param  GPIOPin the pin connected to EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIOPin)
{
  /* LSM6DSO16IS INT2 pin where the ISPU sleep state is routed for algorithm execution time measurement */
  if (GPIOPin == GPIO_PIN_13)
  {
    IspuRunning = 1U;
    IspuResetStateTickUs = DWT_GetTickUS();
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
  __HAL_RCC_GPIOF_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin  = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_RESET);

  /* Unlock DIL24 I2C bus SDA line if LPS22HH DIL24 used */
#define UNLOCK_VALUE     0x42
#define UNLOCK_ADDRESS   0xB9
#define UNLOCK_REGISTER  0x0E

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin  = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  for (int i = 0; i < 10; i++)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    DELAY(10U);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    DELAY(10U);
  }

  BSP_I2C1_Init();

  uint8_t value = UNLOCK_VALUE;
  BSP_I2C1_WriteReg(UNLOCK_ADDRESS, UNLOCK_REGISTER, &value, 1);
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
 * @note   Call at least every 2^32 cycles.
 * @retval Relative time in micro seconds
 */
uint64_t DWT_GetTickUS(void)
{
  static uint64_t last_cycle_count_64 = 0;
  uint32_t clock_MHz = HAL_RCC_GetHCLKFreq() / 1000000;

  __disable_irq();
  last_cycle_count_64 += DWT->CYCCNT - (uint32_t)(last_cycle_count_64);
  uint64_t result = last_cycle_count_64;
  __enable_irq();

  return result / clock_MHz;
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
