/**
 ******************************************************************************
 * @file    DemoSerial.c
 * @author  MEMS Software Solutions Team
 * @brief   Handle the Serial Protocol
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed under Software License Agreement
 * SLA0077, (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
 *
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>  /* snprintf */
#include "com.h"
#include "DemoSerial.h"
#include "main.h"
#include "sensor_def.h"
#include "MotionSP.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup VIBRATION_MONITORING VIBRATION MONITORING
 * @{
 */

/* Private defines -----------------------------------------------------------*/
#define ISM330DHCX_UNICLEO_ID       11
#define IIS2MDC_UNICLEO_ID          4

#define FW_ID "20"
#define FW_VERSION "7.1.0"
#define LIB_VERSION "2.2.0"

#if (defined (USE_IKS02A1))
#define EXPANSION_BOARD "IKS02A1"
#else
#error Not supported shield
#endif

#define DATA_TX_LEN  MIN(4, DATABYTE_LEN)

/* Extern variables ----------------------------------------------------------*/
extern sAcceleroParam_t sTimeDomain;
extern sMotionSP_Parameter_t MotionSP_Parameters;
extern float Filter_Params[FFT_SIZE_DEFAULT];
extern volatile uint32_t SensorsEnabled;

/* Private variables ---------------------------------------------------------*/
static uint8_t PresentationString[] = {"MEMS shield demo,"FW_ID","FW_VERSION","LIB_VERSION","EXPANSION_BOARD};
static volatile uint8_t DataStreamingDest = 1;

/* Private function prototypes -----------------------------------------------*/
static uint8_t Handle_SUBCMD(TMsg *Msg);
static void Send_String(TMsg *Msg, uint8_t *String);
static void Send_Avail_Sensor_List(TMsg *Msg, uint8_t *SensorList);
static void Send_Sensor_FS_List(TMsg *Msg, uint32_t *FsList);
static void Send_Sensor_ODR_List(TMsg *Msg, float *OdrList);
static void Send_Samples_List(TMsg *Msg, uint32_t *SamplesList);
static void Send_ErrorMsg(TMsg *Msg);
static uint8_t Get_Actual_FS_Index(uint8_t *Value);
static uint8_t Get_Actual_ODR_Index(uint8_t *Value);
static uint8_t Get_Actual_Samples_Index(uint8_t *Value);
static uint8_t Get_Actual_Sensor_Index(uint8_t *Value);

/* Public functions ---------------------------------------------------------*/
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
  Msg->Data[2] = CMD_SF_Data;
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
/*  DestAddr | SouceAddr | CMD | PAYLOAD
 *      1          1        1       N
 */
{
  int ret = 1;
  uint32_t i;

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

      i = 0;
      while (i < (sizeof(PresentationString) - 1U))
      {
        Msg->Data[3U + i] = PresentationString[i];
        i++;
      }

      Msg->Len = 3U + i;
      UART_SendMsg(Msg);
      break;

    case CMD_ACCELERO_GYRO_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], ISM330DHCX_UNICLEO_ID, 4);
      Msg->Len = 3 + 4;
      UART_SendMsg(Msg);
      break;

    case CMD_MAGNETO_Init:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Serialize_s32(&Msg->Data[3], IIS2MDC_UNICLEO_ID, 4);
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
      if ((SensorsEnabled & ACCELEROMETER_SENSOR) == ACCELEROMETER_SENSOR)
      {
        (void)IKS02A1_MOTION_SENSOR_Enable(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO);
      }

      if (Init_Demo() == 0)
      {
        Msg->Data[2] = 0;
        return 0;
      }

      DataLoggerActive = 1;

      if (!Restart_FIFO())
      {
        Msg->Data[2] = 0;
        return 0;
      }

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

      /* Disable all sensors */
      (void)IKS02A1_MOTION_SENSOR_Disable(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO);

      SensorsEnabled = 0;

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

    case CMD_SUBCMD_SENSOR:
      if (Msg->Len < 3U)
      {
        return 0;
      }

      if (Msg->Data[4] != SUBCMD_SENSOR_ACC)
      {
        return 0;
      }

      ret = Handle_SUBCMD(Msg);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

void SEND_INIT_ERR_MSG(TMsg *Msg)
{
  Msg->Data[0] = 1;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_SUBCMD_SENSOR + CMD_Reply_Add;
  Msg->Data[3] = 0;
  Msg->Len = 4;
  UART_SendMsg(Msg);
}

void SEND_BOARD_RESTARTED_MSG(TMsg *Msg)
{
  Msg->Data[0] = 1;
  Msg->Data[1] = DEV_ADDR;
  Msg->Data[2] = CMD_SUBCMD_SENSOR + CMD_Reply_Add;
  Msg->Data[3] = SUBCMD_RESTART;
  Msg->Len = 4;
  UART_SendMsg(Msg);
}

/* Private functions ---------------------------------------------------------*/
static uint8_t Handle_SUBCMD(TMsg *Msg)
{
  uint8_t ret_val = 1;
  uint8_t *sensor_list;
  uint8_t index;
  static const char window_type_name_list[] = WINDOWS_LIST;

  switch (Msg->Data[3]) /* SUBCMD */
  {
    case SUBCMD_GET_SENSOR_NAME:
      Send_String(Msg, (uint8_t *)SensorSetting.name);
      break;

    case SUBCMD_GET_FS_LIST:
      Send_Sensor_FS_List(Msg, (uint32_t *)SensorSetting.fs_list);
      break;

    case SUBCMD_SET_FS_INDEX:
      if (!Set_Full_Scale(Msg->Data[5]))
      {
        ret_val = 0;
        Msg->Data[5] = 0;
      }
      else
      {
        if (!Get_Actual_FS_Index(&Msg->Data[5]))
        {
          ret_val = 0;
          Msg->Data[5] = 0;
        }
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    case SUBCMD_GET_ODR_LIST:
      Send_Sensor_ODR_List(Msg, (float *)SensorSetting.odr_list);
      break;

    case SUBCMD_SET_ODR_INDEX:
      index = Msg->Data[5];

      if (index == 0)
      {
        Send_ErrorMsg(Msg);
        break;
      }

      if (!Set_ODR(index))
      {
        ret_val = 0;
        Msg->Data[5] = 0;
      }
      else
      {
        if (!Get_Actual_ODR_Index(&Msg->Data[5]))
        {
          ret_val = 0;
          Msg->Data[5] = 0;
        }
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    case SUBCMD_GET_FFT_SAMPLES_LIST:
      Send_Samples_List(Msg, (uint32_t *)SensorSetting.samples_list);
      break;

    case SUBCMD_SET_FFT_SAMPLES:
      if (!Set_Samples(Msg->Data[5]))
      {
        ret_val = 0;
        Msg->Data[5] = 0;
      }
      else
      {
        if (!Get_Actual_Samples_Index(&Msg->Data[5]))
        {
          ret_val = 0;
          Msg->Data[5] = 0;
        }
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    case SUBCMD_SET_HP_FILTER:
      if (!Set_HP_Filter(Msg->Data[5] - 1))
      {
        ret_val = 0;
        Msg->Data[5] = 0;
      }
      else
      {
        Msg->Data[5] = Get_HP_Filter() + 1;
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    case SUBCMD_GET_SENSOR_LIST:
      Send_Avail_Sensor_List(Msg, Get_Sensor_List());
      break;

    case SUBCMD_SET_SENSOR_INDEX:
      sensor_list = Get_Sensor_List();

      if (Msg->Data[5] > sensor_list[0])
      {
        ret_val = 0;
        Msg->Data[5] = 0;
      }
      else
      {
        if (sensor_list[Msg->Data[5]] != IKS02A1_ISM330DHCX_0)
        {

          /* NOTE: Currently done for just one sensor but possible to disable current sensor and enable another one from sensor_list here */

          if (!Get_Actual_Sensor_Index(&Msg->Data[5]))
          {
            ret_val = 0;
            Msg->Data[5] = 0;
          }

          Init_Demo();
        }
        else
        {
          if (!Get_Actual_Sensor_Index(&Msg->Data[5]))
          {
            ret_val = 0;
            Msg->Data[5] = 0;
          }
        }
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    case SUBCMD_CLEAR_PEAK:
      sTimeDomain.AccPeak.AXIS_X = 0.0f;
      sTimeDomain.AccPeak.AXIS_Y = 0.0f;
      sTimeDomain.AccPeak.AXIS_Z = 0.0f;

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    case SUBCMD_GET_WND_LIST:
      Send_String(Msg, (uint8_t *)window_type_name_list);
      break;

    case SUBCMD_SET_WND:
      if (((Msg->Data[5] - 1) >= RECTANGULAR) && ((Msg->Data[5] - 1) <= FLAT_TOP))
      {
        MotionSP_Parameters.window = Msg->Data[5] - 1;
        MotionSP_SetWindFiltArray(Filter_Params, MotionSP_Parameters.FftSize, (Filt_Type_t)MotionSP_Parameters.window);
      }
      else
      {
        ret_val = 0;
        Msg->Data[5] = 0;
      }

      BUILD_REPLY_HEADER(Msg);
      Msg->Len = 6;
      UART_SendMsg(Msg);
      break;

    default:
      ret_val = 0;
      break;
  }

  return ret_val;
}

static uint8_t Get_Actual_FS_Index(uint8_t *Value)
{
  int32_t fullScale;
  uint8_t i;

  if (IKS02A1_MOTION_SENSOR_GetFullScale(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, &fullScale) != BSP_ERROR_NONE)
  {
    return 0;
  }

  for (i = 0; i <= SensorSetting.fs_list[0]; i++)
  {
    if (SensorSetting.fs_list[i] == fullScale)
    {
      *Value = i;
      return 1;
    }
  }

  return 0;
}

static uint8_t Get_Actual_ODR_Index(uint8_t *Value)
{
  float odr;
  uint8_t i;

  if (IKS02A1_MOTION_SENSOR_GetOutputDataRate(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO, &odr) != BSP_ERROR_NONE)
  {
    return 0;
  }

  for (i = 0; i <= SensorSetting.odr_list[0]; i++)
  {
    if (SensorSetting.odr_list[i] == odr)
    {
      *Value = i;
      return 1;
    }
  }

  return 0;
}

static uint8_t Get_Actual_Samples_Index(uint8_t *Value)
{
  uint16_t samples;
  uint8_t i;

  samples = Get_Samples();

  for (i = 1; i <= SensorSetting.samples_list[0]; i++)
  {
    if (samples == SensorSetting.samples_list[i])
    {
      *Value = i;
      return 1;
    }
  }

  return 0;
}

static uint8_t Get_Actual_Sensor_Index(uint8_t *Value)
{
  uint8_t i;
  uint8_t *sensor_list;

  sensor_list = Get_Sensor_List();

  for (i = 1; i <= sensor_list[0]; i++)
  {
    if (IKS02A1_ISM330DHCX_0 == sensor_list[i])
    {
      *Value = i;
      return 1;
    }
  }

  return 0;
}

static void Send_String(TMsg *Msg, uint8_t *String)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  while (i < strlen((char const *)String))
  {
    Msg->Data[5 + i] = String[i];
    i++;
  }

  Msg->Len = 5 + i;
  UART_SendMsg(Msg);
}

static void Send_Avail_Sensor_List(TMsg *Msg, uint8_t *SensorList)
{
  int i = 0;
  char sensorNames[64];
  char tmp[20];
  BUILD_REPLY_HEADER(Msg);

  memset(sensorNames, 0, sizeof(sensorNames));

  for (i = 0; i < SensorList[0]; i++)
  {
    snprintf(tmp, sizeof(tmp), "%s, ", SensorSetting.name);
    strcat(sensorNames, tmp);
  }

  i = 0;

  while (i < strlen((char const *)sensorNames) - 2) // -2 -> delete ", " after last sensor name
  {
    Msg->Data[5 + i] = sensorNames[i];
    i++;
  }

  Msg->Len = 5 + i;
  UART_SendMsg(Msg);
}

static void Send_Sensor_FS_List(TMsg *Msg, uint32_t *FsList)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], FsList[0], 4);

  for (i = 0; i < FsList[0]; i++)
  {
    Serialize(&Msg->Data[9 + i * 4], FsList[i + 1], 4);
  }

  Msg->Len = 9 + i * 4;
  UART_SendMsg(Msg);
}

static void Send_Sensor_ODR_List(TMsg *Msg, float *OdrList)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], (int) OdrList[0], 4);

  for (i = 0; i < OdrList[0]; i++)
  {
    FloatToArray(&Msg->Data[9 + i * 4], OdrList[i + 1]);
  }

  Msg->Len = 9 + i * 4;
  UART_SendMsg(Msg);
}

static void Send_Samples_List(TMsg *Msg, uint32_t *SamplesList)
{
  int i = 0;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], (int) SamplesList[0], 4);

  for (i = 0; i < SamplesList[0]; i++)
  {
    FloatToArray(&Msg->Data[9 + i * 4], SamplesList[i + 1]);
  }

  Msg->Len = 9 + i * 4;
  UART_SendMsg(Msg);
}

static void Send_ErrorMsg(TMsg *Msg)
{
  Msg->Data[5] = 0;
  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 6;
  UART_SendMsg(Msg);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
