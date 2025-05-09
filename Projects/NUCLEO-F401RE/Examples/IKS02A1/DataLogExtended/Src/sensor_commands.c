/**
  ******************************************************************************
  * @file           : sensor_commands.c
  * @brief          : Handle commands for sensor
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
#include "com.h"
#include "demo_serial.h"
#include "iks02a1_env_sensors.h"
#include "iks02a1_env_sensors_ex.h"
#include "iks02a1_motion_sensors.h"
#include "iks02a1_motion_sensors_ex.h"
#include "nucleo_f401re_errno.h"
#include "sensor_commands.h"
#include "string.h"

/** @addtogroup X_NUCLEO_IKS02A1_Examples X_NUCLEO_IKS02A1 Examples
  * @{
  */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define SENSOR_NAME_MAX_LENGTH  20

/* Private macro -------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
/* Currently selected sensor instances (defaults) */
uint32_t AccInstance = 0xFFFFFFFF;
uint32_t GyrInstance = 0xFFFFFFFF;
uint32_t MagInstance = 0xFFFFFFFF;
uint32_t HumInstance = 0xFFFFFFFF;
uint32_t TmpInstance = 0xFFFFFFFF;

/* Private variables ---------------------------------------------------------*/
/* Supported sensor names. Please verify that second index of array is HIGHER than longest string in array!!! */
static uint8_t AccNameList[][SENSOR_NAME_MAX_LENGTH] = {"ISM330DHCX", "IIS2DLPC", "IIS2ICLX (DIL24)",
                                                        "ASM330LHHX (DIL24)", "ISM330BX (DIL24)", "IIS2DULPX (DIL24)"
                                                       };
static uint8_t GyrNameList[][SENSOR_NAME_MAX_LENGTH] = {"ISM330DHCX", "ASM330LHHX (DIL24)", "ISM330BX (DIL24)"};
static uint8_t MagNameList[][SENSOR_NAME_MAX_LENGTH] = {"IIS2MDC"};
static uint8_t HumNameList[][SENSOR_NAME_MAX_LENGTH] = {"SHT40AD1B (DIL24)"};
static uint8_t TmpNameList[][SENSOR_NAME_MAX_LENGTH] = {"SHT40AD1B (DIL24)"};

/* Supported sensor instances (have to correspond with supported sensor names above) */
static uint32_t AccInstanceList[] =
{
  IKS02A1_ISM330DHCX_0,
  IKS02A1_IIS2DLPC_0,
  IKS02A1_IIS2ICLX_0,
  IKS02A1_ASM330LHHX_0,
  IKS02A1_ISM330BX_0,
  IKS02A1_IIS2DULPX_0,
};
static uint32_t GyrInstanceList[] =
{
  IKS02A1_ISM330DHCX_0,
  IKS02A1_ASM330LHHX_0,
  IKS02A1_ISM330BX_0,
};
static uint32_t MagInstanceList[] =
{
  IKS02A1_IIS2MDC_0,
};
static uint32_t HumInstanceList[] =
{
  IKS02A1_SHT40AD1B_0,
};
static uint32_t TmpInstanceList[] =
{
  IKS02A1_SHT40AD1B_0,
};

/* Sensor fullscale lists (have to correspond with supported sensor names above)
 * Please verify that second index of array is equal to or higher than count of longest sub-array items */
static uint32_t AccFsList[][5] =   /* g */
{
  {4, 2, 4, 8, 16},                /* ISM330DHCX */
  {4, 2, 4, 8, 16},                /* IIS2DLPC */
  {4, 500, 1000, 2000, 3000},      /* IIS2ICLX [mg] */
  {4, 2, 4, 8, 16},                /* ASM330LHHX */
  {4, 2, 4, 8, 16},                /* ISM330BX */
  {4, 2, 4, 8, 16},                /* IIS2DULPX */
};
static uint32_t GyrFsList[][7] =        /* dps */
{
  {6, 125, 250, 500, 1000, 2000, 4000}, /* ISM330DHCX */
  {6, 125, 250, 500, 1000, 2000, 4000}, /* ASM330LHHX */
  {6, 125, 250, 500, 1000, 2000, 4000}, /* ISM330BX */
};
static uint32_t MagFsList[][2] =   /* Ga */
{
  {1, 50},                         /* IIS2MDC */
};
static uint32_t HumFsList[][1] =   /* % */
{
  {0},                             /* SHT40AD1B */
};
static uint32_t TmpFsList[][1] =   /* C */
{
  {0},                             /* SHT40AD1B */
};

/* Sensor output data rate lists (have to correspond with supported sensor names above)
 * Please verify that second index of array is equal to or higher than count of longest sub-array items */
static float AccOdrList[][12] =                                  /* Hz */
{
  {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667},      /* ISM330DHCX */
  {8, 12.5, 25, 50, 100, 200, 400, 800, 1600},                   /* IIS2DLPC */
  {7, 12.5, 26, 52, 104, 208, 416, 833},                         /* IIS2ICLX */
  {11, 1.6, 12.5, 26, 52, 104, 208, 416, 833, 1667, 3333, 6667}, /* ASM330LHHX */
  {11, 1.875, 7.5, 15, 30, 60, 120, 240, 480, 960, 1920, 3840},  /* ISM330BX */
  {10, 1.6, 3, 6, 12.5, 25, 50, 100, 200, 400, 800},             /* IIS2DULPX */
};
static float GyrOdrList[][11] =                             /* Hz */
{
  {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667}, /* ISM330DHCX */
  {10, 12.5, 26, 52, 104, 208, 416, 833, 1667, 3333, 6667}, /* ASM330LHHX */
  {10, 7.5, 15, 30, 60, 120, 240, 480, 960, 1920, 3840},    /* ISM330BX */
};
static float MagOdrList[][5] =   /* Hz */
{
  {4, 10, 20, 50, 100},          /* IIS2MDC */
};
static float HumOdrList[][2] =   /* Hz */
{
  {0},                           /* SHT40AD1B */
};
static float TmpOdrList[][2] =   /* Hz */
{
  {0},                           /* SHT40AD1B */
};

/* Supported sensor names of same kind in one string (comma separated) */
static uint8_t AccNamesString[sizeof(AccNameList) + sizeof(AccNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t GyrNamesString[sizeof(GyrNameList) + sizeof(GyrNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t MagNamesString[sizeof(MagNameList) + sizeof(MagNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t TmpNamesString[sizeof(TmpNameList) + sizeof(TmpNameList) / SENSOR_NAME_MAX_LENGTH];
static uint8_t HumNamesString[sizeof(HumNameList) + sizeof(HumNameList) / SENSOR_NAME_MAX_LENGTH];

/* Currently selected sensor indexes */
static uint8_t AccIndex;
static uint8_t GyrIndex;
static uint8_t MagIndex;
static uint8_t HumIndex;
static uint8_t TmpIndex;

/* Private function prototypes -----------------------------------------------*/
static int32_t SC_Get_Sensor_Name(Msg_t *Msg);
static int32_t SC_Read_Register(Msg_t *Msg);
static int32_t SC_Write_Register(Msg_t *Msg);
static int32_t SC_Get_Full_Scale_List(Msg_t *Msg);
static int32_t SC_Set_Full_Scale(Msg_t *Msg);
static int32_t SC_Get_Full_Scale(Msg_t *Msg);
static int32_t SC_Get_ODR_List(Msg_t *Msg);
static int32_t SC_Set_ODR(Msg_t *Msg);
static int32_t SC_Get_ODR(Msg_t *Msg);
static int32_t SC_Get_Sensor_List(Msg_t *Msg);
static int32_t SC_Set_Sensor_Index(Msg_t *Msg);
static int32_t SC_Write_Multi_Register(Msg_t *Msg);
static int32_t SC_Read_Multi_Register(Msg_t *Msg);
static int32_t SC_Read_Modify_Write_Register(Msg_t *Msg);

static void Send_Sensor_Name(Msg_t *Msg, uint8_t *SensorName);
static void Send_Sensor_FS_List(Msg_t *Msg, uint32_t *FsList);
static void Send_Sensor_ODR_List(Msg_t *Msg, float *OdrList);

static int32_t Is_DIL24_Sensor(uint8_t *SensorName);
static void DIL24_INT1_Init(void);

static void FloatToArray(uint8_t *Dest, float data);
static void ArrayToFloat(uint8_t *Source, float *data);

/* Public functions ----------------------------------------------------------*/
/**
  * @brief  Handle Sensors command
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
int32_t Handle_Sensor_command(Msg_t *Msg)
{
  int32_t ret;

  /* Commands */
  switch (Msg->Data[3])
  {
    case SC_GET_SENSOR_NAME:
      ret = SC_Get_Sensor_Name(Msg);
      break;

    case SC_READ_REGISTER:
      ret = SC_Read_Register(Msg);
      break;

    case SC_WRITE_REGISTER:
      ret = SC_Write_Register(Msg);
      break;

    case SC_GET_FULL_SCALE_LIST:
      ret = SC_Get_Full_Scale_List(Msg);
      break;

    case SC_SET_FULL_SCALE:
      ret = SC_Set_Full_Scale(Msg);
      break;

    case SC_GET_FULL_SCALE:
      ret = SC_Get_Full_Scale(Msg);
      break;

    case SC_GET_ODR_LIST:
      ret = SC_Get_ODR_List(Msg);
      break;

    case SC_SET_ODR:
      ret = SC_Set_ODR(Msg);
      break;

    case SC_GET_ODR:
      ret = SC_Get_ODR(Msg);
      break;

    case SC_GET_SENSOR_LIST:
      ret = SC_Get_Sensor_List(Msg);
      break;

    case SC_SET_SENSOR_INDEX:
      ret = SC_Set_Sensor_Index(Msg);
      break;

    case SC_WRITE_MULTI_REGISTER:
      ret = SC_Write_Multi_Register(Msg);
      break;

    case SC_READ_MULTI_REGISTER:
      ret = SC_Read_Multi_Register(Msg);
      break;

    case SC_READ_MODIFY_WRITE_REGISTER:
      ret = SC_Read_Modify_Write_Register(Msg);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Get sensor name
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Get_Sensor_Name(Msg_t *Msg)
{
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      Send_Sensor_Name(Msg, AccNameList[AccIndex]);
      break;

    case SC_GYROSCOPE:
      Send_Sensor_Name(Msg, GyrNameList[GyrIndex]);
      break;

    case SC_MAGNETOMETER:
      Send_Sensor_Name(Msg, MagNameList[MagIndex]);
      break;

    case SC_TEMPERATURE:
      Send_Sensor_Name(Msg, TmpNameList[TmpIndex]);
      break;

    case SC_HUMIDITY:
      Send_Sensor_Name(Msg, HumNameList[HumIndex]);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
  * @brief  Read sensor register
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Read_Register(Msg_t *Msg)
{
  uint8_t reg_value = 0U;
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_Read_Register(AccInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_Read_Register(GyrInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_Read_Register(MagInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IKS02A1_ENV_SENSOR_Read_Register(TmpInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_HUMIDITY:
      if (IKS02A1_ENV_SENSOR_Read_Register(HumInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Data[6] = reg_value;
  Msg->Len = 7U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Write to sensor register
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Write_Register(Msg_t *Msg)
{
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_Write_Register(AccInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_Write_Register(GyrInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_Write_Register(MagInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IKS02A1_ENV_SENSOR_Write_Register(TmpInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_HUMIDITY:
      if (IKS02A1_ENV_SENSOR_Write_Register(HumInstance, Msg->Data[5], Msg->Data[6]) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 7U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Get sensor full scale list
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Get_Full_Scale_List(Msg_t *Msg)
{
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      Send_Sensor_FS_List(Msg, AccFsList[AccIndex]);
      break;

    case SC_GYROSCOPE:
      Send_Sensor_FS_List(Msg, GyrFsList[GyrIndex]);
      break;

    case SC_MAGNETOMETER:
      Send_Sensor_FS_List(Msg, MagFsList[MagIndex]);
      break;

    case SC_TEMPERATURE:
      Send_Sensor_FS_List(Msg, TmpFsList[TmpIndex]);
      break;

    case SC_HUMIDITY:
      Send_Sensor_FS_List(Msg, HumFsList[HumIndex]);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
  * @brief  Set sensor full scale
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Set_Full_Scale(Msg_t *Msg)
{
  uint32_t full_scale = Deserialize(&Msg->Data[5], 4);
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_SetFullScale(AccInstance, MOTION_ACCELERO, (int32_t)full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_SetFullScale(GyrInstance, MOTION_GYRO, (int32_t)full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_SetFullScale(MagInstance, MOTION_MAGNETO, (int32_t)full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
    case SC_HUMIDITY:
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 9U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Get sensor full scale
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Get_Full_Scale(Msg_t *Msg)
{
  int32_t full_scale = 0;
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_GetFullScale(AccInstance, MOTION_ACCELERO, &full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_GetFullScale(GyrInstance, MOTION_GYRO, &full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_GetFullScale(MagInstance, MOTION_MAGNETO, &full_scale) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
    case SC_HUMIDITY:
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Serialize(&Msg->Data[5], full_scale, 4);
  Msg->Len = 9U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Get sensor output data rate list
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Get_ODR_List(Msg_t *Msg)
{
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      Send_Sensor_ODR_List(Msg, AccOdrList[AccIndex]);
      break;

    case SC_GYROSCOPE:
      Send_Sensor_ODR_List(Msg, GyrOdrList[GyrIndex]);
      break;

    case SC_MAGNETOMETER:
      Send_Sensor_ODR_List(Msg, MagOdrList[MagIndex]);
      break;

    case SC_TEMPERATURE:
      Send_Sensor_ODR_List(Msg, TmpOdrList[TmpIndex]);
      break;

    case SC_HUMIDITY:
      Send_Sensor_ODR_List(Msg, HumOdrList[HumIndex]);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
  * @brief  Set sensor output data rate
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Set_ODR(Msg_t *Msg)
{
  float odr = 0.0f;
  int32_t ret = 1;

  ArrayToFloat(&Msg->Data[5], &odr);

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_SetOutputDataRate(AccInstance, MOTION_ACCELERO, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_SetOutputDataRate(GyrInstance, MOTION_GYRO, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_SetOutputDataRate(MagInstance, MOTION_MAGNETO, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IKS02A1_ENV_SENSOR_SetOutputDataRate(TmpInstance, ENV_TEMPERATURE, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_HUMIDITY:
      if (IKS02A1_ENV_SENSOR_SetOutputDataRate(HumInstance, ENV_HUMIDITY, odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 9U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Get sensor output data rate
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Get_ODR(Msg_t *Msg)
{
  float odr = 0.0f;
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_GetOutputDataRate(AccInstance, MOTION_ACCELERO, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_GetOutputDataRate(GyrInstance, MOTION_GYRO, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_GetOutputDataRate(MagInstance, MOTION_MAGNETO, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IKS02A1_ENV_SENSOR_GetOutputDataRate(TmpInstance, ENV_TEMPERATURE, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    case SC_HUMIDITY:
      if (IKS02A1_ENV_SENSOR_GetOutputDataRate(HumInstance, ENV_HUMIDITY, &odr) != BSP_ERROR_NONE)
      {
        return 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    return ret;
  }

  BUILD_REPLY_HEADER(Msg);
  FloatToArray(&Msg->Data[5], odr);
  Msg->Len = 9U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Get sensor list
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Get_Sensor_List(Msg_t *Msg)
{
  int32_t ret = 1;
  int32_t i;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(AccNamesString); i++)
      {
        AccNamesString[i] = '\0';
      }
      (void)strcat((char *)AccNamesString, (char *)AccNameList[0]);
      for (i = 1; i < sizeof(AccNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)AccNamesString, ",");
        (void)strcat((char *)AccNamesString, (char *)AccNameList[i]);
      }
      Send_Sensor_Name(Msg, AccNamesString);
      break;

    case SC_GYROSCOPE:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(GyrNamesString); i++)
      {
        GyrNamesString[i] = '\0';
      }
      (void)strcat((char *)GyrNamesString, (char *)GyrNameList[0]);
      for (i = 1; i < sizeof(GyrNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)GyrNamesString, ",");
        (void)strcat((char *)GyrNamesString, (char *)GyrNameList[i]);
      }
      Send_Sensor_Name(Msg, GyrNamesString);
      break;

    case SC_MAGNETOMETER:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(MagNamesString); i++)
      {
        MagNamesString[i] = '\0';
      }
      (void)strcat((char *)MagNamesString, (char *)MagNameList[0]);
      for (i = 1; i < sizeof(MagNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)MagNamesString, ",");
        (void)strcat((char *)MagNamesString, (char *)MagNameList[i]);
      }
      Send_Sensor_Name(Msg, MagNamesString);
      break;

    case SC_TEMPERATURE:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(TmpNamesString); i++)
      {
        TmpNamesString[i] = '\0';
      }
      (void)strcat((char *)TmpNamesString, (char *)TmpNameList[0]);
      for (i = 1; i < sizeof(TmpNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)TmpNamesString, ",");
        (void)strcat((char *)TmpNamesString, (char *)TmpNameList[i]);
      }
      Send_Sensor_Name(Msg, TmpNamesString);
      break;

    case SC_HUMIDITY:
      /* Concatenate all sensor names of same kind to one string (comma separated) */
      for (i = 0; i < sizeof(HumNamesString); i++)
      {
        HumNamesString[i] = '\0';
      }
      (void)strcat((char *)HumNamesString, (char *)HumNameList[0]);
      for (i = 1; i < sizeof(HumNameList) / SENSOR_NAME_MAX_LENGTH; i++)
      {
        (void)strcat((char *)HumNamesString, ",");
        (void)strcat((char *)HumNamesString, (char *)HumNameList[i]);
      }
      Send_Sensor_Name(Msg, HumNamesString);
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}

/**
  * @brief  Set sensor index
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Set_Sensor_Index(Msg_t *Msg)
{
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      AccIndex = Msg->Data[5];
      if (AccInstance != AccInstanceList[AccIndex])
      {
        if (AccInstance != 0xFFFFFFFF)
        {
          if (IKS02A1_MOTION_SENSOR_Disable(AccInstance, MOTION_ACCELERO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (IKS02A1_MOTION_SENSOR_Init(AccInstanceList[AccIndex], MOTION_ACCELERO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (ret != 0 && Is_DIL24_Sensor(&AccNameList[AccIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        if (ret != 0)
        {
          AccInstance = AccInstanceList[AccIndex];
        }
      }
      break;

    case SC_GYROSCOPE:
      GyrIndex = Msg->Data[5];
      if (GyrInstance != GyrInstanceList[GyrIndex])
      {
        if (GyrInstance != 0xFFFFFFFF)
        {
          if (IKS02A1_MOTION_SENSOR_Disable(GyrInstance, MOTION_GYRO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (IKS02A1_MOTION_SENSOR_Init(GyrInstanceList[GyrIndex], MOTION_GYRO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (ret != 0 && Is_DIL24_Sensor(&GyrNameList[GyrIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        if (ret != 0)
        {
          GyrInstance = GyrInstanceList[GyrIndex];
        }
      }
      break;

    case SC_MAGNETOMETER:
      MagIndex = Msg->Data[5];
      if (MagInstance != MagInstanceList[MagIndex])
      {
        if (MagInstance != 0xFFFFFFFF)
        {
          if (IKS02A1_MOTION_SENSOR_Disable(MagInstance, MOTION_MAGNETO) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (IKS02A1_MOTION_SENSOR_Init(MagInstanceList[MagIndex], MOTION_MAGNETO) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (ret != 0 && Is_DIL24_Sensor(&MagNameList[MagIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        if (ret != 0)
        {
          MagInstance = MagInstanceList[MagIndex];
        }
      }
      break;

    case SC_TEMPERATURE:
      TmpIndex = Msg->Data[5];
      if ((TmpInstance != TmpInstanceList[TmpIndex]))
      {
        if (TmpInstance != 0xFFFFFFFF)
        {
          if (IKS02A1_ENV_SENSOR_Disable(TmpInstance, ENV_TEMPERATURE) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (IKS02A1_ENV_SENSOR_Init(TmpInstanceList[TmpIndex], ENV_TEMPERATURE) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (ret != 0 && Is_DIL24_Sensor(&TmpNameList[TmpIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        if (ret != 0)
        {
          TmpInstance = TmpInstanceList[TmpIndex];
        }
      }
      break;

    case SC_HUMIDITY:
      HumIndex = Msg->Data[5];
      if (HumInstance != HumInstanceList[HumIndex])
      {
        if (HumInstance != 0xFFFFFFFF)
        {
          if (IKS02A1_ENV_SENSOR_Disable(HumInstance, ENV_HUMIDITY) != BSP_ERROR_NONE)
          {
            ret = 0;
          }
        }
        if (IKS02A1_ENV_SENSOR_Init(HumInstanceList[HumIndex], ENV_HUMIDITY) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
        if (ret != 0 && Is_DIL24_Sensor(&HumNameList[HumIndex][0]) == 1)
        {
          DIL24_INT1_Init();
        }
        if (ret != 0)
        {
          HumInstance = HumInstanceList[HumIndex];
        }
      }
      break;

    default:
      ret = 0;
      break;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Data[5U] = ret;
  Msg->Len = 6U;
  UART_SendMsg(Msg);

  return ret;
}

/**
  * @brief  Write to sensor registers
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Write_Multi_Register(Msg_t *Msg)
{
  int32_t ret = 1;
  uint8_t i;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_MOTION_SENSOR_Write_Register(AccInstance, Msg->Data[6U + (2U * i)],
                                                 Msg->Data[7U + (2U * i)]) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_MOTION_SENSOR_Write_Register(GyrInstance, Msg->Data[6U + (2U * i)],
                                                 Msg->Data[7U + (2U * i)]) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_MAGNETOMETER:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_MOTION_SENSOR_Write_Register(MagInstance, Msg->Data[6U + (2U * i)],
                                                 Msg->Data[7U + (2U * i)]) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_TEMPERATURE:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_ENV_SENSOR_Write_Register(TmpInstance, Msg->Data[6U + (2U * i)],
                                              Msg->Data[7U + (2U * i)]) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_HUMIDITY:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_ENV_SENSOR_Write_Register(HumInstance, Msg->Data[6U + (2U * i)],
                                              Msg->Data[7U + (2U * i)]) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    Msg->Data[5] = 0U;
  }

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 6U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Read sensor registers
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Read_Multi_Register(Msg_t *Msg)
{
  int32_t ret = 1;
  uint8_t i;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_MOTION_SENSOR_Read_Register(AccInstance, Msg->Data[6U + (2U * i)],
                                                &(Msg->Data[7U + (2U * i)])) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_GYROSCOPE:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_MOTION_SENSOR_Read_Register(GyrInstance, Msg->Data[6U + (2U * i)],
                                                &(Msg->Data[7U + (2U * i)])) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_MAGNETOMETER:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_MOTION_SENSOR_Read_Register(MagInstance, Msg->Data[6U + (2U * i)],
                                                &(Msg->Data[7U + (2U * i)])) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_TEMPERATURE:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_ENV_SENSOR_Read_Register(TmpInstance, Msg->Data[6U + (2U * i)],
                                             &(Msg->Data[7U + (2U * i)])) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    case SC_HUMIDITY:
      for (i = 0; i < Msg->Data[5]; i++)
      {
        if (IKS02A1_ENV_SENSOR_Read_Register(HumInstance, Msg->Data[6U + (2U * i)],
                                             &(Msg->Data[7U + (2U * i)])) != BSP_ERROR_NONE)
        {
          ret = 0;
        }
      }
      break;

    default:
      ret = 0;
      break;
  }

  if (ret == 0)
  {
    Msg->Data[5] = 0U;
    Msg->Len = 6U;
  }
  else
  {
    Msg->Len = 8U + (2U * i);
  }

  BUILD_REPLY_HEADER(Msg);
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Read-modify-write sensor register
  * @param  Msg the pointer to the message to be handled
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static int32_t SC_Read_Modify_Write_Register(Msg_t *Msg)
{
  uint8_t reg_value = 0U;
  int32_t ret = 1;

  /* Sensor Type */
  switch (Msg->Data[4])
  {
    case SC_ACCELEROMETER:
      if (IKS02A1_MOTION_SENSOR_Read_Register(AccInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      reg_value = reg_value & Msg->Data[6];
      reg_value = reg_value | Msg->Data[7];
      if (IKS02A1_MOTION_SENSOR_Write_Register(AccInstance, Msg->Data[5], reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      break;

    case SC_GYROSCOPE:
      if (IKS02A1_MOTION_SENSOR_Read_Register(GyrInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      reg_value = reg_value & Msg->Data[6];
      reg_value = reg_value | Msg->Data[7];
      if (IKS02A1_MOTION_SENSOR_Write_Register(GyrInstance, Msg->Data[5], reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      break;

    case SC_MAGNETOMETER:
      if (IKS02A1_MOTION_SENSOR_Read_Register(MagInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      reg_value = reg_value & Msg->Data[6];
      reg_value = reg_value | Msg->Data[7];
      if (IKS02A1_MOTION_SENSOR_Write_Register(MagInstance, Msg->Data[5], reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      break;

    case SC_TEMPERATURE:
      if (IKS02A1_ENV_SENSOR_Read_Register(TmpInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      reg_value = reg_value & Msg->Data[6];
      reg_value = reg_value | Msg->Data[7];
      if (IKS02A1_ENV_SENSOR_Write_Register(TmpInstance, Msg->Data[5], reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      break;

    case SC_HUMIDITY:
      if (IKS02A1_ENV_SENSOR_Read_Register(HumInstance, Msg->Data[5], &reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      reg_value = reg_value & Msg->Data[6];
      reg_value = reg_value | Msg->Data[7];
      if (IKS02A1_ENV_SENSOR_Write_Register(HumInstance, Msg->Data[5], reg_value) != BSP_ERROR_NONE)
      {
        ret = 0;
      }
      break;

    default:
      ret = 0;
      break;
  }

  Msg->Data[6] = reg_value;
  Msg->Data[7] = ret;

  BUILD_REPLY_HEADER(Msg);
  Msg->Len = 8U;
  UART_SendMsg(Msg);
  return ret;
}

/**
  * @brief  Send sensor name
  * @param  Msg the pointer to the message to be handled
  * @param  SensorName the sensor name
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static void Send_Sensor_Name(Msg_t *Msg, uint8_t *SensorName)
{
  uint32_t i = 0U;
  BUILD_REPLY_HEADER(Msg);

  while (i < strlen((char const *)SensorName))
  {
    Msg->Data[5U + i] = SensorName[i];
    i++;
  }

  Msg->Len = 5U + i;
  UART_SendMsg(Msg);
}

/**
  * @brief  Send sensor full scale list
  * @param  Msg the pointer to the message to be handled
  * @param  FsList the sensor full scale list
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static void Send_Sensor_FS_List(Msg_t *Msg, uint32_t *FsList)
{
  uint32_t i;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], FsList[0], 4);

  for (i = 0; i < FsList[0]; i++)
  {
    Serialize(&Msg->Data[9U + i * 4U], FsList[i + 1U], 4U);
  }

  Msg->Len = 9U + i * 4U;
  UART_SendMsg(Msg);
}

/**
  * @brief  Send sensor output data rate list
  * @param  Msg the pointer to the message to be handled
  * @param  OdrList the sensor output data rate list
  * @retval 1 if the message is correctly handled, 0 otherwise
  */
static void Send_Sensor_ODR_List(Msg_t *Msg, float *OdrList)
{
  uint32_t i;
  BUILD_REPLY_HEADER(Msg);

  Serialize(&Msg->Data[5], (uint32_t)OdrList[0], 4);

  for (i = 0; i < (uint32_t)OdrList[0]; i++)
  {
    FloatToArray(&Msg->Data[9U + i * 4U], OdrList[i + 1U]);
  }

  Msg->Len = 9U + i * 4U;
  UART_SendMsg(Msg);
}

/**
  * @brief  Check if sensor is a DIL24 sensor
  * @param  SensorName the pointer to sensor name string
  * @retval 1 if sensor is a DIL24 sensor, 0 otherwise
  */
static int32_t Is_DIL24_Sensor(uint8_t *SensorName)
{
  uint8_t string[] = " (DIL24)";
  size_t size = strlen((const char *)string);
  uint8_t *suffix = SensorName + strlen((const char *)SensorName) - size;

  if (suffix > SensorName)
  {
    return (strncmp((const char *)suffix, (const char *)string, size) == 0) ? 1 : 0;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Configure DIL24 INT1 pin
  * @param  None
  * @retval None
  */
static void DIL24_INT1_Init(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin  = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Enable and set EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief  Copy float data to array
  * @param  Dest the pointer to the destination
  * @param  data the source data
  * @retval None
  */
static void FloatToArray(uint8_t *Dest, float data)
{
  (void)memcpy(Dest, (void *) &data, 4);
}

/**
  * @brief  Copy float data from array
  * @param  Source the pointer to the source data
  * @param  data the pointer to the destination
  * @retval None
  */
static void ArrayToFloat(uint8_t *Source, float *data)
{
  (void)memcpy((void *) data, Source, 4);
}

/**
  * @}
  */

/**
  * @}
  */
