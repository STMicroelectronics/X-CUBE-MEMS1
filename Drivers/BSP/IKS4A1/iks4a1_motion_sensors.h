/**
  ******************************************************************************
  * @file    iks4a1_motion_sensors.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS4A1_MOTION_SENSOR_H
#define IKS4A1_MOTION_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks4a1_conf.h"
#include "motion_sensor.h"

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0                  1
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0
#define USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0                  1
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0
#define USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0                     1
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0                 1
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0                     0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0
#define USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0
#define USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0                   0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0
#define USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0
#define USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0                     0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0
#define USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0               0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0
#define USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0               0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0
#define USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0                   0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0
#define USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0
#define USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0
#define USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0                     0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0                     0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_A3G4250D_0
#define USE_IKS4A1_MOTION_SENSOR_A3G4250D_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0
#define USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0
#define USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0                   0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0
#define USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0                  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0
#define USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0                  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0                   0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0
#define USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_AIS2IH_0
#define USE_IKS4A1_MOTION_SENSOR_AIS2IH_0                      0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0                  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0
#define USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0
#define USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0                  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0                 0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0                     0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0                  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0
#define USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0                   0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0                  0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_ISM330BX_0
#define USE_IKS4A1_MOTION_SENSOR_ISM330BX_0                    0
#endif

#ifndef USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0
#define USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0                  0
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
#include "lsm6dso.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
#include "lis2dw12.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
#include "lis2mdl.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
#include "asm330lhh.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
#include "iis2dlpc.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
#include "iis2mdc.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1) || (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
#include "ism303dac.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
#include "ism330dlc.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
#include "lis2dh12.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
#include "lsm6dsox.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
#include "ais2dw12.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
#include "lis3mdl.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
#include "lsm6dsr.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
#include "a3g4250d.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
#include "ais328dq.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
#include "ais3624dq.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
#include "h3lis331dl.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
#include "lsm6dsrx.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
#include "ism330dhcx.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
#include "lsm6dso32.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
#include "iis2iclx.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
#include "ais2ih.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
#include "lsm6dso32x.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
#include "lis2du12.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
#include "asm330lhhx.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
#include "lsm6dsv16x.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
#include "lsm6dsv16bx.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
#include "lsm6dsv.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
#include "lsm6dsv16b.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
#include "lis2dux12.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
#include "lis2duxs12.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
#include "lsm6dsv32x.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
#include "lsm6dso16is.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330BX_0 == 1)
#include "ism330bx.h"
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0 == 1)
#include "lsm6dsv80x.h"
#endif

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS4A1 IKS4A1
  * @{
  */

/** @addtogroup IKS4A1_MOTION_SENSOR IKS4A1 MOTION SENSOR
  * @{
  */

/** @defgroup IKS4A1_MOTION_SENSOR_Exported_Types IKS4A1 MOTION SENSOR Exported Types
  * @{
  */

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IKS4A1_MOTION_SENSOR_Axes_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IKS4A1_MOTION_SENSOR_AxesRaw_t;

/* Motion Sensor instance Info */
typedef struct
{
  uint8_t  Acc;
  uint8_t  Gyro;
  uint8_t  Magneto;
  uint8_t  LowPower;
  uint32_t GyroMaxFS;
  uint32_t AccMaxFS;
  uint32_t MagMaxFS;
  float_t  GyroMaxOdr;
  float_t  AccMaxOdr;
  float_t  MagMaxOdr;
} IKS4A1_MOTION_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS4A1_MOTION_SENSOR_Ctx_t;

/**
  * @}
  */

/** @defgroup IKS4A1_MOTION_SENSOR_Exported_Constants IKS4A1 MOTION SENSOR Exported Constants
  * @{
  */

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 == 1)
#define IKS4A1_LSM6DSO_0 0
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 == 1)
#define IKS4A1_LIS2DW12_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 == 1)
#define IKS4A1_LIS2MDL_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 == 1)
#define IKS4A1_ASM330LHH_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 == 1)
#define IKS4A1_IIS2DLPC_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 == 1)
#define IKS4A1_IIS2MDC_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                          USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                          USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
#define IKS4A1_ISM303DAC_ACC_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                                USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                                USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                                USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                                USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                                USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
#define IKS4A1_ISM303DAC_MAG_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                                USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                                USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                                USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                                USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                                USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                                USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 == 1)
#define IKS4A1_ISM330DLC_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 == 1)
#define IKS4A1_LIS2DH12_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 == 1)
#define IKS4A1_LSM6DSOX_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 == 1)
#define IKS4A1_AIS2DW12_0 (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 == 1)
#define IKS4A1_LIS3MDL_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 == 1)
#define IKS4A1_LSM6DSR_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 == 1)
#define IKS4A1_A3G4250D_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 == 1)
#define IKS4A1_AIS328DQ_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                            USE_IKS4A1_MOTION_SENSOR_A3G4250D_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 == 1)
#define IKS4A1_AIS3624DQ_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                             USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 == 1)
#define IKS4A1_H3LIS331DL_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 == 1)
#define IKS4A1_LSM6DSRX_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                            USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 == 1)
#define IKS4A1_ISM330DHCX_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 == 1)
#define IKS4A1_LSM6DSO32_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                             USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                             USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 == 1)
#define IKS4A1_IIS2ICLX_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                            USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 == 1)
#define IKS4A1_AIS2IH_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                          USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                          USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                          USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                          USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                          USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                          USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                          USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                          USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                          USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                          USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                          USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                          USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                          USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                          USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 == 1)
#define IKS4A1_LSM6DSO32X_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
#define IKS4A1_LSM6DSOX_SENSORHUB_LIS2MDL_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 == 1)
#define IKS4A1_LIS2DU12_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                            USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 == 1)
#define IKS4A1_ASM330LHHX_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 == 1)
#define IKS4A1_LSM6DSV16X_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 == 1)
#define IKS4A1_LSM6DSV16BX_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                               USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                               USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                               USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                               USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 == 1)
#define IKS4A1_LSM6DSV_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                           USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                           USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                           USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                           USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                           USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                           USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                           USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 == 1)
#define IKS4A1_LSM6DSV16B_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 == 1)
#define IKS4A1_LIS2DUX12_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                             USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                             USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                             USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                             USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                             USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 == 1)
#define IKS4A1_LIS2DUXS12_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 == 1)
#define IKS4A1_LSM6DSV32X_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 == 1)
#define IKS4A1_LSM6DSO16IS_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                               USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                               USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                               USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                               USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 + \
                               USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_ISM330BX_0 == 1)
#define IKS4A1_ISM330BX_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                            USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                            USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                            USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 + \
                            USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0)
#endif

#if (USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0 == 1)
#define IKS4A1_LSM6DSV80X_0  (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                              USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 + \
                              USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 + \
                              USE_IKS4A1_MOTION_SENSOR_ISM330BX_0)
#endif

#ifndef MOTION_GYRO
#define MOTION_GYRO             1U
#endif
#ifndef MOTION_ACCELERO
#define MOTION_ACCELERO         2U
#endif
#ifndef MOTION_MAGNETO
#define MOTION_MAGNETO          4U
#endif

#define IKS4A1_MOTION_FUNCTIONS_NBR    3U
#define IKS4A1_MOTION_INSTANCES_NBR    (USE_IKS4A1_MOTION_SENSOR_LSM6DSO_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS2DW12_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS2MDL_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ASM330LHH_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_IIS2DLPC_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_IIS2MDC_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ISM330DLC_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS2DH12_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_AIS2DW12_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS3MDL_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSR_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_A3G4250D_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_AIS328DQ_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_AIS3624DQ_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_H3LIS331DL_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSRX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ISM330DHCX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSO32_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_IIS2ICLX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_AIS2IH_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSO32X_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS2DU12_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ASM330LHHX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSV16X_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSV16BX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSV_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSV16B_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS2DUX12_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LIS2DUXS12_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSV32X_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSO16IS_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_ISM330BX_0 + \
                                        USE_IKS4A1_MOTION_SENSOR_LSM6DSV80X_0)

#if (IKS4A1_MOTION_INSTANCES_NBR == 0)
#error "No motion sensor instance has been selected"
#endif

/**
  * @}
  */

/** @addtogroup IKS4A1_MOTION_SENSOR_Exported_Functions IKS4A1_MOTION_SENSOR Exported Functions
  * @{
  */

int32_t IKS4A1_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS4A1_MOTION_SENSOR_DeInit(uint32_t Instance);
int32_t IKS4A1_MOTION_SENSOR_GetCapabilities(uint32_t Instance, IKS4A1_MOTION_SENSOR_Capabilities_t *Capabilities);
int32_t IKS4A1_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS4A1_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS4A1_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_Axes_t *Axes);
int32_t IKS4A1_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS4A1_MOTION_SENSOR_AxesRaw_t *Axes);
int32_t IKS4A1_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float_t *Sensitivity);
int32_t IKS4A1_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float_t *Odr);
int32_t IKS4A1_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float_t Odr);
int32_t IKS4A1_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale);
int32_t IKS4A1_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale);

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

#ifdef __cplusplus
}
#endif

#endif /* IKS4A1_MOTION_SENSOR_H */
