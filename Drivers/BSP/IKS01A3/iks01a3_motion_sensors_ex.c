/**
 ******************************************************************************
 * @file    iks01a3_motion_sensors_ex.c
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of extended functions needed to manage the motion sensors
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "iks01a3_motion_sensors_ex.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A3 IKS01A3
 * @{
 */

/** @defgroup IKS01A3_MOTION_SENSOR_EX IKS01A3 MOTION SENSOR EX
 * @{
 */

/** @defgroup IKS01A3_MOTION_SENSOR_EX_Imported_Variables IKS01A3 MOTION SENSOR EX Imported Variables
 * @{
 */

extern void *MotionCompObj[IKS01A3_MOTION_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup IKS01A3_MOTION_SENSOR_EX_Exported_Functions IKS01A3 MOTION SENSOR EX Exported Functions
 * @{
 */

/**
 * @brief  Get the status of data ready bit (available only for LSM6DSO, LIS2DW12, LIS2MDL, ASM330LHH,
 *         IIS2DLPC, IIS2MDC, ISM303DAC, ISM330DLC, LIS2DH12, LSM6DSOX, AIS2IH, LSM6DSO32X sensors)
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_ACCELERO or MOTION_GYRO for instance IKS01A3_LSM6DSO_0
 *         - MOTION_ACCELERO for instance IKS01A3_LIS2DW12_0
 *         - MOTION_MAGNETO for instance IKS01A3_LIS2MDL_0
 * @param  Status the pointer to the status
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS01A3_LIS2MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS01A3_ASM330LHH_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS01A3_IIS2DLPC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS01A3_IIS2MDC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS01A3_ISM303DAC_ACC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS01A3_ISM303DAC_MAG_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS01A3_ISM330DLC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS01A3_LIS2DH12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS01A3_LSM6DSOX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS01A3_AIS2DW12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS01A3_LIS3MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS01A3_LSM6DSR_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS01A3_A3G4250D_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS01A3_AIS328DQ_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS01A3_AIS3624DQ_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS01A3_H3LIS331DL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS01A3_LSM6DSRX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS01A3_ISM330DHCX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS01A3_LSM6DSO32_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS01A3_IIS2ICLX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS01A3_LSM6DSO32X_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
    case IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0:
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

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
 * @brief  Get the register value (available only for LSM6DSO, LIS2DW12, LIS2MDL, ASM330LHH, IIS2DLPC,
 *         IIS2MDC, ISM303DAC, ISM330DLC, LIS2DH12, LSM6DSOX, AIS2IH, LSM6DSO32X sensors)
 * @param  Instance the device instance
 * @param  Reg address to be read
 * @param  Data pointer where the value is written to
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS01A3_LIS2MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS01A3_ASM330LHH_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS01A3_IIS2DLPC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS01A3_IIS2MDC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS01A3_ISM303DAC_ACC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS01A3_ISM303DAC_MAG_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS01A3_ISM330DLC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS01A3_LIS2DH12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS01A3_LSM6DSOX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS01A3_AIS2DW12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS01A3_LIS3MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS01A3_LSM6DSR_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS01A3_A3G4250D_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS01A3_AIS328DQ_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS01A3_AIS3624DQ_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS01A3_H3LIS331DL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS01A3_LSM6DSRX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS01A3_ISM330DHCX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS01A3_LSM6DSO32_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS01A3_IIS2ICLX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS01A3_LSM6DSO32X_0:
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

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
 * @brief  Set the register value (available only for LSM6DSO, LIS2DW12, LIS2MDL, ASM330LHH, IIS2DLPC,
 *         IIS2MDC, ISM303DAC, ISM330DLC, LIS2DH12, LSM6DSOX, AIS2IH, LSM6DSO32X sensors)
 * @param  Instance the device instance
 * @param  Reg address to be read
 * @param  Data value to be written
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS01A3_LIS2MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS01A3_ASM330LHH_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS01A3_IIS2DLPC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS01A3_IIS2MDC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS01A3_ISM303DAC_ACC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS01A3_ISM303DAC_MAG_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS01A3_ISM330DLC_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS01A3_LIS2DH12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS01A3_LSM6DSOX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS01A3_AIS2DW12_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS01A3_LIS3MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS01A3_LSM6DSR_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS01A3_A3G4250D_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS01A3_AIS328DQ_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS01A3_AIS3624DQ_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS01A3_H3LIS331DL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS01A3_LSM6DSRX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS01A3_ISM330DHCX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS01A3_LSM6DSO32_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS01A3_IIS2ICLX_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS01A3_LSM6DSO32X_0:
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

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
 * @brief  Get the status of all hardware events (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Status the pointer to the status of all hardware events
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_Event_Status(uint32_t Instance, IKS01A3_MOTION_SENSOR_Event_Status_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      if (LSM6DSO_ACC_Get_Event_Status(MotionCompObj[Instance], (LSM6DSO_Event_Status_t *)(void *)Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      if (LIS2DW12_ACC_Get_Event_Status(MotionCompObj[Instance], (LIS2DW12_Event_Status_t *)(void *)Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      if (AIS2IH_ACC_Get_Event_Status(MotionCompObj[Instance], (AIS2IH_Event_Status_t *)(void *)Status) != BSP_ERROR_NONE)
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
 * @brief  Enable the free fall detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSO accelerometer ODR to 416Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Free_Fall_Detection(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_Free_Fall_Detection(MotionCompObj[Instance], (LSM6DSO_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the free fall detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Free_Fall_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_Free_Fall_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Set the free fall detection threshold (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Free_Fall_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Free_Fall_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
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
 * @brief  Set the free fall detection duration (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Duration the duration to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Free_Fall_Duration(uint32_t Instance, uint8_t Duration)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Free_Fall_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
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
 * @brief  Enable the pedometer detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @note   This function sets the LSM6DSO accelerometer ODR to 26Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Pedometer(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_Pedometer(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Disable the pedometer detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Pedometer(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_Pedometer(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Reset step counter (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @note   This function sets the LSM6DSO accelerometer ODR to 26Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Reset_Step_Counter(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Step_Counter_Reset(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Get step count (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  StepCount number of steps
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_Step_Count(uint32_t Instance, uint16_t *StepCount)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_Step_Count(MotionCompObj[Instance], StepCount) != BSP_ERROR_NONE)
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
 * @brief  Enable the tilt detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSO accelerometer ODR to 26Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Tilt_Detection(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_Tilt_Detection(MotionCompObj[Instance], (LSM6DSO_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the tilt detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Tilt_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_Tilt_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Enable the wake up detection (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSO accelerometer ODR to 416Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Wake_Up_Detection(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_Wake_Up_Detection(MotionCompObj[Instance], (LSM6DSO_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      /* For LIS2DW12 the Wake Up event can be routed only on INT1 pin */
      if (LIS2DW12_ACC_Enable_Wake_Up_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      /* For AIS2IH the Wake Up event can be routed only on INT1 pin */
      if (AIS2IH_ACC_Enable_Wake_Up_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Disable the wake up detection (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Wake_Up_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_Wake_Up_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Disable_Wake_Up_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Disable_Wake_Up_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Set the wake up detection threshold (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Wake_Up_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Wake_Up_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Set_Wake_Up_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Set_Wake_Up_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
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
 * @brief  Set the wake up detection duration (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Duration the duration to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Wake_Up_Duration(uint32_t Instance, uint8_t Duration)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Wake_Up_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Set_Wake_Up_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Set_Wake_Up_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
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
 * @brief  Enable the inactivity detection (available only for LIS2DW12 and AIS2IH sensors)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Inactivity_Detection(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      /* For LIS2DW12 the Inactivity event can be routed only on INT2 pin */
      if (LIS2DW12_ACC_Enable_Inactivity_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      /* For AIS2IH the Inactivity event can be routed only on INT2 pin */
      if (AIS2IH_ACC_Enable_Inactivity_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Disable the inactivity detection (available only for LIS2DW12 and AIS2IH sensors)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Inactivity_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Disable_Inactivity_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Disable_Inactivity_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Set the sleep duration (available only for LIS2DW12 and AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Duration the duration to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Sleep_Duration(uint32_t Instance, uint8_t Duration)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Set_Sleep_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Set_Sleep_Duration(MotionCompObj[Instance], Duration) != BSP_ERROR_NONE)
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
 * @brief  Enable the single tap detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSO accelerometer ODR to 416Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Single_Tap_Detection(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_Single_Tap_Detection(MotionCompObj[Instance], (LSM6DSO_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the single tap detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Single_Tap_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_Single_Tap_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Enable the double tap detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSO accelerometer ODR to 416Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_Double_Tap_Detection(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_Double_Tap_Detection(MotionCompObj[Instance], (LSM6DSO_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the double tap detection (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_Double_Tap_Detection(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_Double_Tap_Detection(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Set the tap threshold (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Tap_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Tap_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
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
 * @brief  Set the tap shock time (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Time the tap shock time to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Tap_Shock_Time(uint32_t Instance, uint8_t Time)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Tap_Shock_Time(MotionCompObj[Instance], Time) != BSP_ERROR_NONE)
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
 * @brief  Set the tap quiet time (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Time the tap quiet time to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Tap_Quiet_Time(uint32_t Instance, uint8_t Time)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Tap_Quiet_Time(MotionCompObj[Instance], Time) != BSP_ERROR_NONE)
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
 * @brief  Set the tap duration time (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Time the tap duration time to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_Tap_Duration_Time(uint32_t Instance, uint8_t Time)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_Tap_Duration_Time(MotionCompObj[Instance], Time) != BSP_ERROR_NONE)
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
 * @brief  Enable 6D Orientation (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSO accelerometer ODR to 416Hz and the LSM6DSO accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable_6D_Orientation(uint32_t Instance, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Enable_6D_Orientation(MotionCompObj[Instance], (LSM6DSO_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      /* For LIS2DW12 the 6D Orientation event can be routed only on INT1 pin */
      if (LIS2DW12_ACC_Enable_6D_Orientation(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      /* For AIS2IH the 6D Orientation event can be routed only on INT1 pin */
      if (AIS2IH_ACC_Enable_6D_Orientation(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Disable 6D Orientation (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable_6D_Orientation(uint32_t Instance)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Disable_6D_Orientation(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Disable_6D_Orientation(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Disable_6D_Orientation(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Set the 6D orientation threshold (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_6D_Orientation_Threshold(uint32_t Instance, uint8_t Threshold)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Set_6D_Orientation_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Set_6D_Orientation_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Set_6D_Orientation_Threshold(MotionCompObj[Instance], Threshold) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation XL (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  xl the pointer to the 6D orientation XL axis
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_6D_Orientation_XL(uint32_t Instance, uint8_t *xl)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_6D_Orientation_XL(MotionCompObj[Instance], xl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Get_6D_Orientation_XL(MotionCompObj[Instance], xl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Get_6D_Orientation_XL(MotionCompObj[Instance], xl) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation XH (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  xh the pointer to the 6D orientation XH axis
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_6D_Orientation_XH(uint32_t Instance, uint8_t *xh)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_6D_Orientation_XH(MotionCompObj[Instance], xh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Get_6D_Orientation_XH(MotionCompObj[Instance], xh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Get_6D_Orientation_XH(MotionCompObj[Instance], xh) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation YL (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  yl the pointer to the 6D orientation YL axis
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_6D_Orientation_YL(uint32_t Instance, uint8_t *yl)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_6D_Orientation_YL(MotionCompObj[Instance], yl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Get_6D_Orientation_YL(MotionCompObj[Instance], yl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Get_6D_Orientation_YL(MotionCompObj[Instance], yl) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation YH (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  yh the pointer to the 6D orientation YH axis
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_6D_Orientation_YH(uint32_t Instance, uint8_t *yh)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_6D_Orientation_YH(MotionCompObj[Instance], yh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Get_6D_Orientation_YH(MotionCompObj[Instance], yh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Get_6D_Orientation_YH(MotionCompObj[Instance], yh) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation ZL (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  zl the pointer to the 6D orientation ZL axis
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_6D_Orientation_ZL(uint32_t Instance, uint8_t *zl)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_6D_Orientation_ZL(MotionCompObj[Instance], zl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Get_6D_Orientation_ZL(MotionCompObj[Instance], zl) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Get_6D_Orientation_ZL(MotionCompObj[Instance], zl) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation ZH (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  zh the pointer to the 6D orientation ZH axis
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Get_6D_Orientation_ZH(uint32_t Instance, uint8_t *zh)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_ACC_Get_6D_Orientation_ZH(MotionCompObj[Instance], zh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_ACC_Get_6D_Orientation_ZH(MotionCompObj[Instance], zh) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_ACC_Get_6D_Orientation_ZH(MotionCompObj[Instance], zh) != BSP_ERROR_NONE)
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
 * @brief  Get number of unread FIFO samples (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  NumSamples number of unread FIFO samples
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_FIFO_Get_Num_Samples(MotionCompObj[Instance], NumSamples) != BSP_ERROR_NONE)
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
 * @brief  Get FIFO full status (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Status FIFO full status
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Get_Full_Status(uint32_t Instance, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Get_Full_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO BDR value (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for instance IKS01A3_LSM6DSO_0
 * @param  Odr FIFO BDR value
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Set_BDR(uint32_t Instance, uint32_t Function, float Bdr)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO_FIFO_ACC_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
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
        if (LSM6DSO_FIFO_GYRO_Set_BDR(MotionCompObj[Instance], Bdr) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO full interrupt on INT1 pin (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Status FIFO full interrupt on INT1 pin
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Set_INT1_FIFO_Full(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO watermark level (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Watermark FIFO watermark level
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Set_Watermark_Level(MotionCompObj[Instance], Watermark) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO stop on watermark (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Status FIFO stop on watermark status
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Set_Stop_On_Fth(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO mode (available only for LSM6DSO, LIS2DW12, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Mode FIFO mode
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_FIFO_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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
 * @brief  Get FIFO tag (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Tag FIFO tag
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Get_Tag(uint32_t Instance, uint8_t *Tag)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_FIFO_Get_Tag(MotionCompObj[Instance], Tag) != BSP_ERROR_NONE)
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
 * @brief  Get FIFO axes data (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for instance IKS01A3_LSM6DSO_0
 * @param  Data FIFO axes data
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_FIFO_Get_Axes(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_Axes_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO_FIFO_ACC_Get_Axes(MotionCompObj[Instance], (LSM6DSO_Axes_t *)Data) != BSP_ERROR_NONE)
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
        if (LSM6DSO_FIFO_GYRO_Get_Axes(MotionCompObj[Instance], (LSM6DSO_Axes_t *)Data) != BSP_ERROR_NONE)
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
 * @brief  Set device self-test (available only for LSM6DSO, LIS2DW12, LIS2MDL, AIS2IH sensors)
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for LSM6DSO
 *         - MOTION_ACCELERO                for LIS2DW12, AIS2IH
 *         - MOTION_MAGNETO                 for LIS2MDL
 *
 * @param  Mode self-test mode:
 *         - 0: disable self-test, 1: positive self-test, 2: negative self-test for LSM6DSO, LIS2DW12, AIS2IH
 *         - 0: disable self-test, 1: enable self-test                          for LIS2MDL
 *
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Set_SelfTest(uint32_t Instance, uint32_t Function, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSO_ACC_Set_SelfTest(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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
        if (LSM6DSO_GYRO_Set_SelfTest(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LIS2DW12_ACC_Set_SelfTest(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS01A3_LIS2MDL_0:
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

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (AIS2IH_ACC_Set_SelfTest(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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
 * @brief  Set DRDY mode (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Mode DRDY mode
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_DRDY_Set_Mode(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_DRDY_Set_Mode(MotionCompObj[Instance], Mode) != BSP_ERROR_NONE)
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
 * @brief  Enable DRDY on selected interrupt pin (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for LSM6DSO
 * @param  IntPin the interrupt pin to be used
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_DRDY_Enable_Interrupt(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IntPin == IKS01A3_MOTION_SENSOR_INT1_PIN)
        {
          if (LSM6DSO_ACC_Enable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
        if (IntPin == IKS01A3_MOTION_SENSOR_INT2_PIN)
        {
          if (LSM6DSO_GYRO_Enable_DRDY_On_INT2(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
 * @brief  Disable DRDY on selected interrupt pin (available only for LSM6DSO sensor)
 * @param  Instance the device instance
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for LSM6DSO
 * @param  IntPin the interrupt pin to be used
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_DRDY_Disable_Interrupt(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (IntPin == IKS01A3_MOTION_SENSOR_INT1_PIN)
        {
          if (LSM6DSO_ACC_Disable_DRDY_On_INT1(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
