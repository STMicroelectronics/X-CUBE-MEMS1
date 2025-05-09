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
#define MAX_BUF_SIZE    256
#define MLC_STATUS_MAX    8
#define FSM_STATUS_MAX   18
#define SHT40AD1B_ODR  1000U

/* Private macro -------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
int32_t UseLSI = 0;
uint8_t UpdateInterrupt = 0;
uint32_t SensorsEnabled = 0;
uint32_t StartTime = 0;

/* Private variables ---------------------------------------------------------*/
static char DataOut[MAX_BUF_SIZE];
static RTC_HandleTypeDef RtcHandle;
static uint32_t PreviousSensorsEnabled = 0;
static volatile uint8_t AutoInit = 0;
static volatile uint32_t IntCurrentTime1 = 0;
static volatile uint32_t IntCurrentTime2 = 0;
static volatile uint8_t DIL24_INT1_Event = 0;
static uint8_t NewData = 0;
static uint16_t NewDataFlags = 0;
static uint8_t MsgIndex = 0;
static uint32_t LastUpdate = 0;

/* Private function prototypes -----------------------------------------------*/
static void GPIO_Init(void);

static void RTC_Config(void);
static void RTC_TimeStampConfig(void);

static void Enable_Disable_Sensors(void);
static void Float_To_Int(float In, displayFloatToInt_t *OutValue, int32_t DecPrec);

static void Time_Handler(Msg_t *Msg);
static void Temp_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Hum_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Accelero_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Gyro_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Magneto_Sensor_Handler(Msg_t *Msg, uint32_t Instance);
static void Sensors_Interrupt_Handler(Msg_t *Msg);
static void MLC_Handler(Msg_t *Msg);
static void FSM_Handler(Msg_t *Msg);
static void QVAR_Handler(Msg_t *Msg, uint32_t Instance);

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

  /* Pull the INT1 pin low to avoid enabling I3C */
  DIL24_INT1_Force_Low();

  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Initialize GPIO */
  GPIO_Init();

  /* Initialize UART */
  USARTConfig();

  /* Initialize RTC */
  RTC_Config();
  RTC_TimeStampConfig();

  /* Initialize cycle counter */
  DWT_Delay_Init();

  /* Blink with LED */
  BSP_LED_On(LED2);
  HAL_Delay(100);
  BSP_LED_Off(LED2);

  for (;;)
  {
    /* Process incoming messages */
    if (UART_ReceivedMSG((Msg_t *)&msg_cmd) == 1)
    {
      if (msg_cmd.Data[0] == DEV_ADDR)
      {
        (void)HandleMSG((Msg_t *)&msg_cmd);
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

      if ((SensorsEnabled & QVAR_ENABLED) == QVAR_ENABLED)
      {
        QVAR_Handler(&msg_dat, AccInstance);
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
static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Interrupt pins X-NUCLEO-IKS02A1 */
  // ISM330DHCX INT1                     PB10
  // ISM330DHCX INT2                     PB4
  // IIS2DLPC INT2                       PC1
  // DIL24 INT1                          PC0 (initialized separately)
  // IIS2MDC DRDY/IIS2DLPC INT1          PA4
  // IIS2DLPC INT/IIS2MDC DRDY           PB0
  // USER INT1                           PA10

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure GPIO pins : PA4 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure GPIO pins : PB0 PB4 PB5 PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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

  if ((SensorsEnabled & HUMIDITY_SENSOR_ENABLED) == HUMIDITY_SENSOR_ENABLED)
  {
    (void)IKS02A1_ENV_SENSOR_Enable(HumInstance, ENV_HUMIDITY);
  }
  else
  {
    (void)IKS02A1_ENV_SENSOR_Disable(HumInstance, ENV_HUMIDITY);
  }

  if ((SensorsEnabled & TEMPERATURE_SENSOR_ENABLED) == TEMPERATURE_SENSOR_ENABLED)
  {
    (void)IKS02A1_ENV_SENSOR_Enable(TmpInstance, ENV_TEMPERATURE);
  }
  else
  {
    (void)IKS02A1_ENV_SENSOR_Disable(TmpInstance, ENV_TEMPERATURE);
  }
}

/**
 * @brief  Splits a float into two integer values
 * @param  In the float value as input
 * @param  OutValue the pointer to the output integer structure
 * @param  DecPrec the decimal precision to be used
 * @retval None
 */
static void Float_To_Int(float In, displayFloatToInt_t *OutValue, int32_t DecPrec)
{
  if (In >= 0.0f)
  {
    OutValue->sign = 0;
  }
  else
  {
    OutValue->sign = 1;
    In = -In;
  }

  OutValue->out_int = (uint32_t)In;
  In = In - (float)(OutValue->out_int);
  OutValue->out_dec = (uint32_t)trunc(In * pow(10.0f, (float)DecPrec));
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
 * @brief  Handles the TEMPERATURE sensor data getting/sending
 * @param  Msg the TEMPERATURE part of the stream
 * @param  Instance the device instance
 * @retval None
 */
static void Temp_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
{
  float temperature;
  uint8_t status = 0;
  static uint32_t last_time = 0;
  uint32_t time = 0;

  if (Instance == IKS02A1_SHT40AD1B_0)
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
    if (IKS02A1_ENV_SENSOR_Get_DRDY_Status(Instance, ENV_TEMPERATURE, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }
  }

  if (status == 1U)
  {
    NewData++;
    NewDataFlags |= 2U;

    (void)IKS02A1_ENV_SENSOR_GetValue(Instance, ENV_TEMPERATURE, &temperature);

    if (DataLoggerActive != 0U)
    {
      (void)memcpy(&Msg->Data[MsgIndex], (void *)&temperature, 4);
      MsgIndex = MsgIndex + 4;
    }
    else if (AutoInit != 0U)
    {
      displayFloatToInt_t out_value;
      Float_To_Int(temperature, &out_value, 2);
      (void)snprintf(DataOut, MAX_BUF_SIZE, "TEMP: %c%d.%02d\r\n", ((out_value.sign != 0) ? '-' : '+'),
                     (int)out_value.out_int, (int)out_value.out_dec);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
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

  if (Instance == IKS02A1_SHT40AD1B_0)
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
    if (IKS02A1_ENV_SENSOR_Get_DRDY_Status(Instance, ENV_HUMIDITY, &status) != BSP_ERROR_NONE)
    {
      status = 0;
    }
  }

  if (status == 1U)
  {
    NewData++;
    NewDataFlags |= 4U;

    (void)IKS02A1_ENV_SENSOR_GetValue(Instance, ENV_HUMIDITY, &humidity);

    if (DataLoggerActive != 0U)
    {
      (void)memcpy(&Msg->Data[MsgIndex], (void *)&humidity, 4);
      MsgIndex = MsgIndex + 4;
    }
    else if (AutoInit != 0U)
    {
      displayFloatToInt_t out_value;
      Float_To_Int(humidity, &out_value, 2);
      (void)snprintf(DataOut, MAX_BUF_SIZE, "HUM: %d.%02d\r\n", (int)out_value.out_int, (int)out_value.out_dec);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
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
static void Gyro_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
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
static void Magneto_Sensor_Handler(Msg_t *Msg, uint32_t Instance)
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
static void Sensors_Interrupt_Handler(Msg_t *Msg)
{
  uint8_t int_status = 0;
  static uint8_t mem_int_status = 0;

  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) int_status |= (1 << 0); else int_status &= ~(1 << 0);
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET) int_status |= (1 << 1); else int_status &= ~(1 << 1);
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) int_status |= (1 << 2); else int_status &= ~(1 << 2);
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET) int_status |= (1 << 3); else int_status &= ~(1 << 3);

  if (DIL24_INT1_Event == 1)
  {
    DIL24_INT1_Event = 0;
    int_status |= (1 << 4);
  }
  else
  {
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_SET) int_status |= (1 << 4); else int_status &= ~(1 << 4);
  }

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
static void MLC_Handler(Msg_t *Msg)
{
  int32_t i = 0;
  uint8_t new_status = 0;
  uint8_t mlc_status[MLC_STATUS_MAX];
  uint8_t mlc_status_max = 0;
  static uint8_t mem_mlc_status[MLC_STATUS_MAX] = {0};

  if ((AccInstance == IKS02A1_ISM330DHCX_0) && (GyrInstance == IKS02A1_ISM330DHCX_0))
  {
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

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
    mlc_status_max = 8;

#if (MLC_STATUS_MAX < 8)
#error "ERROR: Array index out of bounds!"
#endif

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
  else if ((AccInstance == IKS02A1_ISM330BX_0) && (GyrInstance == IKS02A1_ISM330BX_0))
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330BX_0, ISM330BX_FUNC_CFG_ACCESS, ISM330BX_EMBED_FUNC_MEM_BANK << 7);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_MLC1_SRC, &mlc_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_MLC2_SRC, &mlc_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_MLC3_SRC, &mlc_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_MLC4_SRC, &mlc_status[3]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330BX_0, ISM330BX_FUNC_CFG_ACCESS, ISM330BX_MAIN_MEM_BANK << 7);
  }
  else if (AccInstance == IKS02A1_IIS2DULPX_0)
  {
    mlc_status_max = 4;

#if (MLC_STATUS_MAX < 4)
#error "ERROR: Array index out of bounds!"
#endif

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FUNC_CFG_ACCESS, IIS2DULPX_EMBED_FUNC_MEM_BANK << 7);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_MLC1_SRC, &mlc_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_MLC2_SRC, &mlc_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_MLC3_SRC, &mlc_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_MLC4_SRC, &mlc_status[3]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FUNC_CFG_ACCESS, IIS2DULPX_MAIN_MEM_BANK << 7);
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
    NewDataFlags |= 128U;
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

  if ((AccInstance == IKS02A1_ISM330DHCX_0) && (GyrInstance == IKS02A1_ISM330DHCX_0))
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

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

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330DHCX_0, ISM330DHCX_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if (AccInstance == IKS02A1_IIS2ICLX_0)
  {
    fsm_status_max = 18;

#if (FSM_STATUS_MAX < 18)
#error "ERROR: Array index out of bounds!"
#endif

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

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_STATUS_A_MAINPAGE, &fsm_status[16]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2ICLX_0, IIS2ICLX_FSM_STATUS_B_MAINPAGE, &fsm_status[17]);
  }
  else if ((AccInstance == IKS02A1_ISM330BX_0) && (GyrInstance == IKS02A1_ISM330BX_0))
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330BX_0, ISM330BX_FUNC_CFG_ACCESS, ISM330BX_EMBED_FUNC_MEM_BANK << 7);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_OUTS8, &fsm_status[7]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_ISM330BX_0, ISM330BX_FUNC_CFG_ACCESS, ISM330BX_MAIN_MEM_BANK << 7);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_FSM_STATUS_MAINPAGE, &fsm_status[8]);
  }
  else if (AccInstance == IKS02A1_IIS2DULPX_0)
  {
    fsm_status_max = 9;

#if (FSM_STATUS_MAX < 9)
#error "ERROR: Array index out of bounds!"
#endif

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FUNC_CFG_ACCESS, IIS2DULPX_EMBED_FUNC_MEM_BANK << 7);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS1, &fsm_status[0]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS2, &fsm_status[1]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS3, &fsm_status[2]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS4, &fsm_status[3]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS5, &fsm_status[4]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS6, &fsm_status[5]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS7, &fsm_status[6]);
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_OUTS8, &fsm_status[7]);

    (void)IKS02A1_MOTION_SENSOR_Write_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FUNC_CFG_ACCESS, IIS2DULPX_MAIN_MEM_BANK << 7);

    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_IIS2DULPX_0, IIS2DULPX_FSM_STATUS_MAINPAGE, &fsm_status[8]);
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
    NewDataFlags |= 256U;
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

  if ((AccInstance == IKS02A1_ISM330BX_0) && (GyrInstance == IKS02A1_ISM330BX_0))
  {
    ism330bx_status_reg_t status;
    (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_STATUS_REG, (uint8_t *)&status);
    qvar_data_available = status.ah_qvarda;
  }
  else
  {
    qvar_data_available = 0;
  }

  if (qvar_data_available == 1)
  {
    NewData++;
    NewDataFlags |= 0x400U;

    union {
      int16_t i16bit;
      uint8_t u8bit[2];
    } value;

    float qvar_mv;

    if ((AccInstance == IKS02A1_ISM330BX_0) && (GyrInstance == IKS02A1_ISM330BX_0))
    {
      (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_AH_QVAR_OUT_L, &value.u8bit[0]);
      (void)IKS02A1_MOTION_SENSOR_Read_Register(IKS02A1_ISM330BX_0, ISM330BX_AH_QVAR_OUT_H, &value.u8bit[1]);
      qvar_mv = value.i16bit / ISM330BX_QVAR_GAIN;
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
    else if (AutoInit != 0U)
    {
      displayFloatToInt_t out_value;
      Float_To_Int(qvar_mv, &out_value, 2);
      (void)snprintf(DataOut, MAX_BUF_SIZE, "QVAR: %d.%02d\r\n", (int)out_value.out_int, (int)out_value.out_dec);
      (void)HAL_UART_Transmit(&UartHandle, (uint8_t *)DataOut, (uint16_t)strlen(DataOut), 5000);
    }
    else
    {
      /* Nothing to do */
    }
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

  /*##-2- Configure LSE as RTC clock soucre ###################################*/
  rcc_osc_init_struct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  rcc_osc_init_struct.PLL.PLLState = RCC_PLL_NONE;
  rcc_osc_init_struct.LSEState = RCC_LSE_ON;
  rcc_osc_init_struct.LSIState = RCC_LSI_OFF;
  if (HAL_RCC_OscConfig(&rcc_osc_init_struct) != HAL_OK)
  {
    /* LSE not available, we use LSI */
    UseLSI = 1;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV_LSI;
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV_LSI;
  }
  else
  {
    /* We use LSE */
    UseLSI = 0;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV_LSE;
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV_LSE;
  }
  RtcHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

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

  /*##-3- Configure the Date #################################################*/
  /* Set Date: Monday January 1st 2000 */
  sdatestructure.Year = 0x00;
  sdatestructure.Month = RTC_MONTH_JANUARY;
  sdatestructure.Date = 0x01;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;

  if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-4- Configure the Time #################################################*/
  /* Set Time: 00:00:00 */
  stimestructure.Hours = 0x00;
  stimestructure.Minutes = 0x00;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BCD) != HAL_OK)
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIOPin)
{
  int32_t do_operation = 0;
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
  else if (GPIOPin == GPIO_PIN_0)
  {
    DIL24_INT1_Event = 1;
  }
}

/**
 * @brief  Configures the current time and date
 * @param  hh the hour value to be set
 * @param  mm the minute value to be set
 * @param  ss the second value to be set
 * @retval None
 */
void RTC_TimeRegulate(uint8_t hh, uint8_t mm, uint8_t ss)
{
  RTC_TimeTypeDef stimestructure;

  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.Hours = hh;
  stimestructure.Minutes = mm;
  stimestructure.Seconds = ss;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
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
 * @brief  Reports the name of the source file and the source line number where the assert_param error has occurred
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
