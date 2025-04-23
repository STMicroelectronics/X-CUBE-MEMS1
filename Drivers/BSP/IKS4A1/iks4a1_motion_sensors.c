/**
  ******************************************************************************
  * @file    iks4a1_motion_sensors.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the motion sensors
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
#include "iks4a1_motion_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @defgroup IKS4A1_MOTION_SENSOR IKS4A1 MOTION SENSOR
  * @{
  */

/** @defgroup IKS4A1_MOTION_SENSOR_Exported_Variables IKS4A1 MOTION SENSOR Exported Variables
  * @{
  */
extern void
*MotionCompObj[IKS4A1_MOTION_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *MotionCompObj[IKS4A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS4A1_MOTION_SENSOR_Private_Variables IKS4A1 MOTION SENSOR Private Variables
  * @{
  */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
/* But due to MISRA it has to be increased to 7 + 1. */
static uint32_t FunctionIndex[] = {0, 0, 1, 1, 2, 2, 2, 2};
static MOTION_SENSOR_FuncDrv_t *MotionFuncDrv[IKS4A1_MOTION_INSTANCES_NBR][IKS4A1_MOTION_FUNCTIONS_NBR];
static MOTION_SENSOR_CommonDrv_t *MotionDrv[IKS4A1_MOTION_INSTANCES_NBR];
static IKS4A1_MOTION_SENSOR_Ctx_t MotionCtx[IKS4A1_MOTION_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup IKS4A1_MOTION_SENSOR_Private_Function_Prototypes IKS4A1 MOTION SENSOR Private Function Prototypes
  * @{
  */

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
static int32_t LSM6DSO_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
static int32_t LIS2DW12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
static int32_t LIS2MDL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
static int32_t ASM330LHH_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
static int32_t IIS2DLPC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
static int32_t IIS2MDC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
static int32_t ISM303DAC_ACC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
static int32_t ISM303DAC_MAG_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
static int32_t ISM330DLC_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
static int32_t LIS2DH12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
static int32_t LSM6DSOX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
static int32_t AIS2DW12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
static int32_t LIS3MDL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
static int32_t LSM6DSR_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
static int32_t A3G4250D_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
static int32_t AIS328DQ_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
static int32_t AIS3624DQ_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
static int32_t H3LIS331DL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
static int32_t LSM6DSRX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
static int32_t ISM330DHCX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
static int32_t LSM6DSO32_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
static int32_t IIS2ICLX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
static int32_t AIS2IH_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
static int32_t LSM6DSO32X_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
static int32_t LSM6DSOX_SENSORHUB_LIS2MDL_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
static int32_t LIS2DU12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
static int32_t ASM330LHHX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
static int32_t LSM6DSV16X_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
static int32_t LSM6DSV16BX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
static int32_t LSM6DSV_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
static int32_t LSM6DSV16B_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
static int32_t LIS2DUX12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
static int32_t LIS2DUXS12_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
static int32_t LSM6DSV32X_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
static int32_t LSM6DSO16IS_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330BX_0 == 1)
static int32_t ISM330BX_0_Probe(uint32_t Functions);
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0 == 1)
static int32_t LSM6DSV80X_0_Probe(uint32_t Functions);
#endif

/**
  * @}
  */

/** @defgroup IKS4A1_MOTION_SENSOR_Exported_Functions IKS4A1 MOTION SENSOR Exported Functions
  * @{
  */

/**
  * @brief  Initializes the motion sensors
  * @param  Instance Motion sensor instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = MOTION_GYRO;
  uint32_t i;
  uint32_t component_functions = 0;
  IKS4A1_MOTION_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
    case IKS4A1_LSM6DSO_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
    case IKS4A1_LIS2DW12_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
    case IKS4A1_LIS2MDL_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
    case IKS4A1_ASM330LHH_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
    case IKS4A1_IIS2DLPC_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
    case IKS4A1_IIS2MDC_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    case IKS4A1_ISM303DAC_ACC_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    case IKS4A1_ISM303DAC_MAG_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
    case IKS4A1_ISM330DLC_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
    case IKS4A1_LIS2DH12_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
    case IKS4A1_LSM6DSOX_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
    case IKS4A1_AIS2DW12_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
    case IKS4A1_LIS3MDL_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
    case IKS4A1_LSM6DSR_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
    case IKS4A1_A3G4250D_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
    case IKS4A1_AIS328DQ_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
    case IKS4A1_AIS3624DQ_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
    case IKS4A1_H3LIS331DL_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
    case IKS4A1_LSM6DSRX_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case IKS4A1_ISM330DHCX_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
    case IKS4A1_LSM6DSO32_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
    case IKS4A1_IIS2ICLX_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
    case IKS4A1_AIS2IH_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    case IKS4A1_LSM6DSO32X_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
    case IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0:
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
    case IKS4A1_LIS2DU12_0:
      if (LIS2DU12_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
    case IKS4A1_ASM330LHHX_0:
      if (ASM330LHHX_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
    case IKS4A1_LSM6DSV16X_0:
      if (LSM6DSV16X_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
    case IKS4A1_LSM6DSV16BX_0:
      if (LSM6DSV16BX_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
    case IKS4A1_LSM6DSV_0:
      if (LSM6DSV_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
    case IKS4A1_LSM6DSV16B_0:
      if (LSM6DSV16B_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
    case IKS4A1_LIS2DUX12_0:
      if (LIS2DUX12_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
    case IKS4A1_LIS2DUXS12_0:
      if (LIS2DUXS12_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
    case IKS4A1_LSM6DSV32X_0:
      if (LSM6DSV32X_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
    case IKS4A1_LSM6DSO16IS_0:
      if (LSM6DSO16IS_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM330BX_0 == 1)
    case IKS4A1_ISM330BX_0:
      if (ISM330BX_0_Probe(Functions) != BSP_ERROR_NONE)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0 == 1)
    case IKS4A1_LSM6DSV80X_0:
      if (LSM6DSV80X_0_Probe(Functions) != BSP_ERROR_NONE)
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

  for (i = 0; i < IKS4A1_MOTION_FUNCTIONS_NBR; i++)
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
int32_t IKS4A1_MOTION_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
int32_t IKS4A1_MOTION_SENSOR_GetCapabilities(uint32_t Instance, IKS4A1_MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
int32_t IKS4A1_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Axes pointer to axes data structure
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Axes pointer to axes raw data structure
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Sensitivity pointer to sensitivity read value
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Odr pointer to Output Data Rate read value
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
int32_t IKS4A1_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Odr Output Data Rate value to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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
  *         - MOTION_GYRO
  *         - MOTION_ACCELERO
  *         - MOTION_MAGNETO
  * @param  Fullscale Fullscale value to be set
  * @retval BSP status
  */
int32_t IKS4A1_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  int32_t ret;

  if (Instance >= IKS4A1_MOTION_INSTANCES_NBR)
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

/** @defgroup IKS4A1_MOTION_SENSOR_Private_Functions IKS4A1 MOTION SENSOR Private Functions
  * @{
  */

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSO instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSO_0_Probe(uint32_t Functions)
{
  LSM6DSO_IO_t            io_ctx;
  uint8_t                 id;
  static LSM6DSO_Object_t lsm6dso_obj_0;
  LSM6DSO_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSO_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSO_RegisterBusIO(&lsm6dso_obj_0, &io_ctx) != LSM6DSO_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO_Set_Mem_Bank(&lsm6dso_obj_0, LSM6DSO_USER_BANK) != LSM6DSO_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO_ReadID(&lsm6dso_obj_0, &id) != LSM6DSO_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != (uint8_t)LSM6DSO_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSO_GetCapabilities(&lsm6dso_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSO_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSO_0] = &lsm6dso_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSO_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSO_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                     void *)&LSM6DSO_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO_0]->Init(MotionCompObj[IKS4A1_LSM6DSO_0]) != LSM6DSO_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSO_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                         void *)&LSM6DSO_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO_0]->Init(MotionCompObj[IKS4A1_LSM6DSO_0]) != LSM6DSO_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
/**
  * @brief  Register Bus IOs for LIS2DW12 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LIS2DW12_0_Probe(uint32_t Functions)
{
  LIS2DW12_IO_t            io_ctx;
  uint8_t                  id;
  static LIS2DW12_Object_t lis2dw12_obj_0;
  LIS2DW12_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS2DW12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DW12_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_LIS2DW12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS2DW12_0] = &lis2dw12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS2DW12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DW12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LIS2DW12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&LIS2DW12_ACC_Driver;

      if (MotionDrv[IKS4A1_LIS2DW12_0]->Init(MotionCompObj[IKS4A1_LIS2DW12_0]) != LIS2DW12_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
/**
  * @brief  Register Bus IOs for LIS2MDL instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
static int32_t LIS2MDL_0_Probe(uint32_t Functions)
{
  LIS2MDL_IO_t            io_ctx;
  uint8_t                 id;
  static LIS2MDL_Object_t lis2mdl_obj_0;
  LIS2MDL_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS2MDL_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2MDL_I2C_ADD;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_LIS2MDL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS2MDL_0] = &lis2mdl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS2MDL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2MDL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO) && (cap.Magneto == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LIS2MDL_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LIS2MDL_MAG_Driver;

      if (MotionDrv[IKS4A1_LIS2MDL_0]->Init(MotionCompObj[IKS4A1_LIS2MDL_0]) != LIS2MDL_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
/**
  * @brief  Register Bus IOs for ASM330LHH instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t ASM330LHH_0_Probe(uint32_t Functions)
{
  ASM330LHH_IO_t            io_ctx;
  uint8_t                   id;
  static ASM330LHH_Object_t asm330lhh_obj_0;
  ASM330LHH_Capabilities_t  cap;
  int32_t                   ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = ASM330LHH_I2C_BUS; /* I2C */
  io_ctx.Address     = ASM330LHH_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_ASM330LHH_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ASM330LHH_0] = &asm330lhh_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ASM330LHH_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ASM330LHH_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_ASM330LHH_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                       void *)&ASM330LHH_GYRO_Driver;

      if (MotionDrv[IKS4A1_ASM330LHH_0]->Init(MotionCompObj[IKS4A1_ASM330LHH_0]) != ASM330LHH_OK)
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
      MotionFuncDrv[IKS4A1_ASM330LHH_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                           void *)&ASM330LHH_ACC_Driver;

      if (MotionDrv[IKS4A1_ASM330LHH_0]->Init(MotionCompObj[IKS4A1_ASM330LHH_0]) != ASM330LHH_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
/**
  * @brief  Register Bus IOs for IIS2DLPC instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t IIS2DLPC_0_Probe(uint32_t Functions)
{
  IIS2DLPC_IO_t            io_ctx;
  uint8_t                  id;
  static IIS2DLPC_Object_t iis2dlpc_obj_0;
  IIS2DLPC_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = IIS2DLPC_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2DLPC_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_IIS2DLPC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_IIS2DLPC_0] = &iis2dlpc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_IIS2DLPC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2DLPC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_IIS2DLPC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&IIS2DLPC_ACC_Driver;

      if (MotionDrv[IKS4A1_IIS2DLPC_0]->Init(MotionCompObj[IKS4A1_IIS2DLPC_0]) != IIS2DLPC_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
/**
  * @brief  Register Bus IOs for IIS2MDC instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
static int32_t IIS2MDC_0_Probe(uint32_t Functions)
{
  IIS2MDC_IO_t            io_ctx;
  uint8_t                 id;
  static IIS2MDC_Object_t iis2mdc_obj_0;
  IIS2MDC_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = IIS2MDC_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2MDC_I2C_ADD;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_IIS2MDC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_IIS2MDC_0] = &iis2mdc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_IIS2MDC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2MDC_COMMON_Driver;

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
      MotionFuncDrv[IKS4A1_IIS2MDC_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&IIS2MDC_MAG_Driver;

      if (MotionDrv[IKS4A1_IIS2MDC_0]->Init(MotionCompObj[IKS4A1_IIS2MDC_0]) != IIS2MDC_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
/**
  * @brief  Register Bus IOs for ISM303DAC accelerometer instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t ISM303DAC_ACC_0_Probe(uint32_t Functions)
{
  ISM303DAC_IO_t                io_ctx;
  uint8_t                       id;
  static ISM303DAC_ACC_Object_t ism303dac_acc_obj_0;
  ISM303DAC_Capabilities_t      cap;
  int32_t                       ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = ISM303DAC_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM303DAC_I2C_ADD_XL;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_ISM303DAC_ACC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ISM303DAC_ACC_0] = &ism303dac_acc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ISM303DAC_ACC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM303DAC_ACC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_ISM303DAC_ACC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                               void *)&ISM303DAC_ACC_Driver;

      if (MotionDrv[IKS4A1_ISM303DAC_ACC_0]->Init(MotionCompObj[IKS4A1_ISM303DAC_ACC_0]) != ISM303DAC_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
/**
  * @brief  Register Bus IOs for ISM303DAC magnetometer instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
static int32_t ISM303DAC_MAG_0_Probe(uint32_t Functions)
{
  ISM303DAC_IO_t                io_ctx;
  uint8_t                       id;
  static ISM303DAC_MAG_Object_t ism303dac_mag_obj_0;
  ISM303DAC_Capabilities_t      cap;
  int32_t                       ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = ISM303DAC_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM303DAC_I2C_ADD_MG;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_ISM303DAC_MAG_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ISM303DAC_MAG_0] = &ism303dac_mag_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ISM303DAC_MAG_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM303DAC_MAG_COMMON_Driver;

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
      MotionFuncDrv[IKS4A1_ISM303DAC_MAG_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                              void *)&ISM303DAC_MAG_Driver;

      if (MotionDrv[IKS4A1_ISM303DAC_MAG_0]->Init(MotionCompObj[IKS4A1_ISM303DAC_MAG_0]) != ISM303DAC_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
/**
  * @brief  Register Bus IOs for ISM330DLC instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t ISM330DLC_0_Probe(uint32_t Functions)
{
  ISM330DLC_IO_t            io_ctx;
  uint8_t                   id;
  static ISM330DLC_Object_t ism330dlc_obj_0;
  ISM330DLC_Capabilities_t  cap;
  int32_t                   ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = ISM330DLC_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DLC_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (ISM330DLC_RegisterBusIO(&ism330dlc_obj_0, &io_ctx) != ISM330DLC_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DLC_Set_Mem_Bank(&ism330dlc_obj_0, ISM330DLC_USER_BANK) != ISM330DLC_OK)
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
    MotionCtx[IKS4A1_ISM330DLC_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ISM330DLC_0] = &ism330dlc_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ISM330DLC_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330DLC_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_ISM330DLC_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                       void *)&ISM330DLC_GYRO_Driver;

      if (MotionDrv[IKS4A1_ISM330DLC_0]->Init(MotionCompObj[IKS4A1_ISM330DLC_0]) != ISM330DLC_OK)
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
      MotionFuncDrv[IKS4A1_ISM330DLC_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                           void *)&ISM330DLC_ACC_Driver;

      if (MotionDrv[IKS4A1_ISM330DLC_0]->Init(MotionCompObj[IKS4A1_ISM330DLC_0]) != ISM330DLC_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
/**
  * @brief  Register Bus IOs for LIS2DH12 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LIS2DH12_0_Probe(uint32_t Functions)
{
  LIS2DH12_IO_t            io_ctx;
  uint8_t                  id;
  static LIS2DH12_Object_t lis2dh12_obj_0;
  LIS2DH12_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS2DH12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DH12_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_LIS2DH12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS2DH12_0] = &lis2dh12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS2DH12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DH12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LIS2DH12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&LIS2DH12_Driver;

      if (MotionDrv[IKS4A1_LIS2DH12_0]->Init(MotionCompObj[IKS4A1_LIS2DH12_0]) != LIS2DH12_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSOX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSOX_0_Probe(uint32_t Functions)
{
  LSM6DSOX_IO_t            io_ctx;
  uint8_t                  id;
  static LSM6DSOX_Object_t lsm6dsox_obj_0;
  LSM6DSOX_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSOX_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSOX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSOX_RegisterBusIO(&lsm6dsox_obj_0, &io_ctx) != LSM6DSOX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSOX_Set_Mem_Bank(&lsm6dsox_obj_0, LSM6DSOX_USER_BANK) != LSM6DSOX_OK)
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
    MotionCtx[IKS4A1_LSM6DSOX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSOX_0] = &lsm6dsox_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSOX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSOX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSOX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                      void *)&LSM6DSOX_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSOX_0]->Init(MotionCompObj[IKS4A1_LSM6DSOX_0]) != LSM6DSOX_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSOX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&LSM6DSOX_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSOX_0]->Init(MotionCompObj[IKS4A1_LSM6DSOX_0]) != LSM6DSOX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
/**
  * @brief  Register Bus IOs for AIS2DW12 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t AIS2DW12_0_Probe(uint32_t Functions)
{
  AIS2DW12_IO_t            io_ctx;
  uint8_t                  id;
  static AIS2DW12_Object_t ais2dw12_obj_0;
  AIS2DW12_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = AIS2DW12_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS2DW12_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_AIS2DW12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_AIS2DW12_0] = &ais2dw12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_AIS2DW12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS2DW12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_AIS2DW12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&AIS2DW12_ACC_Driver;

      if (MotionDrv[IKS4A1_AIS2DW12_0]->Init(MotionCompObj[IKS4A1_AIS2DW12_0]) != AIS2DW12_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
/**
  * @brief  Register Bus IOs for LIS3MDL instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_MAGNETO
  * @retval BSP status
  */
static int32_t LIS3MDL_0_Probe(uint32_t Functions)
{
  LIS3MDL_IO_t            io_ctx;
  uint8_t                 id;
  static LIS3MDL_Object_t lis3mdl_obj_0;
  LIS3MDL_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS3MDL_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS3MDL_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_LIS3MDL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS3MDL_0] = &lis3mdl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS3MDL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS3MDL_COMMON_Driver;

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
      MotionFuncDrv[IKS4A1_LIS3MDL_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LIS3MDL_MAG_Driver;

      if (MotionDrv[IKS4A1_LIS3MDL_0]->Init(MotionCompObj[IKS4A1_LIS3MDL_0]) != LIS3MDL_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSR instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSR_0_Probe(uint32_t Functions)
{
  LSM6DSR_IO_t            io_ctx;
  uint8_t                 id;
  static LSM6DSR_Object_t lsm6dsr_obj_0;
  LSM6DSR_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSR_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSR_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSR_RegisterBusIO(&lsm6dsr_obj_0, &io_ctx) != LSM6DSR_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSR_Set_Mem_Bank(&lsm6dsr_obj_0, LSM6DSR_USER_BANK) != LSM6DSR_OK)
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
    MotionCtx[IKS4A1_LSM6DSR_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSR_0] = &lsm6dsr_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSR_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSR_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSR_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                     void *)&LSM6DSR_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSR_0]->Init(MotionCompObj[IKS4A1_LSM6DSR_0]) != LSM6DSR_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSR_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                         void *)&LSM6DSR_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSR_0]->Init(MotionCompObj[IKS4A1_LSM6DSR_0]) != LSM6DSR_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
/**
  * @brief  Register Bus IOs for A3G4250D instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO
  * @retval BSP status
  */
static int32_t A3G4250D_0_Probe(uint32_t Functions)
{
  A3G4250D_IO_t            io_ctx;
  uint8_t                  id;
  static A3G4250D_Object_t a3g4250d_obj_0;
  A3G4250D_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = A3G4250D_I2C_BUS; /* I2C */
  io_ctx.Address     = A3G4250D_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_A3G4250D_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_A3G4250D_0] = &a3g4250d_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_A3G4250D_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&A3G4250D_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_A3G4250D_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                      void *)&A3G4250D_GYRO_Driver;

      if (MotionDrv[IKS4A1_A3G4250D_0]->Init(MotionCompObj[IKS4A1_A3G4250D_0]) != A3G4250D_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
/**
  * @brief  Register Bus IOs for AIS328DQ instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t AIS328DQ_0_Probe(uint32_t Functions)
{
  AIS328DQ_IO_t            io_ctx;
  uint8_t                  id;
  static AIS328DQ_Object_t ais328dq_obj_0;
  AIS328DQ_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = AIS328DQ_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS328DQ_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (AIS328DQ_RegisterBusIO(&ais328dq_obj_0, &io_ctx) != AIS328DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS328DQ_ReadID(&ais328dq_obj_0, &id) != AIS328DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != (uint8_t)AIS328DQ_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS328DQ_GetCapabilities(&ais328dq_obj_0, &cap);
    MotionCtx[IKS4A1_AIS328DQ_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_AIS328DQ_0] = &ais328dq_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_AIS328DQ_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS328DQ_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_AIS328DQ_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&AIS328DQ_ACC_Driver;

      if (MotionDrv[IKS4A1_AIS328DQ_0]->Init(MotionCompObj[IKS4A1_AIS328DQ_0]) != AIS328DQ_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
/**
  * @brief  Register Bus IOs for AIS3624DQ instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t AIS3624DQ_0_Probe(uint32_t Functions)
{
  AIS3624DQ_IO_t            io_ctx;
  uint8_t                   id;
  static AIS3624DQ_Object_t ais3624dq_obj_0;
  AIS3624DQ_Capabilities_t  cap;
  int32_t                   ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = AIS3624DQ_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS3624DQ_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (AIS3624DQ_RegisterBusIO(&ais3624dq_obj_0, &io_ctx) != AIS3624DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS3624DQ_ReadID(&ais3624dq_obj_0, &id) != AIS3624DQ_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != (uint8_t)AIS3624DQ_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS3624DQ_GetCapabilities(&ais3624dq_obj_0, &cap);
    MotionCtx[IKS4A1_AIS3624DQ_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_AIS3624DQ_0] = &ais3624dq_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_AIS3624DQ_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS3624DQ_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_AIS3624DQ_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                           void *)&AIS3624DQ_ACC_Driver;

      if (MotionDrv[IKS4A1_AIS3624DQ_0]->Init(MotionCompObj[IKS4A1_AIS3624DQ_0]) != AIS3624DQ_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
/**
  * @brief  Register Bus IOs for H3LIS331DL instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t H3LIS331DL_0_Probe(uint32_t Functions)
{
  H3LIS331DL_IO_t            io_ctx;
  uint8_t                    id;
  static H3LIS331DL_Object_t h3lis331dl_obj_0;
  H3LIS331DL_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = H3LIS331DL_I2C_BUS; /* I2C */
  io_ctx.Address     = H3LIS331DL_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (H3LIS331DL_RegisterBusIO(&h3lis331dl_obj_0, &io_ctx) != H3LIS331DL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (H3LIS331DL_ReadID(&h3lis331dl_obj_0, &id) != H3LIS331DL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != (uint8_t)H3LIS331DL_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)H3LIS331DL_GetCapabilities(&h3lis331dl_obj_0, &cap);
    MotionCtx[IKS4A1_H3LIS331DL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_H3LIS331DL_0] = &h3lis331dl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_H3LIS331DL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&H3LIS331DL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_H3LIS331DL_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&H3LIS331DL_ACC_Driver;

      if (MotionDrv[IKS4A1_H3LIS331DL_0]->Init(MotionCompObj[IKS4A1_H3LIS331DL_0]) != H3LIS331DL_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSRX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSRX_0_Probe(uint32_t Functions)
{
  LSM6DSRX_IO_t            io_ctx;
  uint8_t                  id;
  static LSM6DSRX_Object_t lsm6dsrx_obj_0;
  LSM6DSRX_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSRX_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSRX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSRX_RegisterBusIO(&lsm6dsrx_obj_0, &io_ctx) != LSM6DSRX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSRX_Set_Mem_Bank(&lsm6dsrx_obj_0, LSM6DSRX_USER_BANK) != LSM6DSRX_OK)
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
    MotionCtx[IKS4A1_LSM6DSRX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSRX_0] = &lsm6dsrx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSRX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSRX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSRX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                      void *)&LSM6DSRX_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSRX_0]->Init(MotionCompObj[IKS4A1_LSM6DSRX_0]) != LSM6DSRX_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSRX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&LSM6DSRX_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSRX_0]->Init(MotionCompObj[IKS4A1_LSM6DSRX_0]) != LSM6DSRX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
/**
  * @brief  Register Bus IOs for ISM330DHCX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t ISM330DHCX_0_Probe(uint32_t Functions)
{
  ISM330DHCX_IO_t            io_ctx;
  uint8_t                    id;
  static ISM330DHCX_Object_t ism330dhcx_obj_0;
  ISM330DHCX_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = ISM330DHCX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DHCX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (ISM330DHCX_RegisterBusIO(&ism330dhcx_obj_0, &io_ctx) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DHCX_Set_Mem_Bank(&ism330dhcx_obj_0, ISM330DHCX_USER_BANK) != ISM330DHCX_OK)
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
    MotionCtx[IKS4A1_ISM330DHCX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ISM330DHCX_0] = &ism330dhcx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ISM330DHCX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330DHCX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_ISM330DHCX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&ISM330DHCX_GYRO_Driver;

      if (MotionDrv[IKS4A1_ISM330DHCX_0]->Init(MotionCompObj[IKS4A1_ISM330DHCX_0]) != ISM330DHCX_OK)
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
      MotionFuncDrv[IKS4A1_ISM330DHCX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&ISM330DHCX_ACC_Driver;

      if (MotionDrv[IKS4A1_ISM330DHCX_0]->Init(MotionCompObj[IKS4A1_ISM330DHCX_0]) != ISM330DHCX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSO32 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSO32_0_Probe(uint32_t Functions)
{
  LSM6DSO32_IO_t            io_ctx;
  uint8_t                   id;
  static LSM6DSO32_Object_t lsm6dso32_obj_0;
  LSM6DSO32_Capabilities_t  cap;
  int32_t                   ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSO32_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO32_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSO32_RegisterBusIO(&lsm6dso32_obj_0, &io_ctx) != LSM6DSO32_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO32_Set_Mem_Bank(&lsm6dso32_obj_0, LSM6DSO32_USER_BANK) != LSM6DSO32_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO32_ReadID(&lsm6dso32_obj_0, &id) != LSM6DSO32_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != (uint8_t)LSM6DSO32_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSO32_GetCapabilities(&lsm6dso32_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSO32_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSO32_0] = &lsm6dso32_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSO32_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO32_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSO32_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                       void *)&LSM6DSO32_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO32_0]->Init(MotionCompObj[IKS4A1_LSM6DSO32_0]) != LSM6DSO32_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSO32_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                           void *)&LSM6DSO32_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO32_0]->Init(MotionCompObj[IKS4A1_LSM6DSO32_0]) != LSM6DSO32_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
/**
  * @brief  Register Bus IOs for IIS2ICLX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t IIS2ICLX_0_Probe(uint32_t Functions)
{
  IIS2ICLX_IO_t            io_ctx;
  uint8_t                  id;
  static IIS2ICLX_Object_t iis2iclx_obj_0;
  IIS2ICLX_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = IIS2ICLX_I2C_BUS; /* I2C */
  io_ctx.Address     = IIS2ICLX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (IIS2ICLX_RegisterBusIO(&iis2iclx_obj_0, &io_ctx) != IIS2ICLX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (IIS2ICLX_Set_Mem_Bank(&iis2iclx_obj_0, IIS2ICLX_USER_BANK) != IIS2ICLX_OK)
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
    MotionCtx[IKS4A1_IIS2ICLX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_IIS2ICLX_0] = &iis2iclx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_IIS2ICLX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&IIS2ICLX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_IIS2ICLX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&IIS2ICLX_ACC_Driver;

      if (MotionDrv[IKS4A1_IIS2ICLX_0]->Init(MotionCompObj[IKS4A1_IIS2ICLX_0]) != IIS2ICLX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
/**
  * @brief  Register Bus IOs for AIS2IH instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t AIS2IH_0_Probe(uint32_t Functions)
{
  AIS2IH_IO_t            io_ctx;
  uint8_t                id;
  static AIS2IH_Object_t ais2ih_obj_0;
  AIS2IH_Capabilities_t  cap;
  int32_t                ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = AIS2IH_I2C_BUS; /* I2C */
  io_ctx.Address     = AIS2IH_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (AIS2IH_RegisterBusIO(&ais2ih_obj_0, &io_ctx) != AIS2IH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (AIS2IH_ReadID(&ais2ih_obj_0, &id) != AIS2IH_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != AIS2IH_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)AIS2IH_GetCapabilities(&ais2ih_obj_0, &cap);
    MotionCtx[IKS4A1_AIS2IH_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_AIS2IH_0] = &ais2ih_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_AIS2IH_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&AIS2IH_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_AIS2IH_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&AIS2IH_ACC_Driver;

      if (MotionDrv[IKS4A1_AIS2IH_0]->Init(MotionCompObj[IKS4A1_AIS2IH_0]) != AIS2IH_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSO32X instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSO32X_0_Probe(uint32_t Functions)
{
  LSM6DSO32X_IO_t            io_ctx;
  uint8_t                    id;
  static LSM6DSO32X_Object_t lsm6dso32x_obj_0;
  LSM6DSO32X_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSO32X_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO32X_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSO32X_RegisterBusIO(&lsm6dso32x_obj_0, &io_ctx) != LSM6DSO32X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO32X_Set_Mem_Bank(&lsm6dso32x_obj_0, LSM6DSO32X_USER_BANK) != LSM6DSO32X_OK)
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
    MotionCtx[IKS4A1_LSM6DSO32X_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSO32X_0] = &lsm6dso32x_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSO32X_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO32X_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSO32X_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LSM6DSO32X_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO32X_0]->Init(MotionCompObj[IKS4A1_LSM6DSO32X_0]) != LSM6DSO32X_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSO32X_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&LSM6DSO32X_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO32X_0]->Init(MotionCompObj[IKS4A1_LSM6DSO32X_0]) != LSM6DSO32X_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSOX_SENSORHUB_LIS2MDL instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO and/or MOTION_MAGNETO
  * @retval BSP status
  */
static int32_t LSM6DSOX_SENSORHUB_LIS2MDL_0_Probe(uint32_t Functions)
{
  LIS2MDL_IO_t            io_ctx;
  uint8_t                 id;
  static LIS2MDL_Object_t lis2mdl_obj_0;
  LIS2MDL_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSOX_SENSORHUB_LIS2MDL_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSOX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

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
    MotionCtx[IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0] = &lis2mdl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2MDL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO) && (cap.Magneto == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0][FunctionIndex[MOTION_MAGNETO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                                           void *)&LIS2MDL_MAG_Driver;

      if (MotionDrv[IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0]->Init(MotionCompObj[IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0]) !=
          LIS2MDL_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
/**
  * @brief  Register Bus IOs for LIS2DU12 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LIS2DU12_0_Probe(uint32_t Functions)
{
  LIS2DU12_IO_t            io_ctx;
  uint8_t                  id;
  static LIS2DU12_Object_t lis2du12_obj_0;
  LIS2DU12_Capabilities_t  cap;
  int32_t                  ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS2DU12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DU12_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LIS2DU12_RegisterBusIO(&lis2du12_obj_0, &io_ctx) != LIS2DU12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DU12_ReadID(&lis2du12_obj_0, &id) != LIS2DU12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2DU12_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2DU12_GetCapabilities(&lis2du12_obj_0, &cap);
    MotionCtx[IKS4A1_LIS2DU12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS2DU12_0] = &lis2du12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS2DU12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DU12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LIS2DU12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                          void *)&LIS2DU12_ACC_Driver;

      if (MotionDrv[IKS4A1_LIS2DU12_0]->Init(MotionCompObj[IKS4A1_LIS2DU12_0]) != LIS2DU12_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
/**
  * @brief  Register Bus IOs for ASM330LHHX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t ASM330LHHX_0_Probe(uint32_t Functions)
{
  ASM330LHHX_IO_t            io_ctx;
  uint8_t                    id;
  static ASM330LHHX_Object_t asm330lhhx_obj_0;
  ASM330LHHX_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = ASM330LHHX_I2C_BUS; /* I2C */
  io_ctx.Address     = ASM330LHHX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (ASM330LHHX_RegisterBusIO(&asm330lhhx_obj_0, &io_ctx) != ASM330LHHX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ASM330LHHX_Set_Mem_Bank(&asm330lhhx_obj_0, ASM330LHHX_USER_BANK) != ASM330LHHX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ASM330LHHX_ReadID(&asm330lhhx_obj_0, &id) != ASM330LHHX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ASM330LHHX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ASM330LHHX_GetCapabilities(&asm330lhhx_obj_0, &cap);
    MotionCtx[IKS4A1_ASM330LHHX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ASM330LHHX_0] = &asm330lhhx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ASM330LHHX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ASM330LHHX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_ASM330LHHX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&ASM330LHHX_GYRO_Driver;

      if (MotionDrv[IKS4A1_ASM330LHHX_0]->Init(MotionCompObj[IKS4A1_ASM330LHHX_0]) != ASM330LHHX_OK)
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
      MotionFuncDrv[IKS4A1_ASM330LHHX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&ASM330LHHX_ACC_Driver;

      if (MotionDrv[IKS4A1_ASM330LHHX_0]->Init(MotionCompObj[IKS4A1_ASM330LHHX_0]) != ASM330LHHX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSV16X instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSV16X_0_Probe(uint32_t Functions)
{
  LSM6DSV16X_IO_t            io_ctx;
  uint8_t                    id;
  static LSM6DSV16X_Object_t lsm6dsv16x_obj_0;
  LSM6DSV16X_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSV16X_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSV16X_I2C_ADD_H;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSV16X_RegisterBusIO(&lsm6dsv16x_obj_0, &io_ctx) != LSM6DSV16X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV16X_Set_Mem_Bank(&lsm6dsv16x_obj_0, LSM6DSV16X_MAIN_MEM_BANK) != LSM6DSV16X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV16X_ReadID(&lsm6dsv16x_obj_0, &id) != LSM6DSV16X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSV16X_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSV16X_GetCapabilities(&lsm6dsv16x_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSV16X_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSV16X_0] = &lsm6dsv16x_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSV16X_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSV16X_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSV16X_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LSM6DSV16X_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV16X_0]->Init(MotionCompObj[IKS4A1_LSM6DSV16X_0]) != LSM6DSV16X_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSV16X_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&LSM6DSV16X_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV16X_0]->Init(MotionCompObj[IKS4A1_LSM6DSV16X_0]) != LSM6DSV16X_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSV16BX instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSV16BX_0_Probe(uint32_t Functions)
{
  LSM6DSV16BX_IO_t            io_ctx;
  uint8_t                     id;
  static LSM6DSV16BX_Object_t lsm6dsv16bx_obj_0;
  LSM6DSV16BX_Capabilities_t  cap;
  int32_t                     ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSV16BX_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSV16BX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSV16BX_RegisterBusIO(&lsm6dsv16bx_obj_0, &io_ctx) != LSM6DSV16BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV16BX_Set_Mem_Bank(&lsm6dsv16bx_obj_0, LSM6DSV16BX_MAIN_MEM_BANK) != LSM6DSV16BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV16BX_ReadID(&lsm6dsv16bx_obj_0, &id) != LSM6DSV16BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSV16BX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSV16BX_GetCapabilities(&lsm6dsv16bx_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSV16BX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSV16BX_0] = &lsm6dsv16bx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSV16BX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSV16BX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSV16BX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                         void *)&LSM6DSV16BX_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV16BX_0]->Init(MotionCompObj[IKS4A1_LSM6DSV16BX_0]) != LSM6DSV16BX_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSV16BX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                             void *)&LSM6DSV16BX_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV16BX_0]->Init(MotionCompObj[IKS4A1_LSM6DSV16BX_0]) != LSM6DSV16BX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSV instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSV_0_Probe(uint32_t Functions)
{
  LSM6DSV_IO_t            io_ctx;
  uint8_t                 id;
  static LSM6DSV_Object_t lsm6dsv_obj_0;
  LSM6DSV_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSV_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSV_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSV_RegisterBusIO(&lsm6dsv_obj_0, &io_ctx) != LSM6DSV_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV_Set_Mem_Bank(&lsm6dsv_obj_0, LSM6DSV_MAIN_MEM_BANK) != LSM6DSV_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV_ReadID(&lsm6dsv_obj_0, &id) != LSM6DSV_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSV_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSV_GetCapabilities(&lsm6dsv_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSV_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSV_0] = &lsm6dsv_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSV_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSV_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSV_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                     void *)&LSM6DSV_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV_0]->Init(MotionCompObj[IKS4A1_LSM6DSV_0]) != LSM6DSV_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSV_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                         void *)&LSM6DSV_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV_0]->Init(MotionCompObj[IKS4A1_LSM6DSV_0]) != LSM6DSV_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSV16B instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSV16B_0_Probe(uint32_t Functions)
{
  LSM6DSV16B_IO_t            io_ctx;
  uint8_t                    id;
  static LSM6DSV16B_Object_t lsm6dsv16b_obj_0;
  LSM6DSV16B_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSV16B_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSV16B_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSV16B_RegisterBusIO(&lsm6dsv16b_obj_0, &io_ctx) != LSM6DSV16B_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV16B_Set_Mem_Bank(&lsm6dsv16b_obj_0, LSM6DSV16B_MAIN_MEM_BANK) != LSM6DSV16B_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV16B_ReadID(&lsm6dsv16b_obj_0, &id) != LSM6DSV16B_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSV16B_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSV16B_GetCapabilities(&lsm6dsv16b_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSV16B_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSV16B_0] = &lsm6dsv16b_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSV16B_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSV16B_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSV16B_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LSM6DSV16B_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV16B_0]->Init(MotionCompObj[IKS4A1_LSM6DSV16B_0]) != LSM6DSV16B_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSV16B_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&LSM6DSV16B_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV16B_0]->Init(MotionCompObj[IKS4A1_LSM6DSV16B_0]) != LSM6DSV16B_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
/**
  * @brief  Register Bus IOs for LIS2DUX12 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LIS2DUX12_0_Probe(uint32_t Functions)
{
  LIS2DUX12_IO_t            io_ctx;
  uint8_t                   id;
  static LIS2DUX12_Object_t lis2dux12_obj_0;
  LIS2DUX12_Capabilities_t  cap;
  int32_t                   ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS2DUX12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DUX12_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LIS2DUX12_RegisterBusIO(&lis2dux12_obj_0, &io_ctx) != LIS2DUX12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DUX12_Set_Mem_Bank(&lis2dux12_obj_0, LIS2DUX12_MAIN_MEM_BANK) != LIS2DUX12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DUX12_ReadID(&lis2dux12_obj_0, &id) != LIS2DUX12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2DUX12_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2DUX12_GetCapabilities(&lis2dux12_obj_0, &cap);
    MotionCtx[IKS4A1_LIS2DUX12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS2DUX12_0] = &lis2dux12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS2DUX12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DUX12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LIS2DUX12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                           void *)&LIS2DUX12_ACC_Driver;

      if (MotionDrv[IKS4A1_LIS2DUX12_0]->Init(MotionCompObj[IKS4A1_LIS2DUX12_0]) != LIS2DUX12_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
/**
  * @brief  Register Bus IOs for LIS2DUXS12 instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LIS2DUXS12_0_Probe(uint32_t Functions)
{
  LIS2DUXS12_IO_t            io_ctx;
  uint8_t                    id;
  static LIS2DUXS12_Object_t lis2duxs12_obj_0;
  LIS2DUXS12_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LIS2DUXS12_I2C_BUS; /* I2C */
  io_ctx.Address     = LIS2DUXS12_I2C_ADD_H;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LIS2DUXS12_RegisterBusIO(&lis2duxs12_obj_0, &io_ctx) != LIS2DUXS12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DUXS12_Set_Mem_Bank(&lis2duxs12_obj_0, LIS2DUXS12_MAIN_MEM_BANK) != LIS2DUXS12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LIS2DUXS12_ReadID(&lis2duxs12_obj_0, &id) != LIS2DUXS12_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LIS2DUXS12_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LIS2DUXS12_GetCapabilities(&lis2duxs12_obj_0, &cap);
    MotionCtx[IKS4A1_LIS2DUXS12_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LIS2DUXS12_0] = &lis2duxs12_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LIS2DUXS12_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LIS2DUXS12_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LIS2DUXS12_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&LIS2DUXS12_ACC_Driver;

      if (MotionDrv[IKS4A1_LIS2DUXS12_0]->Init(MotionCompObj[IKS4A1_LIS2DUXS12_0]) != LIS2DUXS12_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSV32X instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSV32X_0_Probe(uint32_t Functions)
{
  LSM6DSV32X_IO_t            io_ctx;
  uint8_t                    id;
  static LSM6DSV32X_Object_t lsm6dsv32x_obj_0;
  LSM6DSV32X_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSV32X_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSV32X_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSV32X_RegisterBusIO(&lsm6dsv32x_obj_0, &io_ctx) != LSM6DSV32X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV32X_Set_Mem_Bank(&lsm6dsv32x_obj_0, LSM6DSV32X_MAIN_MEM_BANK) != LSM6DSV32X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV32X_ReadID(&lsm6dsv32x_obj_0, &id) != LSM6DSV32X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSV32X_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSV32X_GetCapabilities(&lsm6dsv32x_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSV32X_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSV32X_0] = &lsm6dsv32x_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSV32X_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSV32X_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSV32X_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LSM6DSV32X_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV32X_0]->Init(MotionCompObj[IKS4A1_LSM6DSV32X_0]) != LSM6DSV32X_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSV32X_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&LSM6DSV32X_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV32X_0]->Init(MotionCompObj[IKS4A1_LSM6DSV32X_0]) != LSM6DSV32X_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSO16IS instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSO16IS_0_Probe(uint32_t Functions)
{
  LSM6DSO16IS_IO_t            io_ctx;
  uint8_t                     id;
  static LSM6DSO16IS_Object_t lsm6dso16is_obj_0;
  LSM6DSO16IS_Capabilities_t  cap;
  int32_t                     ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSO16IS_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSO16IS_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSO16IS_RegisterBusIO(&lsm6dso16is_obj_0, &io_ctx) != LSM6DSO16IS_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO16IS_Set_Mem_Bank(&lsm6dso16is_obj_0, LSM6DSO16IS_MAIN_MEM_BANK) != LSM6DSO16IS_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSO16IS_ReadID(&lsm6dso16is_obj_0, &id) != LSM6DSO16IS_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSO16IS_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSO16IS_GetCapabilities(&lsm6dso16is_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSO16IS_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSO16IS_0] = &lsm6dso16is_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSO16IS_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSO16IS_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSO16IS_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                         void *)&LSM6DSO16IS_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO16IS_0]->Init(MotionCompObj[IKS4A1_LSM6DSO16IS_0]) != LSM6DSO16IS_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSO16IS_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                             void *)&LSM6DSO16IS_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSO16IS_0]->Init(MotionCompObj[IKS4A1_LSM6DSO16IS_0]) != LSM6DSO16IS_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_ISM330BX_0  == 1)
/**
  * @brief  Register Bus IOs for instance 0 if component ID is OK
  * @retval BSP status
  */
static int32_t ISM330BX_0_Probe(uint32_t Functions)
{
  ISM330BX_IO_t            io_ctx;
  uint8_t                  id;
  static ISM330BX_Object_t ism330bx_obj_0;
  ISM330BX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM330BX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330BX_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (ISM330BX_RegisterBusIO(&ism330bx_obj_0, &io_ctx) != ISM330BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330BX_Set_Mem_Bank(&ism330bx_obj_0, ISM330BX_MAIN_MEM_BANK) != ISM330BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330BX_ReadID(&ism330bx_obj_0, &id) != ISM330BX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM330BX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM330BX_GetCapabilities(&ism330bx_obj_0, &cap);
    MotionCtx[IKS4A1_ISM330BX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_ISM330BX_0] = &ism330bx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_ISM330BX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330BX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_ISM330BX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330BX_GYRO_Driver;

      if (MotionDrv[IKS4A1_ISM330BX_0]->Init(MotionCompObj[IKS4A1_ISM330BX_0]) != ISM330BX_OK)
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
      MotionFuncDrv[IKS4A1_ISM330BX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330BX_ACC_Driver;

      if (MotionDrv[IKS4A1_ISM330BX_0]->Init(MotionCompObj[IKS4A1_ISM330BX_0]) != ISM330BX_OK)
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

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0 == 1)
/**
  * @brief  Register Bus IOs for LSM6DSV80X instance
  * @param  Functions Motion sensor functions. Could be :
  *         - MOTION_GYRO and/or MOTION_ACCELERO
  * @retval BSP status
  */
static int32_t LSM6DSV80X_0_Probe(uint32_t Functions)
{
  LSM6DSV80X_IO_t            io_ctx;
  uint8_t                    id;
  static LSM6DSV80X_Object_t lsm6dsv80x_obj_0;
  LSM6DSV80X_Capabilities_t  cap;
  int32_t                    ret = BSP_ERROR_NONE;

  /* Configure the driver */
  io_ctx.BusType     = LSM6DSV80X_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSV80X_I2C_ADD_L;
  io_ctx.Init        = IKS4A1_I2C_INIT;
  io_ctx.DeInit      = IKS4A1_I2C_DEINIT;
  io_ctx.ReadReg     = IKS4A1_I2C_READ_REG;
  io_ctx.WriteReg    = IKS4A1_I2C_WRITE_REG;
  io_ctx.GetTick     = IKS4A1_GET_TICK;
  io_ctx.Delay       = IKS4A1_DELAY;

  if (LSM6DSV80X_RegisterBusIO(&lsm6dsv80x_obj_0, &io_ctx) != LSM6DSV80X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV80X_Set_Mem_Bank(&lsm6dsv80x_obj_0, LSM6DSV80X_MAIN_MEM_BANK) != LSM6DSV80X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSV80X_ReadID(&lsm6dsv80x_obj_0, &id) != LSM6DSV80X_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSV80X_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSV80X_GetCapabilities(&lsm6dsv80x_obj_0, &cap);
    MotionCtx[IKS4A1_LSM6DSV80X_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[IKS4A1_LSM6DSV80X_0] = &lsm6dsv80x_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[IKS4A1_LSM6DSV80X_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSV80X_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[IKS4A1_LSM6DSV80X_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                        void *)&LSM6DSV80X_GYRO_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV80X_0]->Init(MotionCompObj[IKS4A1_LSM6DSV80X_0]) != LSM6DSV80X_OK)
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
      MotionFuncDrv[IKS4A1_LSM6DSV80X_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
                                                                            void *)&LSM6DSV80X_ACC_Driver;

      if (MotionDrv[IKS4A1_LSM6DSV80X_0]->Init(MotionCompObj[IKS4A1_LSM6DSV80X_0]) != LSM6DSV80X_OK)
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
