/**
  ******************************************************************************
  * @file    iks4a1_motion_sensors_ex.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of extended functions needed to manage the motion sensors
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
#include "iks4a1_motion_sensors_ex.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @defgroup IKS4A1_MOTION_SENSOR_EX IKS4A1 MOTION SENSOR EX
  * @{
  */

/** @defgroup IKS4A1_MOTION_SENSOR_EX_Imported_Variables IKS4A1 MOTION SENSOR EX Imported Variables
  * @{
  */

extern void *MotionCompObj[IKS4A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS4A1_MOTION_SENSOR_EX_Exported_Functions IKS4A1 MOTION SENSOR EX Exported Functions
  * @{
  */

/**
  * @brief  Get the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS4A1_LSM6DSO_0:
      if (LSM6DSO_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS4A1_LIS2DW12_0:
      if (LIS2DW12_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS4A1_LIS2MDL_0:
      if (LIS2MDL_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS4A1_ASM330LHH_0:
      if (ASM330LHH_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS4A1_IIS2DLPC_0:
      if (IIS2DLPC_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS4A1_IIS2MDC_0:
      if (IIS2MDC_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS4A1_ISM303DAC_ACC_0:
      if (ISM303DAC_ACC_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS4A1_ISM303DAC_MAG_0:
      if (ISM303DAC_MAG_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS4A1_ISM330DLC_0:
      if (ISM330DLC_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS4A1_LIS2DH12_0:
      if (LIS2DH12_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS4A1_LSM6DSOX_0:
      if (LSM6DSOX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS4A1_AIS2DW12_0:
      if (AIS2DW12_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS4A1_LIS3MDL_0:
      if (LIS3MDL_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS4A1_LSM6DSR_0:
      if (LSM6DSR_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS4A1_A3G4250D_0:
      if (A3G4250D_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS4A1_AIS328DQ_0:
      if (AIS328DQ_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS4A1_AIS3624DQ_0:
      if (AIS3624DQ_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS4A1_H3LIS331DL_0:
      if (H3LIS331DL_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS4A1_LSM6DSRX_0:
      if (LSM6DSRX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS4A1_ISM330DHCX_0:
      if (ISM330DHCX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS4A1_LSM6DSO32_0:
      if (LSM6DSO32_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS4A1_IIS2ICLX_0:
      if (IIS2ICLX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS4A1_AIS2IH_0:
      if (AIS2IH_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS4A1_LSM6DSO32X_0:
      if (LSM6DSO32X_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
    case IKS4A1_LIS2DU12_0:
      if (LIS2DU12_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS4A1_ASM330LHHX_0:
      if (ASM330LHHX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
    case IKS4A1_LSM6DSV16BX_0:
      if (LSM6DSV16BX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
    case IKS4A1_LSM6DSV_0:
      if (LSM6DSV_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
    case IKS4A1_LSM6DSV16B_0:
      if (LSM6DSV16B_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
    case IKS4A1_LIS2DUX12_0:
      if (LIS2DUX12_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
    case IKS4A1_LIS2DUXS12_0:
      if (LIS2DUXS12_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
    case IKS4A1_LSM6DSV32X_0:
      if (LSM6DSV32X_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
    case IKS4A1_LSM6DSO16IS_0:
      if (LSM6DSO16IS_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS4A1_LSM6DSO_0:
      if (LSM6DSO_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS4A1_LIS2DW12_0:
      if (LIS2DW12_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS4A1_LIS2MDL_0:
      if (LIS2MDL_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS4A1_ASM330LHH_0:
      if (ASM330LHH_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS4A1_IIS2DLPC_0:
      if (IIS2DLPC_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS4A1_IIS2MDC_0:
      if (IIS2MDC_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS4A1_ISM303DAC_ACC_0:
      if (ISM303DAC_ACC_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS4A1_ISM303DAC_MAG_0:
      if (ISM303DAC_MAG_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS4A1_ISM330DLC_0:
      if (ISM330DLC_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS4A1_LIS2DH12_0:
      if (LIS2DH12_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS4A1_LSM6DSOX_0:
      if (LSM6DSOX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS4A1_AIS2DW12_0:
      if (AIS2DW12_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS4A1_LIS3MDL_0:
      if (LIS3MDL_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS4A1_LSM6DSR_0:
      if (LSM6DSR_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS4A1_A3G4250D_0:
      if (A3G4250D_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS4A1_AIS328DQ_0:
      if (AIS328DQ_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS4A1_AIS3624DQ_0:
      if (AIS3624DQ_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS4A1_H3LIS331DL_0:
      if (H3LIS331DL_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS4A1_LSM6DSRX_0:
      if (LSM6DSRX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS4A1_ISM330DHCX_0:
      if (ISM330DHCX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS4A1_LSM6DSO32_0:
      if (LSM6DSO32_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS4A1_IIS2ICLX_0:
      if (IIS2ICLX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS4A1_AIS2IH_0:
      if (AIS2IH_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS4A1_LSM6DSO32X_0:
      if (LSM6DSO32X_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
    case IKS4A1_LIS2DU12_0:
      if (LIS2DU12_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS4A1_ASM330LHHX_0:
      if (ASM330LHHX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
    case IKS4A1_LSM6DSV16BX_0:
      if (LSM6DSV16BX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
    case IKS4A1_LSM6DSV_0:
      if (LSM6DSV_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
    case IKS4A1_LSM6DSV16B_0:
      if (LSM6DSV16B_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
    case IKS4A1_LIS2DUX12_0:
      if (LIS2DUX12_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
    case IKS4A1_LIS2DUXS12_0:
      if (LIS2DUXS12_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
    case IKS4A1_LSM6DSV32X_0:
      if (LSM6DSV32X_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
    case IKS4A1_LSM6DSO16IS_0:
      if (LSM6DSO16IS_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the status of data ready bit
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Status the pointer to the status
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS4A1_LSM6DSO_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSO_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS4A1_LIS2DW12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LIS2DW12_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS4A1_LIS2MDL_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (LIS2MDL_MAG_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS4A1_ASM330LHH_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ASM330LHH_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ASM330LHH_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS4A1_IIS2DLPC_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IIS2DLPC_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS4A1_IIS2MDC_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (IIS2MDC_MAG_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS4A1_ISM303DAC_ACC_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM303DAC_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS4A1_ISM303DAC_MAG_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (ISM303DAC_MAG_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS4A1_ISM330DLC_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330DLC_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM330DLC_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS4A1_LIS2DH12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LIS2DH12_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS4A1_LSM6DSOX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSOX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSOX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS4A1_AIS2DW12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (AIS2DW12_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS4A1_LIS3MDL_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (LIS3MDL_MAG_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS4A1_LSM6DSR_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSR_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSR_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS4A1_A3G4250D_0:
      if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (A3G4250D_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS4A1_AIS328DQ_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (AIS328DQ_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS4A1_AIS3624DQ_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (AIS3624DQ_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS4A1_H3LIS331DL_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (H3LIS331DL_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS4A1_LSM6DSRX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSRX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSRX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS4A1_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330DHCX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM330DHCX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS4A1_LSM6DSO32_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO32_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSO32_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS4A1_IIS2ICLX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IIS2ICLX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS4A1_AIS2IH_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (AIS2IH_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS4A1_LSM6DSO32X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO32X_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSO32X_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
    case IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (LIS2MDL_MAG_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
    case IKS4A1_LIS2DU12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LIS2DU12_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS4A1_ASM330LHHX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ASM330LHHX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ASM330LHHX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV16X_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV16X_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
    case IKS4A1_LSM6DSV16BX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV16BX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV16BX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
    case IKS4A1_LSM6DSV_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
    case IKS4A1_LSM6DSV16B_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV16B_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV16B_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
    case IKS4A1_LIS2DUX12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LIS2DUX12_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
    case IKS4A1_LIS2DUXS12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LIS2DUXS12_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
    case IKS4A1_LSM6DSV32X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV32X_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV32X_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
    case IKS4A1_LSM6DSO16IS_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO16IS_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSO16IS_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the status of all hardware events
  * @param  Instance the device instance
  * @param  Status the pointer to the status of all hardware events
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_Event_Status(uint32_t Instance, IKS4A1_MOTION_SENSOR_Event_Status_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      if (LSM6DSV16X_ACC_Get_Event_Status(MotionCompObj[Instance], (LSM6DSV16X_Event_Status_t *)(void *)Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable the free fall detection
  * @param  Instance the device instance
  * @param  IntPin the interrupt pin to be used
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_Free_Fall_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_Free_Fall_Detection(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable the free fall detection
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_Free_Fall_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_Free_Fall_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the free fall detection threshold
  * @param  Instance the device instance
  * @param  Threshold the threshold to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Free_Fall_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Free_Fall_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the free fall detection duration
  * @param  Instance the device instance
  * @param  Duration the duration to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Free_Fall_Duration(uint32_t Instance, uint8_t Duration)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Free_Fall_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable the pedometer detection
  * @param  Instance the device instance
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_Pedometer(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_Pedometer(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable the pedometer detection
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_Pedometer(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_Pedometer(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Reset step counter
  * @param  Instance the device instance
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Reset_Step_Counter(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Step_Counter_Reset(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get step count
  * @param  Instance the device instance
  * @param  StepCount number of steps
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_Step_Count(uint32_t Instance, uint16_t *StepCount)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_Step_Count(MotionCompObj[Instance], StepCount) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable the tilt detection
  * @param  Instance the device instance
  * @param  IntPin the interrupt pin to be used
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_Tilt_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_Tilt_Detection(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable the tilt detection
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_Tilt_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_Tilt_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable the wake up detection
  * @param  Instance the device instance
  * @param  IntPin the interrupt pin to be used
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_Wake_Up_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_Wake_Up_Detection(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable the wake up detection
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_Wake_Up_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_Wake_Up_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the wake up detection threshold
  * @param  Instance the device instance
  * @param  Threshold the threshold to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Wake_Up_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Wake_Up_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the wake up detection duration
  * @param  Instance the device instance
  * @param  Duration the duration to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Wake_Up_Duration(uint32_t Instance, uint8_t Duration)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Wake_Up_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable the single tap detection
  * @param  Instance the device instance
  * @param  IntPin the interrupt pin to be used
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_Single_Tap_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_Single_Tap_Detection(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable the single tap detection
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_Single_Tap_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_Single_Tap_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable the double tap detection
  * @param  Instance the device instance
  * @param  IntPin the interrupt pin to be used
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_Double_Tap_Detection(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_Double_Tap_Detection(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable the double tap detection
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_Double_Tap_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_Double_Tap_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the tap threshold
  * @param  Instance the device instance
  * @param  Threshold the threshold to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Tap_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the tap shock time
  * @param  Instance the device instance
  * @param  Time the tap shock time to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Shock_Time(uint32_t Instance, uint8_t Time)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Tap_Shock_Time(MotionCompObj[Instance], Time) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the tap quiet time
  * @param  Instance the device instance
  * @param  Time the tap quiet time to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Quiet_Time(uint32_t Instance, uint8_t Time)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Tap_Quiet_Time(MotionCompObj[Instance], Time) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the tap duration time
  * @param  Instance the device instance
  * @param  Time the tap duration time to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_Tap_Duration_Time(uint32_t Instance, uint8_t Time)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_Tap_Duration_Time(MotionCompObj[Instance], Time) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable 6D Orientation
  * @param  Instance the device instance
  * @param  IntPin the interrupt pin to be used
  * @note   This function can modify sensor settings
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable_6D_Orientation(uint32_t Instance, IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
  LSM6DSV16X_SensorIntPin_t int_pin_local;
  switch (IntPin)
  {
    default:
    case IKS4A1_MOTION_SENSOR_INT1_PIN:
      int_pin_local = LSM6DSV16X_INT1_PIN;
      break;

    case IKS4A1_MOTION_SENSOR_INT2_PIN:
      int_pin_local = LSM6DSV16X_INT2_PIN;
      break;
  }
#endif

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Enable_6D_Orientation(MotionCompObj[Instance], int_pin_local) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable 6D Orientation
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable_6D_Orientation(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Disable_6D_Orientation(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the 6D orientation threshold
  * @param  Instance the device instance
  * @param  Threshold the threshold to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_6D_Orientation_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Set_6D_Orientation_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get 6D Orientation XL
  * @param  Instance the device instance
  * @param  xl the pointer to the 6D orientation XL axis
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_XL(uint32_t Instance, uint8_t *xl)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_6D_Orientation_XL(MotionCompObj[Instance], xl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get 6D Orientation XH
  * @param  Instance the device instance
  * @param  xh the pointer to the 6D orientation XH axis
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_XH(uint32_t Instance, uint8_t *xh)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_6D_Orientation_XH(MotionCompObj[Instance], xh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get 6D Orientation YL
  * @param  Instance the device instance
  * @param  yl the pointer to the 6D orientation YL axis
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_YL(uint32_t Instance, uint8_t *yl)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_6D_Orientation_YL(MotionCompObj[Instance], yl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get 6D Orientation YH
  * @param  Instance the device instance
  * @param  yh the pointer to the 6D orientation YH axis
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_YH(uint32_t Instance, uint8_t *yh)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_6D_Orientation_YH(MotionCompObj[Instance], yh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get 6D Orientation ZL
  * @param  Instance the device instance
  * @param  zl the pointer to the 6D orientation ZL axis
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_ZL(uint32_t Instance, uint8_t *zl)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_6D_Orientation_ZL(MotionCompObj[Instance], zl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get 6D Orientation ZH
  * @param  Instance the device instance
  * @param  zh the pointer to the 6D orientation ZH axis
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Get_6D_Orientation_ZH(uint32_t Instance, uint8_t *zh)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_ACC_Get_6D_Orientation_ZH(MotionCompObj[Instance], zh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get number of unread FIFO samples
  * @param  Instance the device instance
  * @param  NumSamples number of unread FIFO samples
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get FIFO full status
  * @param  Instance the device instance
  * @param  Status FIFO full status
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Full_Status(uint32_t Instance, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Get_Full_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set FIFO BDR value
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Odr FIFO BDR value
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_BDR(uint32_t Instance, uint32_t Function, float_t Bdr)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV16X_FIFO_ACC_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV16X_FIFO_GYRO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_WRONG_PARAM;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set FIFO full interrupt on INT1 pin
  * @param  Instance the device instance
  * @param  Status FIFO full interrupt on INT1 pin
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Set_INT1_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set FIFO watermark level
  * @param  Instance the device instance
  * @param  Watermark FIFO watermark level
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set FIFO stop on watermark
  * @param  Instance the device instance
  * @param  Status FIFO stop on watermark status
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set FIFO mode
  * @param  Instance the device instance
  * @param  Mode FIFO mode
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get FIFO tag
  * @param  Instance the device instance
  * @param  Tag FIFO tag
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Tag(uint32_t Instance, uint8_t *Tag)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_FIFO_Get_Tag(MotionCompObj[Instance], Tag) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get FIFO axes data
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Data FIFO axes data
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_FIFO_Get_Axes(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_Axes_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSV16X_FIFO_ACC_Get_Axes(MotionCompObj[Instance], (LSM6DSV16X_Axes_t *)Data) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSV16X_FIFO_GYRO_Get_Axes(MotionCompObj[Instance], (LSM6DSV16X_Axes_t *)Data) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_WRONG_PARAM;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set device self-test
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Mode self-test mode:
  *         - 0: disable self-test, 1: enable self-test (for LIS2MDL)
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Set_SelfTest(uint32_t Instance, uint32_t Function, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS4A1_LIS2MDL_0:
      if ((Function & MOTION_MAGNETO) == MOTION_MAGNETO)
      {
        if (LIS2MDL_MAG_Set_SelfTest(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_WRONG_PARAM;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set DRDY mode
  * @param  Instance the device instance
  * @param  Mode DRDY mode
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_DRDY_Set_Mode(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_DRDY_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Enable DRDY on selected interrupt pin
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  IntPin the interrupt pin to be used
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_DRDY_Enable_Interrupt(uint32_t Instance, uint32_t Function,
                                                   IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IntPin == IKS4A1_MOTION_SENSOR_INT1_PIN)
        {
          if (LSM6DSV16X_ACC_Enable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            ret = BSP_ERROR_NONE;
          }
        }
        else
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (IntPin == IKS4A1_MOTION_SENSOR_INT2_PIN)
        {
          if (LSM6DSV16X_GYRO_Enable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            ret = BSP_ERROR_NONE;
          }
        }
        else
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Disable DRDY on selected interrupt pin
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  IntPin the interrupt pin to be used
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_DRDY_Disable_Interrupt(uint32_t Instance, uint32_t Function,
                                                    IKS4A1_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IntPin == IKS4A1_MOTION_SENSOR_INT1_PIN)
        {
          if (LSM6DSV16X_ACC_Disable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            ret = BSP_ERROR_NONE;
          }
        }
        else
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
