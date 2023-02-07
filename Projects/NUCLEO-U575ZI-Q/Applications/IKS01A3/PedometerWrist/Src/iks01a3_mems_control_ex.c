/**
  ******************************************************************************
  * @file    iks01a3_mems_control_ex.c
  * @author  MEMS Application Team
  * @brief   This file contains the MEMS sensors interface for X-NUCLEO-IKS01A3
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

#include "iks01a3_mems_control_ex.h"

/**
  * @brief  Get accelerometer sensor orientation
  * @param  Orientation Pointer to sensor orientation
  * @retval None
  */
void BSP_SENSOR_ACC_GetOrientation(char *Orientation)
{
  Orientation[0] = 's';
  Orientation[1] = 'e';
  Orientation[2] = 'u';
}

/**
  * @brief  Get gyroscope sensor orientation
  * @param  Orientation Pointer to sensor orientation
  * @retval None
  */
void BSP_SENSOR_GYR_GetOrientation(char *Orientation)
{
  Orientation[0] = 's';
  Orientation[1] = 'e';
  Orientation[2] = 'u';
}

/**
  * @brief  Get magnetometer sensor orientation
  * @param  Orientation Pointer to sensor orientation
  * @retval None
  */
void BSP_SENSOR_MAG_GetOrientation(char *Orientation)
{
  Orientation[0] = 'n';
  Orientation[1] = 'e';
  Orientation[2] = 'u';
}

/**
  * @brief  Get the register value from accelerometer
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_ACC_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LSM6DSO_0, Reg, Data);
}

/**
  * @brief  Get the register value from gyroscope
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_GYR_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LSM6DSO_0, Reg, Data);
}

/**
  * @brief  Get the register value from magnetometer
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_MAG_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LIS2MDL_0, Reg, Data);
}

/**
  * @brief  Get the register value from pressure sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_PRESS_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS01A3_ENV_SENSOR_Read_Register(IKS01A3_LPS22HH_0, Reg, Data);
}

/**
  * @brief  Get the register value from temperature sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_TEMP_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS01A3_ENV_SENSOR_Read_Register(IKS01A3_HTS221_0, Reg, Data);
}

/**
  * @brief  Get the register value from humidity sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_HUM_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS01A3_ENV_SENSOR_Read_Register(IKS01A3_HTS221_0, Reg, Data);
}

/**
  * @brief  Set the register value in accelerometer
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_ACC_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, Reg, Data);
}

/**
  * @brief  Set the register value in gyroscope
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_GYR_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, Reg, Data);
}

/**
  * @brief  Set the register value in magnetometer
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_MAG_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LIS2MDL_0, Reg, Data);
}

/**
  * @brief  Set the register value in pressure sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_PRESS_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS01A3_ENV_SENSOR_Write_Register(IKS01A3_LPS22HH_0, Reg, Data);
}

/**
  * @brief  Set the register value in temperature sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_TEMP_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS01A3_ENV_SENSOR_Write_Register(IKS01A3_HTS221_0, Reg, Data);
}

/**
  * @brief  Set the register value in humidity sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_HUM_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS01A3_ENV_SENSOR_Write_Register(IKS01A3_HTS221_0, Reg, Data);
}

/**
  * @brief  Set DRDY interrupt mode for accelerometer
  * @param  Mode Mode to be set (1 means pulsed mode otherwise latched mode)
  * @retval None
  */
void BSP_SENSOR_ACC_SetDRDYMode(uint8_t Mode)
{
  if (Mode == 1)
  {
    (void)IKS01A3_MOTION_SENSOR_DRDY_Set_Mode(IKS01A3_LSM6DSO_0, LSM6DSO_DRDY_PULSED);
  }
  else
  {
    (void)IKS01A3_MOTION_SENSOR_DRDY_Set_Mode(IKS01A3_LSM6DSO_0, LSM6DSO_DRDY_LATCHED);
  }
}

/**
  * @brief  Set DRDY interrupt mode for gyroscope
  * @param  Mode Mode to be set (1 means pulsed mode otherwise latched mode)
  * @retval None
  */
void BSP_SENSOR_GYR_SetDRDYMode(uint8_t Mode)
{
  if (Mode == 1)
  {
    (void)IKS01A3_MOTION_SENSOR_DRDY_Set_Mode(IKS01A3_LSM6DSO_0, LSM6DSO_DRDY_PULSED);
  }
  else
  {
    (void)IKS01A3_MOTION_SENSOR_DRDY_Set_Mode(IKS01A3_LSM6DSO_0, LSM6DSO_DRDY_LATCHED);
  }
}

/**
  * @brief  Enables/Disables DRDY signal from acceleromter
  * @param  Enable Define if DRDY signal is enable or disabled
  * @retval None
  */
void BSP_SENSOR_ACC_SetDRDYInt(uint8_t Enable)
{
  uint8_t reg = 0;

  (void)IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LSM6DSO_0, LSM6DSO_INT1_CTRL, &reg);

  if (Enable == 1)
  {
    reg = reg | 0x01;
  }
  else
  {
    reg = reg & ~0x01;
  }

  (void)IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, LSM6DSO_INT1_CTRL, reg);
}

/**
  * @brief  Enables/Disables DRDY signal from gyroscope
  * @param  Enable Define if DRDY signal is enable or disabled
  * @retval None
  */
void BSP_SENSOR_GYR_SetDRDYInt(uint8_t Enable)
{
  uint8_t reg = 0;

  (void)IKS01A3_MOTION_SENSOR_Read_Register(IKS01A3_LSM6DSO_0, LSM6DSO_INT1_CTRL, &reg);

  if (Enable == 1)
  {
    reg = reg | 0x02;
  }
  else
  {
    reg = reg & ~0x02;
  }

  (void)IKS01A3_MOTION_SENSOR_Write_Register(IKS01A3_LSM6DSO_0, LSM6DSO_INT1_CTRL, reg);
}

/**
  * @brief  Get DRDY status from acceleromter
  * @param  Status DRDY bit status
  * @retval None
  */
void BSP_SENSOR_ACC_GetDRDYStatus(uint8_t *Status)
{
  (void)IKS01A3_MOTION_SENSOR_Get_DRDY_Status(IKS01A3_LSM6DSO_0, MOTION_ACCELERO, Status);
}

/**
  * @brief  Reads data from FSM
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_ACC_GYR_Read_FSM_Data(uint8_t *Data)
{
  (void)BSP_SENSOR_ACC_Write_Register(LSM6DSO_FUNC_CFG_ACCESS, 0x80);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS1, &Data[0]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS2, &Data[1]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS3, &Data[2]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS4, &Data[3]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS5, &Data[4]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS6, &Data[5]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS7, &Data[6]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS8, &Data[7]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS9, &Data[8]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS10, &Data[9]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS11, &Data[10]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS12, &Data[11]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS13, &Data[12]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS14, &Data[13]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS15, &Data[14]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSO_FSM_OUTS16, &Data[15]);
  (void)BSP_SENSOR_ACC_Write_Register(LSM6DSO_FUNC_CFG_ACCESS, 0x00);
}

/**
  * @brief  Reads data from MLC
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_ACC_GYR_Read_MLC_Data(uint8_t *Data)
{
  /* MLC not available on LSM6DSO */
}
