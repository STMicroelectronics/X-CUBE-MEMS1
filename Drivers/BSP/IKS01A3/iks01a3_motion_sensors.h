/**
 ******************************************************************************
 * @file    iks01a3_motion_sensors.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS01A3_MOTION_SENSOR_H
#define IKS01A3_MOTION_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks01a3_conf.h"
#include "motion_sensor.h"

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0                     1
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0
#define USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0                    1
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0
#define USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0                     1
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0
#define USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0                   0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0
#define USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0
#define USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0                     0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0
#define USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0               0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0
#define USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0               0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0
#define USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0                   0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0
#define USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0
#define USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0
#define USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0                     0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0                     0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_A3G4250D_0
#define USE_IKS01A3_MOTION_SENSOR_A3G4250D_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0
#define USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0
#define USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0                   0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0
#define USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0                  0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0
#define USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0                  0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0                   0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0
#define USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0                    0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_AIS2IH_0
#define USE_IKS01A3_MOTION_SENSOR_AIS2IH_0                      0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0                  0
#endif

#ifndef USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0  0
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
#include "lsm6dso.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
#include "lis2dw12.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
#include "lis2mdl.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
#include "asm330lhh.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
#include "iis2dlpc.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
#include "iis2mdc.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1) || (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
#include "ism303dac.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
#include "ism330dlc.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
#include "lis2dh12.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
#include "lsm6dsox.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
#include "ais2dw12.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
#include "lis3mdl.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
#include "lsm6dsr.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
#include "a3g4250d.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
#include "ais328dq.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
#include "ais3624dq.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
#include "h3lis331dl.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
#include "lsm6dsrx.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
#include "ism330dhcx.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
#include "lsm6dso32.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
#include "iis2iclx.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
#include "ais2ih.h"
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
#include "lsm6dso32x.h"
#endif

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A3 IKS01A3
 * @{
 */

/** @addtogroup IKS01A3_MOTION_SENSOR IKS01A3 MOTION SENSOR
 * @{
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Exported_Types IKS01A3 MOTION SENSOR Exported Types
 * @{
 */

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} IKS01A3_MOTION_SENSOR_Axes_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IKS01A3_MOTION_SENSOR_AxesRaw_t;

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
  float    GyroMaxOdr;
  float    AccMaxOdr;
  float    MagMaxOdr;
} IKS01A3_MOTION_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} IKS01A3_MOTION_SENSOR_Ctx_t;

/**
 * @}
 */

/** @defgroup IKS01A3_MOTION_SENSOR_Exported_Constants IKS01A3 MOTION SENSOR Exported Constants
 * @{
 */

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 == 1)
#define IKS01A3_LSM6DSO_0 0
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 == 1)
#define IKS01A3_LIS2DW12_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 == 1)
#define IKS01A3_LIS2MDL_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 == 1)
#define IKS01A3_ASM330LHH_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 == 1)
#define IKS01A3_IIS2DLPC_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 == 1)
#define IKS01A3_IIS2MDC_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
#define IKS01A3_ISM303DAC_ACC_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
#define IKS01A3_ISM303DAC_MAG_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                                 USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 == 1)
#define IKS01A3_ISM330DLC_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 == 1)
#define IKS01A3_LIS2DH12_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 == 1)
#define IKS01A3_LSM6DSOX_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 == 1)
#define IKS01A3_AIS2DW12_0 (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 == 1)
#define IKS01A3_LIS3MDL_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 == 1)
#define IKS01A3_LSM6DSR_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                            USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                            USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                            USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 == 1)
#define IKS01A3_A3G4250D_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 == 1)
#define IKS01A3_AIS328DQ_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                             USE_IKS01A3_MOTION_SENSOR_A3G4250D_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 == 1)
#define IKS01A3_AIS3624DQ_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 == 1)
#define IKS01A3_H3LIS331DL_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                               USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 == 1)
#define IKS01A3_LSM6DSRX_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                             USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                             USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 == 1)
#define IKS01A3_ISM330DHCX_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                               USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                               USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 == 1)
#define IKS01A3_LSM6DSO32_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                              USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                              USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                              USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                              USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                              USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                              USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                              USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 + \
                              USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 == 1)
#define IKS01A3_IIS2ICLX_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                             USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                             USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                             USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 + \
                             USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 + \
                             USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 == 1)
#define IKS01A3_AIS2IH_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                           USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                           USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                           USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                           USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                           USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                           USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                           USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                           USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                           USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                           USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                           USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 + \
                           USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 + \
                           USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 + \
                           USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 == 1)
#define IKS01A3_LSM6DSO32X_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                               USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                               USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 + \
                               USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 + \
                               USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 + \
                               USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 + \
                               USE_IKS01A3_MOTION_SENSOR_AIS2IH_0)
#endif

#if (USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0 == 1)
#define IKS01A3_LSM6DSOX_SENSORHUB_LIS2MDL_0  (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 + \
                                               USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0)
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

#define IKS01A3_MOTION_FUNCTIONS_NBR    3U
#define IKS01A3_MOTION_INSTANCES_NBR    (USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_A3G4250D_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_AIS2IH_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0 + \
                                         USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0)

#if (IKS01A3_MOTION_INSTANCES_NBR == 0)
#error "No motion sensor instance has been selected"
#endif

/**
 * @}
 */

/** @addtogroup IKS01A3_MOTION_SENSOR_Exported_Functions IKS01A3_MOTION_SENSOR Exported Functions
 * @{
 */

int32_t IKS01A3_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t IKS01A3_MOTION_SENSOR_DeInit(uint32_t Instance);
int32_t IKS01A3_MOTION_SENSOR_GetCapabilities(uint32_t Instance, IKS01A3_MOTION_SENSOR_Capabilities_t *Capabilities);
int32_t IKS01A3_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t IKS01A3_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t IKS01A3_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t IKS01A3_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_Axes_t *Axes);
int32_t IKS01A3_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, IKS01A3_MOTION_SENSOR_AxesRaw_t *Axes);
int32_t IKS01A3_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float *Sensitivity);
int32_t IKS01A3_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr);
int32_t IKS01A3_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr);
int32_t IKS01A3_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale);
int32_t IKS01A3_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale);

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

#endif /* IKS01A3_MOTION_SENSOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
