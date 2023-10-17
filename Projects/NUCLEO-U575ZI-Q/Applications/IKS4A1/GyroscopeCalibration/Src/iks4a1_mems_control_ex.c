/**
  ******************************************************************************
  * @file    iks4a1_mems_control_ex.c
  * @author  MEMS Software Solutions Team
  * @brief   This file contains the MEMS sensors interface for X-NUCLEO-IKS4A1
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

#include "iks4a1_mems_control_ex.h"

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
  (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, Reg, Data);
}

/**
  * @brief  Get the register value from gyroscope
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_GYR_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, Reg, Data);
}

/**
  * @brief  Get the register value from magnetometer
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_MAG_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LIS2MDL_0, Reg, Data);
}

/**
  * @brief  Get the register value from pressure sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_PRESS_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS4A1_ENV_SENSOR_Read_Register(IKS4A1_LPS22DF_0, Reg, Data);
}

/**
  * @brief  Get the register value from temperature sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_TEMP_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS4A1_ENV_SENSOR_Read_Register(IKS4A1_STTS22H_0, Reg, Data);
}

/**
  * @brief  Get the register value from humidity sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_HUM_Read_Register(uint8_t Reg, uint8_t *Data)
{
  (void)IKS4A1_ENV_SENSOR_Read_Register(IKS4A1_SHT40AD1B_0, Reg, Data);
}

/**
  * @brief  Set the register value in accelerometer
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_ACC_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS4A1_MOTION_SENSOR_Write_Register(IKS4A1_LSM6DSV16X_0, Reg, Data);
}

/**
  * @brief  Set the register value in gyroscope
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_GYR_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS4A1_MOTION_SENSOR_Write_Register(IKS4A1_LSM6DSV16X_0, Reg, Data);
}

/**
  * @brief  Set the register value in magnetometer
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_MAG_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS4A1_MOTION_SENSOR_Write_Register(IKS4A1_LIS2MDL_0, Reg, Data);
}

/**
  * @brief  Set the register value in pressure sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_PRESS_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS4A1_ENV_SENSOR_Write_Register(IKS4A1_LPS22DF_0, Reg, Data);
}

/**
  * @brief  Set the register value in temperature sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_TEMP_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS4A1_ENV_SENSOR_Write_Register(IKS4A1_STTS22H_0, Reg, Data);
}

/**
  * @brief  Set the register value in humidity sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_HUM_Write_Register(uint8_t Reg, uint8_t Data)
{
  (void)IKS4A1_ENV_SENSOR_Write_Register(IKS4A1_SHT40AD1B_0, Reg, Data);
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
    (void)IKS4A1_MOTION_SENSOR_DRDY_Set_Mode(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_DRDY_PULSED);
  }
  else
  {
    (void)IKS4A1_MOTION_SENSOR_DRDY_Set_Mode(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_DRDY_LATCHED);
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
    (void)IKS4A1_MOTION_SENSOR_DRDY_Set_Mode(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_DRDY_PULSED);
  }
  else
  {
    (void)IKS4A1_MOTION_SENSOR_DRDY_Set_Mode(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_DRDY_LATCHED);
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

  (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_INT1_CTRL, &reg);

  if (Enable == 1)
  {
    reg = reg | 0x01;
  }
  else
  {
    reg = reg & ~0x01;
  }

  (void)IKS4A1_MOTION_SENSOR_Write_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_INT1_CTRL, reg);
}

/**
  * @brief  Enables/Disables DRDY signal from gyroscope
  * @param  Enable Define if DRDY signal is enable or disabled
  * @retval None
  */
void BSP_SENSOR_GYR_SetDRDYInt(uint8_t Enable)
{
  uint8_t reg = 0;

  (void)IKS4A1_MOTION_SENSOR_Read_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_INT1_CTRL, &reg);

  if (Enable == 1)
  {
    reg = reg | 0x02;
  }
  else
  {
    reg = reg & ~0x02;
  }

  (void)IKS4A1_MOTION_SENSOR_Write_Register(IKS4A1_LSM6DSV16X_0, LSM6DSV16X_INT1_CTRL, reg);
}

/**
  * @brief  Get DRDY status from acceleromter
  * @param  Status DRDY bit status
  * @retval None
  */
void BSP_SENSOR_ACC_GetDRDYStatus(uint8_t *Status)
{
  (void)IKS4A1_MOTION_SENSOR_Get_DRDY_Status(IKS4A1_LSM6DSV16X_0, MOTION_ACCELERO, Status);
}

/**
  * @brief  Reads data from FSM
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_ACC_GYR_Read_FSM_Data(uint8_t *Data)
{
  (void)BSP_SENSOR_ACC_Write_Register(LSM6DSV16X_FUNC_CFG_ACCESS, 0x80);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS1, &Data[0]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS2, &Data[1]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS3, &Data[2]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS4, &Data[3]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS5, &Data[4]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS6, &Data[5]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS7, &Data[6]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_FSM_OUTS8, &Data[7]);
  (void)BSP_SENSOR_ACC_Write_Register(LSM6DSV16X_FUNC_CFG_ACCESS, 0x00);
}

/**
  * @brief  Reads data from MLC
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_ACC_GYR_Read_MLC_Data(uint8_t *Data)
{
  (void)BSP_SENSOR_ACC_Write_Register(LSM6DSV16X_FUNC_CFG_ACCESS, 0x80);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_MLC1_SRC, &Data[0]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_MLC2_SRC, &Data[1]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_MLC3_SRC, &Data[2]);
  (void)BSP_SENSOR_ACC_Read_Register(LSM6DSV16X_MLC4_SRC, &Data[3]);
  (void)BSP_SENSOR_ACC_Write_Register(LSM6DSV16X_FUNC_CFG_ACCESS, 0x00);
}
