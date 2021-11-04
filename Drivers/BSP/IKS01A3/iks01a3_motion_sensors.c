/**
 ******************************************************************************
 * @file    iks01a3_motion_sensors.c
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of functions needed to manage the motion sensors
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
#include "iks01a3_motion_sensors.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A3 IKS01A3
 * @{
 */

/** @defgroup IKS01A3_MOTION_SENSOR IKS01A3 MOTION SENSOR
 * @{
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Exported_Variables IKS01A3 MOTION SENSOR Exported Variables
 * @{
 */
extern void
*MotionCompObj[IKS01A3_MOTION_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *MotionCompObj[IKS01A3_MOTION_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Private_Variables IKS01A3 MOTION SENSOR Private Variables
 * @{
 */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
static uint32_t FunctionIndex[5] = {0, 0, 1, 1, 2};
static MOTION_SENSOR_FuncDrv_t *MotionFuncDrv[IKS01A3_MOTION_INSTANCES_NBR][IKS01A3_MOTION_FUNCTIONS_NBR];
static MOTION_SENSOR_CommonDrv_t *MotionDrv[IKS01A3_MOTION_INSTANCES_NBR];
static IKS01A3_MOTION_SENSOR_Ctx_t MotionCtx[IKS01A3_MOTION_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Private_Function_Prototypes IKS01A3 MOTION SENSOR Private Function Prototypes
 * @{
 */

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
static int32_t LSM6DSO_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
static int32_t LIS2DW12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
static int32_t LIS2MDL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
static int32_t ASM330LHH_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
static int32_t IIS2DLPC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
static int32_t IIS2MDC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
static int32_t ISM303DAC_ACC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
static int32_t ISM303DAC_MAG_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
static int32_t ISM330DLC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
static int32_t LIS2DH12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
static int32_t LSM6DSOX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
static int32_t AIS2DW12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
static int32_t LIS3MDL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
static int32_t LSM6DSR_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
static int32_t A3G4250D_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
static int32_t AIS328DQ_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
static int32_t AIS3624DQ_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
static int32_t H3LIS331DL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
static int32_t LSM6DSRX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
static int32_t ISM330DHCX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
static int32_t LSM6DSO32_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
static int32_t IIS2ICLX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
static int32_t AIS2IH_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
static int32_t LSM6DSO32X_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
static int32_t LSM6DSOX_SENSORHUB_LIS2MDL_0_Probe(uint32_t Functions);
#endif

/**
 * @}
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Exported_Functions IKS01A3 MOTION SENSOR Exported Functions
 * @{
 */

/**
 * @brief  Initializes the motion sensors
 * @param  Instance Motion sensor instance
 * @param  Functions Motion sensor functions. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instances 0, 3, 4
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = MOTION_GYRO;
  uint32_t i;
  uint32_t component_functions = 0;
  IKS01A3_MOTION_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS01A3_LSM6DSO_0:
      if (LSM6DSO_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS01A3_LIS2DW12_0:
      if (LIS2DW12_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS01A3_LIS2MDL_0:
      if (LIS2MDL_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS01A3_ASM330LHH_0:
      if (ASM330LHH_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS01A3_IIS2DLPC_0:
      if (IIS2DLPC_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS01A3_IIS2MDC_0:
      if (IIS2MDC_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS01A3_ISM303DAC_ACC_0:
      if (ISM303DAC_ACC_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS01A3_ISM303DAC_MAG_0:
      if (ISM303DAC_MAG_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS01A3_ISM330DLC_0:
      if (ISM330DLC_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS01A3_LIS2DH12_0:
      if (LIS2DH12_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS01A3_LSM6DSOX_0:
      if (LSM6DSOX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS01A3_AIS2DW12_0:
      if (AIS2DW12_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS01A3_LIS3MDL_0:
      if (LIS3MDL_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS01A3_LSM6DSR_0:
      if (LSM6DSR_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS01A3_A3G4250D_0:
      if (A3G4250D_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS01A3_AIS328DQ_0:
      if (AIS328DQ_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS01A3_AIS3624DQ_0:
      if (AIS3624DQ_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS01A3_H3LIS331DL_0:
      if (H3LIS331DL_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS01A3_LSM6DSRX_0:
      if (LSM6DSRX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS01A3_ISM330DHCX_0:
      if (ISM330DHCX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS01A3_LSM6DSO32_0:
      if (LSM6DSO32_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS01A3_IIS2ICLX_0:
      if (IIS2ICLX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS01A3_AIS2IH_0:
      if (AIS2IH_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS01A3_LSM6DSO32X_0:
      if (LSM6DSO32X_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
    case IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0:
      if (LSM6DSOX_SENSORHUB_LIS2MDL_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  for (i = 0; i < IKS01A3_MOTION_FUNCTIONS_NBR; i++)
  {
    if (((Functions & function) == function) && ((component_functions & function) == function))
    {
      if (MotionFuncDrv[Instance][FunctionIndex[function]]->Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    function = function << 1;
  }

  return ret;
}

/**
 * @brief  Deinitialize Motion sensor
 * @param  Instance Motion sensor instance
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->DeInit(MotionCompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Get motion sensor instance capabilities
 * @param  Instance Motion sensor instance
 * @param  Capabilities pointer to motion sensor capabilities
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_GetCapabilities(uint32_t Instance, IKS01A3_MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], Capabilities) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Get WHOAMI value
 * @param  Instance Motion sensor instance
 * @param  Id WHOAMI value
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->ReadID(MotionCompObj[Instance], Id) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Enable Motion sensor
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Disable Motion sensor
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->Disable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Get motion sensor axes data
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Axes pointer to axes data structure
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetAxes(MotionCompObj[Instance], Axes) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Get motion sensor axes raw data
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Axes pointer to axes raw data structure
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetAxesRaw(MotionCompObj[Instance], Axes) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Get motion sensor sensitivity
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Sensitivity pointer to sensitivity read value
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float *Sensitivity)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetSensitivity(MotionCompObj[Instance],
          Sensitivity) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Get motion sensor Output Data Rate
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Odr pointer to Output Data Rate read value
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Get motion sensor Full Scale
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Fullscale pointer to Fullscale read value
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetFullScale(MotionCompObj[Instance],
          Fullscale) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Set motion sensor Output Data Rate
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Odr Output Data Rate value to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->SetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @brief  Set motion sensor Full Scale
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Fullscale Fullscale value to be set
 * @retval BSP status
 */
int32_t IKS01A3_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  int32_t ret;

  if (Instance >= IKS01A3_MOTION_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->SetFullScale(MotionCompObj[Instance],
          Fullscale) != BSP_ERROR_NONE)
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
  }

  return ret;
}

/**
 * @}
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Private_Functions IKS01A3 MOTION SENSOR Private Functions
 * @{
 */

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSO_0_Probe(uint32_t Functions)
{
  LSM6DSO_IO_t            io_ctx;
  uint8_t                 id;
  static LSM6DSO_Object_t lsm6dso_obj_0;
  LSM6DSO_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSO_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO_I2C_ADD_H;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LSM6DSO_RegisterBusIO(&lsm6dso_obj_0, &io_ctx) != LSM6DSO_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO_ReadID(&lsm6dso_obj_0, &id) != LSM6DSO_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSO_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSO_GetCapabilities(&lsm6dso_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSO_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSO_0] = &lsm6dso_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSO_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSO_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSO_GYRO_Driver;

      if (MotionDrv[IKS01A3_LSM6DSO_0]->Init(MotionCompObj[IKS01A3_LSM6DSO_0]) != LSM6DSO_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSO_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSO_ACC_Driver;

      if (MotionDrv[IKS01A3_LSM6DSO_0]->Init(MotionCompObj[IKS01A3_LSM6DSO_0]) != LSM6DSO_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0  == 1)
/**
 * @brief  Register Bus IOs for instance 1 if component ID is OK
 * @retval BSP status
 */
static int32_t LIS2DW12_0_Probe(uint32_t Functions)
{
  LIS2DW12_IO_t            io_ctx;
  uint8_t                  id;
  static LIS2DW12_Object_t lis2dw12_obj_0;
  LIS2DW12_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LIS2DW12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DW12_I2C_ADD_H;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LIS2DW12_RegisterBusIO(&lis2dw12_obj_0, &io_ctx) != LIS2DW12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DW12_ReadID(&lis2dw12_obj_0, &id) != LIS2DW12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2DW12_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2DW12_GetCapabilities(&lis2dw12_obj_0, &cap);
    MotionCtx[IKS01A3_LIS2DW12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LIS2DW12_0] = &lis2dw12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LIS2DW12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DW12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LIS2DW12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LIS2DW12_ACC_Driver;

      if (MotionDrv[IKS01A3_LIS2DW12_0]->Init(MotionCompObj[IKS01A3_LIS2DW12_0]) != LIS2DW12_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0  == 1)
/**
 * @brief  Register Bus IOs for instance 1 if component ID is OK
 * @retval BSP status
 */
static int32_t LIS2MDL_0_Probe(uint32_t Functions)
{
  LIS2MDL_IO_t            io_ctx;
  uint8_t                 id;
  static LIS2MDL_Object_t lis2mdl_obj_0;
  LIS2MDL_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LIS2MDL_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2MDL_I2C_ADD;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LIS2MDL_RegisterBusIO(&lis2mdl_obj_0, &io_ctx) != LIS2MDL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2MDL_ReadID(&lis2mdl_obj_0, &id) != LIS2MDL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2MDL_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2MDL_GetCapabilities(&lis2mdl_obj_0, &cap);
    MotionCtx[IKS01A3_LIS2MDL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LIS2MDL_0] = &lis2mdl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LIS2MDL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2MDL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO) && (cap.Magneto == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LIS2MDL_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LIS2MDL_MAG_Driver;

      if (MotionDrv[IKS01A3_LIS2MDL_0]->Init(MotionCompObj[IKS01A3_LIS2MDL_0]) != LIS2MDL_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t ASM330LHH_0_Probe(uint32_t Functions)
{
  ASM330LHH_IO_t            io_ctx;
  uint8_t                   id;
  static ASM330LHH_Object_t asm330lhh_obj_0;
  ASM330LHH_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ASM330LHH_I2C_BUS; /* I2C */
  io_ctx.Address     = ASM330LHH_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (ASM330LHH_RegisterBusIO(&asm330lhh_obj_0, &io_ctx) != ASM330LHH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ASM330LHH_ReadID(&asm330lhh_obj_0, &id) != ASM330LHH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ASM330LHH_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ASM330LHH_GetCapabilities(&asm330lhh_obj_0, &cap);
    MotionCtx[IKS01A3_ASM330LHH_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_ASM330LHH_0] = &asm330lhh_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_ASM330LHH_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ASM330LHH_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ASM330LHH_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ASM330LHH_GYRO_Driver;

      if (MotionDrv[IKS01A3_ASM330LHH_0]->Init(MotionCompObj[IKS01A3_ASM330LHH_0]) != ASM330LHH_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ASM330LHH_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&ASM330LHH_ACC_Driver;

      if (MotionDrv[IKS01A3_ASM330LHH_0]->Init(MotionCompObj[IKS01A3_ASM330LHH_0]) != ASM330LHH_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t IIS2DLPC_0_Probe(uint32_t Functions)
{
  IIS2DLPC_IO_t            io_ctx;
  uint8_t                  id;
  static IIS2DLPC_Object_t iis2dlpc_obj_0;
  IIS2DLPC_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = IIS2DLPC_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2DLPC_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (IIS2DLPC_RegisterBusIO(&iis2dlpc_obj_0, &io_ctx) != IIS2DLPC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2DLPC_ReadID(&iis2dlpc_obj_0, &id) != IIS2DLPC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != IIS2DLPC_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2DLPC_GetCapabilities(&iis2dlpc_obj_0, &cap);
    MotionCtx[IKS01A3_IIS2DLPC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_IIS2DLPC_0] = &iis2dlpc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_IIS2DLPC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2DLPC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_IIS2DLPC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&IIS2DLPC_ACC_Driver;

      if (MotionDrv[IKS01A3_IIS2DLPC_0]->Init(MotionCompObj[IKS01A3_IIS2DLPC_0]) != IIS2DLPC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t IIS2MDC_0_Probe(uint32_t Functions)
{
  IIS2MDC_IO_t            io_ctx;
  uint8_t                 id;
  static IIS2MDC_Object_t iis2mdc_obj_0;
  IIS2MDC_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = IIS2MDC_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2MDC_I2C_ADD;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (IIS2MDC_RegisterBusIO(&iis2mdc_obj_0, &io_ctx) != IIS2MDC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2MDC_ReadID(&iis2mdc_obj_0, &id) != IIS2MDC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != IIS2MDC_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2MDC_GetCapabilities(&iis2mdc_obj_0, &cap);
    MotionCtx[IKS01A3_IIS2MDC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_IIS2MDC_0] = &iis2mdc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_IIS2MDC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2MDC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_IIS2MDC_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&IIS2MDC_MAG_Driver;

      if (MotionDrv[IKS01A3_IIS2MDC_0]->Init(MotionCompObj[IKS01A3_IIS2MDC_0]) != IIS2MDC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t ISM303DAC_ACC_0_Probe(uint32_t Functions)
{
  ISM303DAC_IO_t                io_ctx;
  uint8_t                       id;
  static ISM303DAC_ACC_Object_t ism303dac_acc_obj_0;
  ISM303DAC_Capabilities_t      cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM303DAC_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM303DAC_I2C_ADD_XL;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (ISM303DAC_ACC_RegisterBusIO(&ism303dac_acc_obj_0, &io_ctx) != ISM303DAC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM303DAC_ACC_ReadID(&ism303dac_acc_obj_0, &id) != ISM303DAC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM303DAC_ID_XL)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM303DAC_ACC_GetCapabilities(&ism303dac_acc_obj_0, &cap);
    MotionCtx[IKS01A3_ISM303DAC_ACC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_ISM303DAC_ACC_0] = &ism303dac_acc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_ISM303DAC_ACC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM303DAC_ACC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ISM303DAC_ACC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&ISM303DAC_ACC_Driver;

      if (MotionDrv[IKS01A3_ISM303DAC_ACC_0]->Init(MotionCompObj[IKS01A3_ISM303DAC_ACC_0]) != ISM303DAC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t ISM303DAC_MAG_0_Probe(uint32_t Functions)
{
  ISM303DAC_IO_t                io_ctx;
  uint8_t                       id;
  static ISM303DAC_MAG_Object_t ism303dac_mag_obj_0;
  ISM303DAC_Capabilities_t      cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM303DAC_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM303DAC_I2C_ADD_MG;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (ISM303DAC_MAG_RegisterBusIO(&ism303dac_mag_obj_0, &io_ctx) != ISM303DAC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM303DAC_MAG_ReadID(&ism303dac_mag_obj_0, &id) != ISM303DAC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM303DAC_ID_MG)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM303DAC_MAG_GetCapabilities(&ism303dac_mag_obj_0, &cap);
    MotionCtx[IKS01A3_ISM303DAC_MAG_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_ISM303DAC_MAG_0] = &ism303dac_mag_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_ISM303DAC_MAG_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM303DAC_MAG_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ISM303DAC_MAG_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&ISM303DAC_MAG_Driver;

      if (MotionDrv[IKS01A3_ISM303DAC_MAG_0]->Init(MotionCompObj[IKS01A3_ISM303DAC_MAG_0]) != ISM303DAC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t ISM330DLC_0_Probe(uint32_t Functions)
{
  ISM330DLC_IO_t            io_ctx;
  uint8_t                   id;
  static ISM330DLC_Object_t ism330dlc_obj_0;
  ISM330DLC_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM330DLC_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DLC_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (ISM330DLC_RegisterBusIO(&ism330dlc_obj_0, &io_ctx) != ISM330DLC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DLC_ReadID(&ism330dlc_obj_0, &id) != ISM330DLC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM330DLC_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM330DLC_GetCapabilities(&ism330dlc_obj_0, &cap);
    MotionCtx[IKS01A3_ISM330DLC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_ISM330DLC_0] = &ism330dlc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_ISM330DLC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330DLC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ISM330DLC_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330DLC_GYRO_Driver;

      if (MotionDrv[IKS01A3_ISM330DLC_0]->Init(MotionCompObj[IKS01A3_ISM330DLC_0]) != ISM330DLC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ISM330DLC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&ISM330DLC_ACC_Driver;

      if (MotionDrv[IKS01A3_ISM330DLC_0]->Init(MotionCompObj[IKS01A3_ISM330DLC_0]) != ISM330DLC_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LIS2DH12_0_Probe(uint32_t Functions)
{
  LIS2DH12_IO_t            io_ctx;
  uint8_t                  id;
  static LIS2DH12_Object_t lis2dh12_obj_0;
  LIS2DH12_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LIS2DH12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DH12_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LIS2DH12_RegisterBusIO(&lis2dh12_obj_0, &io_ctx) != LIS2DH12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DH12_ReadID(&lis2dh12_obj_0, &id) != LIS2DH12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2DH12_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2DH12_GetCapabilities(&lis2dh12_obj_0, &cap);
    MotionCtx[IKS01A3_LIS2DH12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LIS2DH12_0] = &lis2dh12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LIS2DH12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DH12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LIS2DH12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LIS2DH12_Driver;

      if (MotionDrv[IKS01A3_LIS2DH12_0]->Init(MotionCompObj[IKS01A3_LIS2DH12_0]) != LIS2DH12_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSOX_0_Probe(uint32_t Functions)
{
  LSM6DSOX_IO_t            io_ctx;
  uint8_t                  id;
  static LSM6DSOX_Object_t lsm6dsox_obj_0;
  LSM6DSOX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSOX_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSOX_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LSM6DSOX_RegisterBusIO(&lsm6dsox_obj_0, &io_ctx) != LSM6DSOX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSOX_ReadID(&lsm6dsox_obj_0, &id) != LSM6DSOX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSOX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSOX_GetCapabilities(&lsm6dsox_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSOX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSOX_0] = &lsm6dsox_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSOX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSOX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSOX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSOX_GYRO_Driver;

      if (MotionDrv[IKS01A3_LSM6DSOX_0]->Init(MotionCompObj[IKS01A3_LSM6DSOX_0]) != LSM6DSOX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSOX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSOX_ACC_Driver;

      if (MotionDrv[IKS01A3_LSM6DSOX_0]->Init(MotionCompObj[IKS01A3_LSM6DSOX_0]) != LSM6DSOX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t AIS2DW12_0_Probe(uint32_t Functions)
{
  AIS2DW12_IO_t            io_ctx;
  uint8_t                  id;
  static AIS2DW12_Object_t ais2dw12_obj_0;
  AIS2DW12_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = AIS2DW12_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS2DW12_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (AIS2DW12_RegisterBusIO(&ais2dw12_obj_0, &io_ctx) != AIS2DW12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS2DW12_ReadID(&ais2dw12_obj_0, &id) != AIS2DW12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != AIS2DW12_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS2DW12_GetCapabilities(&ais2dw12_obj_0, &cap);
    MotionCtx[IKS01A3_AIS2DW12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_AIS2DW12_0] = &ais2dw12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_AIS2DW12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS2DW12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_AIS2DW12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&AIS2DW12_ACC_Driver;

      if (MotionDrv[IKS01A3_AIS2DW12_0]->Init(MotionCompObj[IKS01A3_AIS2DW12_0]) != AIS2DW12_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LIS3MDL_0_Probe(uint32_t Functions)
{
  LIS3MDL_IO_t            io_ctx;
  uint8_t                 id;
  static LIS3MDL_Object_t lis3mdl_obj_0;
  LIS3MDL_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LIS3MDL_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS3MDL_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LIS3MDL_RegisterBusIO(&lis3mdl_obj_0, &io_ctx) != LIS3MDL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS3MDL_ReadID(&lis3mdl_obj_0, &id) != LIS3MDL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS3MDL_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS3MDL_GetCapabilities(&lis3mdl_obj_0, &cap);
    MotionCtx[IKS01A3_LIS3MDL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LIS3MDL_0] = &lis3mdl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LIS3MDL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS3MDL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LIS3MDL_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LIS3MDL_MAG_Driver;

      if (MotionDrv[IKS01A3_LIS3MDL_0]->Init(MotionCompObj[IKS01A3_LIS3MDL_0]) != LIS3MDL_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSR_0_Probe(uint32_t Functions)
{
  LSM6DSR_IO_t            io_ctx;
  uint8_t                 id;
  static LSM6DSR_Object_t lsm6dsr_obj_0;
  LSM6DSR_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSR_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSR_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LSM6DSR_RegisterBusIO(&lsm6dsr_obj_0, &io_ctx) != LSM6DSR_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSR_ReadID(&lsm6dsr_obj_0, &id) != LSM6DSR_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSR_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSR_GetCapabilities(&lsm6dsr_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSR_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSR_0] = &lsm6dsr_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSR_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSR_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSR_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSR_GYRO_Driver;

      if (MotionDrv[IKS01A3_LSM6DSR_0]->Init(MotionCompObj[IKS01A3_LSM6DSR_0]) != LSM6DSR_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSR_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSR_ACC_Driver;

      if (MotionDrv[IKS01A3_LSM6DSR_0]->Init(MotionCompObj[IKS01A3_LSM6DSR_0]) != LSM6DSR_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t A3G4250D_0_Probe(uint32_t Functions)
{
  A3G4250D_IO_t             io_ctx;
  uint8_t                   id;
  static A3G4250D_Object_t  a3g4250d_obj_0;
  A3G4250D_Capabilities_t   cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = A3G4250D_I2C_BUS; /* I2C */
  io_ctx.Address     = A3G4250D_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (A3G4250D_RegisterBusIO(&a3g4250d_obj_0, &io_ctx) != A3G4250D_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (A3G4250D_ReadID(&a3g4250d_obj_0, &id) != A3G4250D_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != A3G4250D_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)A3G4250D_GetCapabilities(&a3g4250d_obj_0, &cap);
    MotionCtx[IKS01A3_A3G4250D_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_A3G4250D_0] = &a3g4250d_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_A3G4250D_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&A3G4250D_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_A3G4250D_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
          void *)&A3G4250D_GYRO_Driver;

      if (MotionDrv[IKS01A3_A3G4250D_0]->Init(MotionCompObj[IKS01A3_A3G4250D_0]) != A3G4250D_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t AIS328DQ_0_Probe(uint32_t Functions)
{
  AIS328DQ_IO_t            io_ctx;
  uint8_t                  id;
  static AIS328DQ_Object_t ais328dq_obj_0;
  AIS328DQ_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = AIS328DQ_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS328DQ_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (AIS328DQ_RegisterBusIO(&ais328dq_obj_0, &io_ctx) != AIS328DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS328DQ_ReadID(&ais328dq_obj_0, &id) != AIS328DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != AIS328DQ_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS328DQ_GetCapabilities(&ais328dq_obj_0, &cap);
    MotionCtx[IKS01A3_AIS328DQ_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_AIS328DQ_0] = &ais328dq_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_AIS328DQ_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS328DQ_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_AIS328DQ_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
          void *)&AIS328DQ_ACC_Driver;

      if (MotionDrv[IKS01A3_AIS328DQ_0]->Init(MotionCompObj[IKS01A3_AIS328DQ_0]) != AIS328DQ_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t AIS3624DQ_0_Probe(uint32_t Functions)
{
  AIS3624DQ_IO_t            io_ctx;
  uint8_t                   id;
  static AIS3624DQ_Object_t ais3624dq_obj_0;
  AIS3624DQ_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = AIS3624DQ_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS3624DQ_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (AIS3624DQ_RegisterBusIO(&ais3624dq_obj_0, &io_ctx) != AIS3624DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS3624DQ_ReadID(&ais3624dq_obj_0, &id) != AIS3624DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != AIS3624DQ_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS3624DQ_GetCapabilities(&ais3624dq_obj_0, &cap);
    MotionCtx[IKS01A3_AIS3624DQ_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_AIS3624DQ_0] = &ais3624dq_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_AIS3624DQ_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS3624DQ_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_AIS3624DQ_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
          void *)&AIS3624DQ_ACC_Driver;

      if (MotionDrv[IKS01A3_AIS3624DQ_0]->Init(MotionCompObj[IKS01A3_AIS3624DQ_0]) != AIS3624DQ_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t H3LIS331DL_0_Probe(uint32_t Functions)
{
  H3LIS331DL_IO_t            io_ctx;
  uint8_t                    id;
  static H3LIS331DL_Object_t h3lis331dl_obj_0;
  H3LIS331DL_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = H3LIS331DL_I2C_BUS; /* I2C */
  io_ctx.Address     = H3LIS331DL_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (H3LIS331DL_RegisterBusIO(&h3lis331dl_obj_0, &io_ctx) != H3LIS331DL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (H3LIS331DL_ReadID(&h3lis331dl_obj_0, &id) != H3LIS331DL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != H3LIS331DL_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)H3LIS331DL_GetCapabilities(&h3lis331dl_obj_0, &cap);
    MotionCtx[IKS01A3_H3LIS331DL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_H3LIS331DL_0] = &h3lis331dl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_H3LIS331DL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&H3LIS331DL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_H3LIS331DL_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
          void *)&H3LIS331DL_ACC_Driver;

      if (MotionDrv[IKS01A3_H3LIS331DL_0]->Init(MotionCompObj[IKS01A3_H3LIS331DL_0]) != H3LIS331DL_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSRX_0_Probe(uint32_t Functions)
{
  LSM6DSRX_IO_t            io_ctx;
  uint8_t                  id;
  static LSM6DSRX_Object_t lsm6dsrx_obj_0;
  LSM6DSRX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSRX_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSRX_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LSM6DSRX_RegisterBusIO(&lsm6dsrx_obj_0, &io_ctx) != LSM6DSRX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSRX_ReadID(&lsm6dsrx_obj_0, &id) != LSM6DSRX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSRX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSRX_GetCapabilities(&lsm6dsrx_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSRX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSRX_0] = &lsm6dsrx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSRX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSRX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSRX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSRX_GYRO_Driver;

      if (MotionDrv[IKS01A3_LSM6DSRX_0]->Init(MotionCompObj[IKS01A3_LSM6DSRX_0]) != LSM6DSRX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSRX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSRX_ACC_Driver;

      if (MotionDrv[IKS01A3_LSM6DSRX_0]->Init(MotionCompObj[IKS01A3_LSM6DSRX_0]) != LSM6DSRX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t ISM330DHCX_0_Probe(uint32_t Functions)
{
  ISM330DHCX_IO_t            io_ctx;
  uint8_t                    id;
  static ISM330DHCX_Object_t ism330dhcx_obj_0;
  ISM330DHCX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM330DHCX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DHCX_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (ISM330DHCX_RegisterBusIO(&ism330dhcx_obj_0, &io_ctx) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DHCX_ReadID(&ism330dhcx_obj_0, &id) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM330DHCX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM330DHCX_GetCapabilities(&ism330dhcx_obj_0, &cap);
    MotionCtx[IKS01A3_ISM330DHCX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_ISM330DHCX_0] = &ism330dhcx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_ISM330DHCX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330DHCX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ISM330DHCX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330DHCX_GYRO_Driver;

      if (MotionDrv[IKS01A3_ISM330DHCX_0]->Init(MotionCompObj[IKS01A3_ISM330DHCX_0]) != ISM330DHCX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_ISM330DHCX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&ISM330DHCX_ACC_Driver;

      if (MotionDrv[IKS01A3_ISM330DHCX_0]->Init(MotionCompObj[IKS01A3_ISM330DHCX_0]) != ISM330DHCX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSO32_0_Probe(uint32_t Functions)
{
  LSM6DSO32_IO_t            io_ctx;
  uint8_t                   id;
  static LSM6DSO32_Object_t lsm6dso32_obj_0;
  LSM6DSO32_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSO32_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO32_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LSM6DSO32_RegisterBusIO(&lsm6dso32_obj_0, &io_ctx) != LSM6DSO32_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO32_ReadID(&lsm6dso32_obj_0, &id) != LSM6DSO32_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSO32_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSO32_GetCapabilities(&lsm6dso32_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSO32_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSO32_0] = &lsm6dso32_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSO32_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO32_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSO32_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSO32_GYRO_Driver;

      if (MotionDrv[IKS01A3_LSM6DSO32_0]->Init(MotionCompObj[IKS01A3_LSM6DSO32_0]) != LSM6DSO32_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSO32_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSO32_ACC_Driver;

      if (MotionDrv[IKS01A3_LSM6DSO32_0]->Init(MotionCompObj[IKS01A3_LSM6DSO32_0]) != LSM6DSO32_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t IIS2ICLX_0_Probe(uint32_t Functions)
{
  IIS2ICLX_IO_t             io_ctx;
  uint8_t                   id;
  static IIS2ICLX_Object_t  iis2iclx_obj_0;
  IIS2ICLX_Capabilities_t   cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = IIS2ICLX_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2ICLX_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (IIS2ICLX_RegisterBusIO(&iis2iclx_obj_0, &io_ctx) != IIS2ICLX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2ICLX_ReadID(&iis2iclx_obj_0, &id) != IIS2ICLX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != IIS2ICLX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)IIS2ICLX_GetCapabilities(&iis2iclx_obj_0, &cap);
    MotionCtx[IKS01A3_IIS2ICLX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_IIS2ICLX_0] = &iis2iclx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_IIS2ICLX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2ICLX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_IIS2ICLX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&IIS2ICLX_ACC_Driver;

      if (MotionDrv[IKS01A3_IIS2ICLX_0]->Init(MotionCompObj[IKS01A3_IIS2ICLX_0]) != IIS2ICLX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t AIS2IH_0_Probe(uint32_t Functions)
{
  AIS2IH_IO_t             io_ctx;
  uint8_t                 id;
  static AIS2IH_Object_t  AIS2IH_obj_0;
  AIS2IH_Capabilities_t   cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = AIS2IH_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS2IH_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (AIS2IH_RegisterBusIO(&AIS2IH_obj_0, &io_ctx) != AIS2IH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS2IH_ReadID(&AIS2IH_obj_0, &id) != AIS2IH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != AIS2IH_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS2IH_GetCapabilities(&AIS2IH_obj_0, &cap);
    MotionCtx[IKS01A3_AIS2IH_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_AIS2IH_0] = &AIS2IH_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_AIS2IH_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS2IH_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_AIS2IH_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&AIS2IH_ACC_Driver;

      if (MotionDrv[IKS01A3_AIS2IH_0]->Init(MotionCompObj[IKS01A3_AIS2IH_0]) != AIS2IH_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSO32X_0_Probe(uint32_t Functions)
{
  LSM6DSO32X_IO_t            io_ctx;
  uint8_t                    id;
  static LSM6DSO32X_Object_t lsm6dso32x_obj_0;
  LSM6DSO32X_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSO32X_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO32X_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LSM6DSO32X_RegisterBusIO(&lsm6dso32x_obj_0, &io_ctx) != LSM6DSO32X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO32X_ReadID(&lsm6dso32x_obj_0, &id) != LSM6DSO32X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSO32X_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSO32X_GetCapabilities(&lsm6dso32x_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSO32X_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSO32X_0] = &lsm6dso32x_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSO32X_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO32X_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSO32X_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSO32X_GYRO_Driver;

      if (MotionDrv[IKS01A3_LSM6DSO32X_0]->Init(MotionCompObj[IKS01A3_LSM6DSO32X_0]) != LSM6DSO32X_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSO32X_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSO32X_ACC_Driver;

      if (MotionDrv[IKS01A3_LSM6DSO32X_0]->Init(MotionCompObj[IKS01A3_LSM6DSO32X_0]) != LSM6DSO32X_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0  == 1)
/**
 * @brief  Register Bus IOs for instance 1 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSOX_SENSORHUB_LIS2MDL_0_Probe(uint32_t Functions)
{
  LIS2MDL_IO_t            io_ctx;
  uint8_t                 id;
  static LIS2MDL_Object_t lis2mdl_obj_0;
  LIS2MDL_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSOX_SENSORHUB_LIS2MDL_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSOX_I2C_ADD_L;
  io_ctx.Init        = IKS01A3_I2C_Init;
  io_ctx.DeInit      = IKS01A3_I2C_DeInit;
  io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
  io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
  io_ctx.GetTick     = IKS01A3_GetTick;

  if (LIS2MDL_RegisterBusIO(&lis2mdl_obj_0, &io_ctx) != LIS2MDL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2MDL_ReadID(&lis2mdl_obj_0, &id) != LIS2MDL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2MDL_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2MDL_GetCapabilities(&lis2mdl_obj_0, &cap);
    MotionCtx[IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0] = &lis2mdl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2MDL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO) && (cap.Magneto == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LIS2MDL_MAG_Driver;

      if (MotionDrv[IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0]->Init(MotionCompObj[IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0]) != LIS2MDL_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

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
