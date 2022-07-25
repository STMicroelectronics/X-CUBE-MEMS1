/**
  ******************************************************************************
  * @file    custom_mems_conf_app.h
  * @author  MEMS Application Team
  * @brief   Custom sensor configuration file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_MEMS_CONF_APP_H__
#define __CUSTOM_MEMS_CONF_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sensor_unicleo_id.h"

/* Exported defines ----------------------------------------------------------*/
#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSL_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSL_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSL_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSL"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSL_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSL_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSL"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {5, 125, 250, 500, 1000, 2000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSL_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM303AGR_ACC_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM303AGR_ACC_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM303AGR_ACC_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM303AGR"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {7, 1, 10, 25, 50, 100, 200, 400} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM303AGR_UNICLEO_ID_ACC
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM303AGR_MAG_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM303AGR_MAG_0 == 1)
    #ifndef CUSTOM_MAG_INSTANCE_0
      #define CUSTOM_MAG_INSTANCE_0 CUSTOM_LSM303AGR_MAG_0
      #define CUSTOM_MAG_INSTANCE_NAME_0 "LSM303AGR"
      #define CUSTOM_MAG_INSTANCE_ODR_0 {4, 10, 20, 50, 100} /* Hz */
      #define CUSTOM_MAG_INSTANCE_FS_0 {1, 50} /* Ga */
      #ifndef MAG_UNICLEO_ID
        #define MAG_UNICLEO_ID LSM303AGR_UNICLEO_ID_MAG
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LIS3MDL_0
  #if (USE_CUSTOM_MOTION_SENSOR_LIS3MDL_0 == 1)
    #ifndef CUSTOM_MAG_INSTANCE_0
      #define CUSTOM_MAG_INSTANCE_0 CUSTOM_LIS3MDL_0
      #define CUSTOM_MAG_INSTANCE_NAME_0 "LIS3MDL"
      #define CUSTOM_MAG_INSTANCE_ODR_0 {8, 0.625, 1.25, 2.5, 5, 10, 20, 40, 80} /* Hz */
      #define CUSTOM_MAG_INSTANCE_FS_0 {4, 4, 8, 12, 16} /* Ga */
      #ifndef MAG_UNICLEO_ID
        #define MAG_UNICLEO_ID LIS3MDL_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSO_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSO_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSO"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSO_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSO"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {5, 125, 250, 500, 1000, 2000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LIS2DW12_0
  #if (USE_CUSTOM_MOTION_SENSOR_LIS2DW12_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LIS2DW12_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LIS2DW12"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {8, 12.5, 25, 50, 100, 200, 400, 800, 1600} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LIS2DW12_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_HYBRID_SENSOR_LIS2DTW12_0
  #if (USE_CUSTOM_HYBRID_SENSOR_LIS2DTW12_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LIS2DTW12_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LIS2DTW12"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {8, 12.5, 25, 50, 100, 200, 400, 800, 1600} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LIS2DTW12_UNICLEO_ID_ACC
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LIS2MDL_0
  #if (USE_CUSTOM_MOTION_SENSOR_LIS2MDL_0 == 1)
    #ifndef CUSTOM_MAG_INSTANCE_0
      #define CUSTOM_MAG_INSTANCE_0 CUSTOM_LIS2MDL_0
      #define CUSTOM_MAG_INSTANCE_NAME_0 "LIS2MDL"
      #define CUSTOM_MAG_INSTANCE_ODR_0 {4, 10, 20, 50, 100} /* Hz */
      #define CUSTOM_MAG_INSTANCE_FS_0 {1, 50} /* Ga */
      #ifndef MAG_UNICLEO_ID
        #define MAG_UNICLEO_ID LIS2MDL_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSOX_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSOX_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSOX_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSOX"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSOX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSOX_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSOX"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {5, 125, 250, 500, 1000, 2000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSOX_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LIS2DH12_0
  #if (USE_CUSTOM_MOTION_SENSOR_LIS2DH12_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LIS2DH12_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LIS2DH12"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 1, 10, 25, 50, 100, 200, 400, 1344, 1620, 5376} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LIS2DH12_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_ASM330LHH_0
  #if (USE_CUSTOM_MOTION_SENSOR_ASM330LHH_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_ASM330LHH_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "ASM330LHH"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ASM330LHH_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ASM330LHH_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "ASM330LHH"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {6, 125, 250, 500, 1000, 2000, 4000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ASM330LHH_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_ISM330DLC_0
  #if (USE_CUSTOM_MOTION_SENSOR_ISM330DLC_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_ISM330DLC_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "ISM330DLC"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM330DLC_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ISM330DLC_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "ISM330DLC"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1660, 3330, 6660} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {5, 125, 250, 500, 1000, 2000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM330DLC_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_ISM303DAC_ACC_0
  #if (USE_CUSTOM_MOTION_SENSOR_ISM303DAC_ACC_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_ISM303DAC_ACC_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "ISM303DAC"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {11, 1, 12.5, 25, 50, 100, 200, 400, 800, 1600, 3200, 6400} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM303DAC_UNICLEO_ID_ACC
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_ISM303DAC_MAG_0
  #if (USE_CUSTOM_MOTION_SENSOR_ISM303DAC_MAG_0 == 1)
    #ifndef CUSTOM_MAG_INSTANCE_0
      #define CUSTOM_MAG_INSTANCE_0 CUSTOM_ISM303DAC_MAG_0
      #define CUSTOM_MAG_INSTANCE_NAME_0 "ISM303DAC"
      #define CUSTOM_MAG_INSTANCE_ODR_0 {4, 10, 20, 50, 100} /* Hz */
      #define CUSTOM_MAG_INSTANCE_FS_0 {1, 50} /* Ga */
      #ifndef MAG_UNICLEO_ID
        #define MAG_UNICLEO_ID ISM303DAC_UNICLEO_ID_MAG
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_IIS2DLPC_0
  #if (USE_CUSTOM_MOTION_SENSOR_IIS2DLPC_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_IIS2DLPC_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "IIS2DLPC"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {8, 12.5, 25, 50, 100, 200, 400, 800, 1600} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID IIS2DLPC_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_IIS2MDC_0
  #if (USE_CUSTOM_MOTION_SENSOR_IIS2MDC_0 == 1)
    #ifndef CUSTOM_MAG_INSTANCE_0
      #define CUSTOM_MAG_INSTANCE_0 CUSTOM_IIS2MDC_0
      #define CUSTOM_MAG_INSTANCE_NAME_0 "IIS2MDC"
      #define CUSTOM_MAG_INSTANCE_ODR_0 {4, 10, 20, 50, 100} /* Hz */
      #define CUSTOM_MAG_INSTANCE_FS_0 {1, 50} /* Ga */
      #ifndef MAG_UNICLEO_ID
        #define MAG_UNICLEO_ID IIS2MDC_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0
  #if (USE_CUSTOM_MOTION_SENSOR_ISM330DHCX_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_ISM330DHCX_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "ISM330DHCX"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM330DHCX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ISM330DHCX_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "ISM330DHCX"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {6, 125, 250, 500, 1000, 2000, 4000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM330DHCX_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSR_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSR_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSR_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSR"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSR_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSR_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSR"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {6, 125, 250, 500, 1000, 2000, 4000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSR_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_AIS2DW12_0
  #if (USE_CUSTOM_MOTION_SENSOR_AIS2DW12_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_AIS2DW12_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "AIS2DW12"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {5, 1.6, 12.5, 25, 50, 100} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {2, 2, 4} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID AIS2DW12_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_A3G4250D_0
  #if (USE_CUSTOM_MOTION_SENSOR_A3G4250D_0 == 1)
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_A3G4250D_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "A3G4250D"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {4, 105, 208, 420, 840} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {1, 245} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID A3G4250D_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_AIS328DQ_0
  #if (USE_CUSTOM_MOTION_SENSOR_AIS328DQ_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_AIS328DQ_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "AIS328DQ"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {9, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {3, 2, 4, 8} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID AIS328DQ_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_AIS3624DQ_0
  #if (USE_CUSTOM_MOTION_SENSOR_AIS3624DQ_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_AIS3624DQ_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "AIS3624DQ"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {9, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {3, 6, 12, 24} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID AIS3624DQ_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_H3LIS331DL_0
  #if (USE_CUSTOM_MOTION_SENSOR_H3LIS331DL_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_H3LIS331DL_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "H3LIS331DL"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {9, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {3, 100, 200, 400} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID H3LIS331DL_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSRX_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSRX_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSRX_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSRX"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSRX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSRX_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSRX"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {6, 125, 250, 500, 1000, 2000, 4000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSRX_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSO32_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSO32_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSO32"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 4, 8, 16, 32} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSO32_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSO32"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {5, 125, 250, 500, 1000, 2000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_IIS2ICLX_0
  #if (USE_CUSTOM_MOTION_SENSOR_IIS2ICLX_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_IIS2ICLX_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "IIS2ICLX"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {7, 12.5, 26, 52, 104, 208, 416, 833} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 500, 1000, 2000, 3000} /* mg */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID IIS2ICLX_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_AIS2IH_0
  #if (USE_CUSTOM_MOTION_SENSOR_AIS2IH_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_AIS2IH_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "AIS2IH"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {9, 1.6, 12.5, 25, 50, 100, 200, 400, 800, 1600} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID AIS2IH_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0
  #if (USE_CUSTOM_MOTION_SENSOR_LSM6DSO32X_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LSM6DSO32X_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LSM6DSO32X"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {11, 1.6, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 4, 8, 16, 32} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32X_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSO32X_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "LSM6DSO32X"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {5, 125, 250, 500, 1000, 2000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32X_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_LIS2DU12_0
  #if (USE_CUSTOM_MOTION_SENSOR_LIS2DU12_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_LIS2DU12_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "LIS2DU12"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {8, 6, 12.5, 25, 50, 100, 200, 400, 800} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LIS2DU12_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_HTS221_0
  #if (USE_CUSTOM_ENV_SENSOR_HTS221_0 == 1)
    #ifndef CUSTOM_HUM_INSTANCE_0
      #define CUSTOM_HUM_INSTANCE_0 CUSTOM_HTS221_0
      #define CUSTOM_HUM_INSTANCE_NAME_0 "HTS221"
      #define CUSTOM_HUM_INSTANCE_ODR_0 {3, 1, 7, 12.5} /* Hz */
      #define CUSTOM_HUM_INSTANCE_FS_0 {0} /* % */
      #ifndef HUM_TEMP_UNICLEO_ID
        #define HUM_TEMP_UNICLEO_ID HTS221_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_TEMP_INSTANCE_0
      #define CUSTOM_TEMP_INSTANCE_0 CUSTOM_HTS221_0
      #define CUSTOM_TEMP_INSTANCE_NAME_0 "HTS221"
      #define CUSTOM_TEMP_INSTANCE_ODR_0 {3, 1, 7, 12.5} /* Hz */
      #define CUSTOM_TEMP_INSTANCE_FS_0 {0} /* C */
      #ifndef HUM_TEMP_UNICLEO_ID
        #define HUM_TEMP_UNICLEO_ID HTS221_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS22HB_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS22HB_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS22HB_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS22HB"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {5, 1, 10, 25, 50, 75} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS22HB_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS22HH_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS22HH_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS22HH_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS22HH"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {7, 1, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS22HH_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_STTS751_0
  #if (USE_CUSTOM_ENV_SENSOR_STTS751_0 == 1)
    #ifndef CUSTOM_TEMP_INSTANCE_0
      #define CUSTOM_TEMP_INSTANCE_0 CUSTOM_STTS751_0
      #define CUSTOM_TEMP_INSTANCE_NAME_0 "STTS751"
      #define CUSTOM_TEMP_INSTANCE_ODR_0 {10, 0.0625, 0.125, 0.25, 0.5, 1, 2, 4, 8, 16, 32} /* Hz */
      #define CUSTOM_TEMP_INSTANCE_FS_0 {0} /* C */
      #ifndef HUM_TEMP_UNICLEO_ID
        #define HUM_TEMP_UNICLEO_ID STTS751_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS33HW_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS33HW_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS33HW_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS33HW"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {5, 1, 10, 25, 50, 75} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS33HW_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_STTS22H_0
  #if (USE_CUSTOM_ENV_SENSOR_STTS22H_0 == 1)
    #ifndef CUSTOM_TEMP_INSTANCE_0
      #define CUSTOM_TEMP_INSTANCE_0 CUSTOM_STTS22H_0
      #define CUSTOM_TEMP_INSTANCE_NAME_0 "STTS22H"
      #define CUSTOM_TEMP_INSTANCE_ODR_0 {5, 1, 25, 50, 100, 200} /* Hz */
      #define CUSTOM_TEMP_INSTANCE_FS_0 {0} /* C */
      #ifndef HUM_TEMP_UNICLEO_ID
        #define HUM_TEMP_UNICLEO_ID STTS22H_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_HYBRID_SENSOR_LIS2DTW12_0
  #if (USE_CUSTOM_HYBRID_SENSOR_LIS2DTW12_0 == 1)
    #ifndef CUSTOM_TEMP_INSTANCE_0
      #define CUSTOM_TEMP_INSTANCE_0 CUSTOM_LIS2DTW12_0
      #define CUSTOM_TEMP_INSTANCE_NAME_0 "LIS2DTW12"
      #define CUSTOM_TEMP_INSTANCE_ODR_0 {4, 1.6, 12.5, 25, 50} /* Hz */
      #define CUSTOM_TEMP_INSTANCE_FS_0 {0} /* C */
      #ifndef HUM_TEMP_UNICLEO_ID
        #define HUM_TEMP_UNICLEO_ID LIS2DTW12_UNICLEO_ID_TEMP
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS33K_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS33K_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS33K_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS33K"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {5, 1, 10, 25, 50, 75} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS33K_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS22CH_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS22CH_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS22CH_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS22CH"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {7, 1, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS22CH_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS22DF_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS22DF_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS22DF_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS22DF"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {8, 1, 4, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS22DF_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS27HHTW_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS27HHTW_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS27HHTW_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS27HHTW"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {7, 1, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS27HHTW_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_ILPS22QS_0
  #if (USE_CUSTOM_ENV_SENSOR_ILPS22QS_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_ILPS22QS_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "ILPS22QS"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {8, 1, 4, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID ILPS22QS_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_ILPS28QSW_0
  #if (USE_CUSTOM_ENV_SENSOR_ILPS28QSW_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_ILPS28QSW_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "ILPS28QSW"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {8, 1, 4, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID ILPS28QSW_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_LPS28DFW_0
  #if (USE_CUSTOM_ENV_SENSOR_LPS28DFW_0 == 1)
    #ifndef CUSTOM_PRESS_INSTANCE_0
      #define CUSTOM_PRESS_INSTANCE_0 CUSTOM_LPS28DFW_0
      #define CUSTOM_PRESS_INSTANCE_NAME_0 "LPS28DFW"
      #define CUSTOM_PRESS_INSTANCE_ODR_0 {8, 1, 4, 10, 25, 50, 75, 100, 200} /* Hz */
      #define CUSTOM_PRESS_INSTANCE_FS_0 {0} /* Pa */
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS28DFW_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_ASM330LHHX_0
  #if (USE_CUSTOM_MOTION_SENSOR_ASM330LHHX_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_ASM330LHHX_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "ASM330LHHX"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ASM330LHHX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ASM330LHHX_0
      #define CUSTOM_GYR_INSTANCE_NAME_0 "ASM330LHHX"
      #define CUSTOM_GYR_INSTANCE_ODR_0 {10, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667} /* Hz */
      #define CUSTOM_GYR_INSTANCE_FS_0 {6, 125, 250, 500, 1000, 2000, 4000} /* dps */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ASM330LHHX_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_MOTION_SENSOR_IIS3DWB_0
  #if (USE_CUSTOM_MOTION_SENSOR_IIS3DWB_0 == 1)
    #ifndef CUSTOM_ACC_INSTANCE_0
      #define CUSTOM_ACC_INSTANCE_0 CUSTOM_IIS3DWB_0
      #define CUSTOM_ACC_INSTANCE_NAME_0 "IIS3DWB"
      #define CUSTOM_ACC_INSTANCE_ODR_0 {1, 26700} /* Hz */
      #define CUSTOM_ACC_INSTANCE_FS_0 {4, 2, 4, 8, 16} /* g */
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID IIS3DWB_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CUSTOM_MEMS_CONF_APP_H__ */
