/**
  ******************************************************************************
  * @file    iks01a3_conf.h
  * @author  MEMS Application Team
  * @brief   This file contains definitions for the MEMS components bus interfaces
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

#include "stm32l1xx_hal.h"
#include "nucleo_l152re_bus.h"
#include "nucleo_l152re_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IKS01A3_CONF_H__
#define __IKS01A3_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define USE_IKS01A3_ENV_SENSOR_HTS221_0                         1U
#define USE_IKS01A3_ENV_SENSOR_LPS22HH_0                        1U
#define USE_IKS01A3_ENV_SENSOR_STTS751_0                        1U

#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO_0                     1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2DW12_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2MDL_0                     1U

#define USE_IKS01A3_ENV_SENSOR_SHT40AD1B_0                      1U

#define USE_IKS01A3_ENV_SENSOR_LPS33HW_0                        1U
#define USE_IKS01A3_ENV_SENSOR_STTS22H_0                        1U
#define USE_IKS01A3_ENV_SENSOR_LPS33K_0                         1U
#define USE_IKS01A3_ENV_SENSOR_LPS22CH_0                        1U
#define USE_IKS01A3_ENV_SENSOR_LPS27HHTW_0                      1U
#define USE_IKS01A3_ENV_SENSOR_LPS22DF_0                        1U
#define USE_IKS01A3_ENV_SENSOR_ILPS22QS_0                       1U
#define USE_IKS01A3_ENV_SENSOR_ILPS28QSW_0                      1U
#define USE_IKS01A3_ENV_SENSOR_LPS28DFW_0                       1U

#define USE_IKS01A3_MOTION_SENSOR_ASM330LHH_0                   1U
#define USE_IKS01A3_MOTION_SENSOR_IIS2DLPC_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_IIS2MDC_0                     1U
#define USE_IKS01A3_MOTION_SENSOR_ISM303DAC_ACC_0               1U
#define USE_IKS01A3_MOTION_SENSOR_ISM303DAC_MAG_0               1U
#define USE_IKS01A3_MOTION_SENSOR_ISM330DLC_0                   1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2DH12_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_AIS2DW12_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSR_0                     1U
#define USE_IKS01A3_MOTION_SENSOR_LIS3MDL_0                     1U
#define USE_IKS01A3_MOTION_SENSOR_A3G4250D_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_AIS328DQ_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_AIS3624DQ_0                   1U
#define USE_IKS01A3_MOTION_SENSOR_H3LIS331DL_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSRX_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_ISM330DHCX_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO32_0                   1U
#define USE_IKS01A3_MOTION_SENSOR_IIS2ICLX_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_AIS2IH_0                      1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSO32X_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSOX_SENSORHUB_LIS2MDL_0  1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2DU12_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_ASM330LHHX_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSV16X_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSV16BX_0                 1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSV_0                     1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSV16B_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2DUX12_0                   1U
#define USE_IKS01A3_MOTION_SENSOR_LIS2DUXS12_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSV32X_0                  1U
#define USE_IKS01A3_MOTION_SENSOR_ISM330BX_0                    1U
#define USE_IKS01A3_MOTION_SENSOR_LSM6DSV80X_0                  1U

#define USE_IKS01A3_HYBRID_SENSOR_LIS2DTW12_0                   1U

#define IKS01A3_I2C_INIT BSP_I2C1_Init
#define IKS01A3_I2C_DEINIT BSP_I2C1_DeInit
#define IKS01A3_I2C_READ_REG BSP_I2C1_ReadReg
#define IKS01A3_I2C_WRITE_REG BSP_I2C1_WriteReg
#define IKS01A3_I2C_Read BSP_I2C1_Recv
#define IKS01A3_I2C_Write BSP_I2C1_Send
#define IKS01A3_DELAY HAL_Delay
#define IKS01A3_GET_TICK BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __IKS01A3_CONF_H__*/
