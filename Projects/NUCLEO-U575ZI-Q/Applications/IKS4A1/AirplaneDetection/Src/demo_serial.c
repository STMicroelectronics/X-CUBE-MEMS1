/**
  ******************************************************************************
  * @file    demo_serial.c
  * @author  MEMS Software Solutions Team
  * @brief   Handle the Serial Protocol
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo.h"
#include "com.h"
#include "demo_serial.h"
#include "fw_version.h"
#include "motion_ad_manager.h"

#ifdef USE_CUSTOM_BOARD
#include "custom_mems_conf_app.h"
#endif

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
  * @{
  */

/* Private defines -----------------------------------------------------------*/
#ifndef ACC_GYR_UNICLEO_ID
#define ACC_GYR_UNICLEO_ID UNKNOWN_UNICLEO_ID
#endif

#ifndef MAG_UNICLEO_ID
#define MAG_UNICLEO_ID UNKNOWN_UNICLEO_ID
#endif

#ifndef HUM_TEMP_UNICLEO_ID
#define HUM_TEMP_UNICLEO_ID UNKNOWN_UNICLEO_ID
#endif

#ifndef PRESS_UNICLEO_ID
#define PRESS_UNICLEO_ID UNKNOWN_UNICLEO_ID
#endif

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t DataStreamingDest = 1;

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Build the reply header
  * @param  Msg the pointer to the message to be built
  * @retval None
  */
void BUILD_REPLY_HEADER(TMsg *Msg)
{
  Msg->Data[0] = Msg->Data[1];
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] += CMD_Reply_Add;
}

/**
  * @brief  Initialize the streaming header
  * @param  Msg the pointer to the header to be initialized
  * @retval None
  */
void INIT_STREAMING_HEADER(TMsg *Msg)
{
  Msg->Data[0] = DataStreamingDest;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_Start_Data_Streaming;
  Msg->Len = 3;
}

/**
  * @brief  Initialize the streaming message
  * @param  Msg the pointer to the message to be initialized
  * @retval None
  */
void INIT_STREAMING_MSG(TMsg *Msg)
{
  uint32_t i;

  Msg->Data[0] = DataStreamingDest;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_Start_Data_Streaming;
  for (i = 3; i < STREAMING_MSG_LENGTH + 3; i++)
  {
    Msg->Data[i] = 0;
  }

  Msg->Len = 3;
}

/**
  * @brief  Handle a message
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
int HandleMSG(TMsg *Msg)
/*  DestAddr | SourceAddr | CMD | PAYLOAD
 *      1          1         1       N
 */
{
  int ret = 1;
  uint32_t i;
  char ps[64];
  uint32_t ps_len = 0;
  static uint32_t sensors_enabled_prev = 0;
  int32_t msg_offset;
  uint32_t msg_count;

  if (Msg->Len < 2U)
  {
    return 0;
  }

  if (Msg->Data[0] != DEV_ADDR)
  {
    return 0;
  }

  switch (Msg->Data[2])   /* CMD */
  {
    case CMD_Ping:
      if (Msg->Len != 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      UART_SendMsg(Msg);
      break;

    case CMD_Enter_DFU_Mode:
      if (Msg->Len != 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      break;

    case CMD_Read_PresString:
      if (Msg->Len != 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);

      Get_PresentationString(ps, &ps_len);

      i = 0;
      while (i < ps_len)
      {
        Msg->Data[3U + i] = ps[i];
        i++;
      }

      Msg->Len = 3U + i;
      UART_SendMsg(Msg);
      break;

    case CMD_PRESSURE_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], PRESS_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_HUMIDITY_TEMPERATURE_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], HUM_TEMP_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_ACCELERO_GYRO_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], ACC_GYR_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_MAGNETO_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], MAG_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_Start_Data_Streaming:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      SensorsEnabled = Deserialize(&Msg->Data[3], 4);

      /* Start enabled sensors */
      if ((SensorsEnabled & PRESSURE_SENSOR) == PRESSURE_SENSOR)
      {
        BSP_SENSOR_PRESS_Enable();
      }

      if ((SensorsEnabled & TEMPERATURE_SENSOR) == TEMPERATURE_SENSOR)
      {
        BSP_SENSOR_TEMP_Enable();
      }

      if ((SensorsEnabled & HUMIDITY_SENSOR) == HUMIDITY_SENSOR)
      {
        BSP_SENSOR_HUM_Enable();
      }

      if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
      {
        BSP_SENSOR_ACC_Enable();
      }

      if ((SensorsEnabled & GYROSCOPE_SENSOR) == GYROSCOPE_SENSOR)
      {
        BSP_SENSOR_GYR_Enable();
      }

      if ((SensorsEnabled & MAGNETIC_SENSOR) == MAGNETIC_SENSOR)
      {
        BSP_SENSOR_MAG_Enable();
      }

      (void)HAL_TIM_Base_Start_IT(&BSP_IP_TIM_Handle);
      DataLoggerActive = 1;

      DataStreamingDest = Msg->Data[1];
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      UART_SendMsg(Msg);
      break;

    case CMD_Stop_Data_Streaming:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      DataLoggerActive = 0;
      (void)HAL_TIM_Base_Stop_IT(&BSP_IP_TIM_Handle);

      /* Disable all sensors */
      BSP_SENSOR_ACC_Disable();
      BSP_SENSOR_GYR_Disable();
      BSP_SENSOR_MAG_Disable();
      BSP_SENSOR_PRESS_Disable();
      BSP_SENSOR_TEMP_Disable();
      BSP_SENSOR_HUM_Disable();

      SensorsEnabled = 0;
      UseOfflineData = 0;

      BUILD_REPLY_HEADER(Msg);
      UART_SendMsg(Msg);
      break;

    case CMD_Set_DateTime:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      RTC_TimeRegulate(Msg->Data[3], Msg->Data[4], Msg->Data[5]);
      RTC_DateRegulate(Msg->Data[6], Msg->Data[7], Msg->Data[8], Msg->Data[9]);
      UART_SendMsg(Msg);
      break;

    case CMD_Offline_Data:
      if (Msg->Len < 55U)
      {
        return 0;
      }

      msg_offset = 4;
      msg_count = (uint32_t)Msg->Data[3];

      for (i = 0; i < msg_count; i++)
      {
        memcpy(&OfflineData[OfflineDataWriteIndex].hours, &Msg->Data[msg_offset], 1);
        memcpy(&OfflineData[OfflineDataWriteIndex].minutes, &Msg->Data[msg_offset + 1], 1);
        memcpy(&OfflineData[OfflineDataWriteIndex].seconds, &Msg->Data[msg_offset + 2], 1);
        memcpy(&OfflineData[OfflineDataWriteIndex].subsec, &Msg->Data[msg_offset + 3], 1);

        memcpy(&OfflineData[OfflineDataWriteIndex].pressure, &Msg->Data[msg_offset + 4], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].temperature, &Msg->Data[msg_offset + 8], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].humidity, &Msg->Data[msg_offset + 12], 4);

        memcpy(&OfflineData[OfflineDataWriteIndex].acceleration_x_mg, &Msg->Data[msg_offset + 16], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].acceleration_y_mg, &Msg->Data[msg_offset + 20], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].acceleration_z_mg, &Msg->Data[msg_offset + 24], 4);

        memcpy(&OfflineData[OfflineDataWriteIndex].angular_rate_x_mdps, &Msg->Data[msg_offset + 28], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].angular_rate_y_mdps, &Msg->Data[msg_offset + 32], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].angular_rate_z_mdps, &Msg->Data[msg_offset + 36], 4);

        memcpy(&OfflineData[OfflineDataWriteIndex].magnetic_field_x_mgauss, &Msg->Data[msg_offset + 40], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].magnetic_field_y_mgauss, &Msg->Data[msg_offset + 44], 4);
        memcpy(&OfflineData[OfflineDataWriteIndex].magnetic_field_z_mgauss, &Msg->Data[msg_offset + 48], 4);

        msg_offset += 52;

        OfflineDataCount++;
        if (OfflineDataCount > OFFLINE_DATA_SIZE)
        {
          OfflineDataCount = OFFLINE_DATA_SIZE;
        }

        OfflineDataWriteIndex++;
        if (OfflineDataWriteIndex >= OFFLINE_DATA_SIZE)
        {
          OfflineDataWriteIndex = 0;
        }
      }

      SensorReadRequest = 1;

      /* Mark Msg as read */
      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3;
      break;

    case CMD_Use_Offline_Data:
      if (Msg->Len < 4U)
      {
        return 0;
      }

      if (Msg->Data[3] == 1U)
      {
        UseOfflineData = 1U;
        sensors_enabled_prev = SensorsEnabled;
        SensorsEnabled = 0xFFFFFFFFU;
        (void)HAL_TIM_Base_Stop_IT(&BSP_IP_TIM_Handle);
      }
      else
      {
        UseOfflineData = 0U;
        SensorsEnabled = sensors_enabled_prev;
      }

      BUILD_REPLY_HEADER(Msg);
      UART_SendMsg(Msg);
      break;

    case CMD_Get_App_Info:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      Serialize_s32(&Msg->Data[3], AlgoFreq, 4);
      Serialize_s32(&Msg->Data[7], REQUIRED_DATA, 1);

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 3 + 5;
      UART_SendMsg(Msg);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
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

  if (HAL_RTC_SetDate(&hrtc, &sdatestructure, FORMAT_BIN) != HAL_OK)
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

  stimestructure.Hours          = hh;
  stimestructure.Minutes        = mm;
  stimestructure.Seconds        = ss;
  stimestructure.SubSeconds     = 0;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&hrtc, &stimestructure, FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Gets Presentation string
  * @param  PresentationString the Presentation string
  * @param  Length the length of Presentation string
  * @retval None
  */
void Get_PresentationString(char *PresentationString, uint32_t *Length)
{
  const uint8_t string_pointer_shift = strlen("ST MotionXX v"); /* Shift string pointer by this amount */
  char *lib_version_num;
  char lib_version_string[64];
  int lib_version_len = 0;
  const char ps[] = {"MEMS shield demo,25,"FW_VERSION",%s,"BOARD_NAME};

  MotionAD_manager_get_version(lib_version_string, &lib_version_len);

  /* Shorten library version string (e.g.: ST MotionXX v1.0.0 resp. ST MotionXXX v1.0.0 resp. ST InfraredXX v1.0.0 resp. ST EnvXX v1.0.0) to contain version number only (e.g.: 1.0.0) */
  if (lib_version_len > string_pointer_shift)
  {
    lib_version_num = lib_version_string + string_pointer_shift;
  }
  else
  {
    lib_version_num = lib_version_string;
  }

  *Length = snprintf(PresentationString, 64, ps, lib_version_num);
}

/**
  * @}
  */
