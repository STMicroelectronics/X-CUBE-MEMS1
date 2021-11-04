/**
 ******************************************************************************
 * @file    custom_mems_conf_app.h
 * @author  MEMS Application Team
 * @brief   Custom sensor configuration file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSL_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSL_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSO_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSOX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSOX_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ASM330LHH_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ASM330LHH_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM330DLC_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ISM330DLC_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID ISM330DHCX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_ISM330DHCX_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSR_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSR_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSRX_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSRX_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSO32_0
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
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32X_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_GYR_INSTANCE_0
      #define CUSTOM_GYR_INSTANCE_0 CUSTOM_LSM6DSO32X_0
      #ifndef ACC_GYR_UNICLEO_ID
        #define ACC_GYR_UNICLEO_ID LSM6DSO32X_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef USE_CUSTOM_ENV_SENSOR_HTS221_0
  #if (USE_CUSTOM_ENV_SENSOR_HTS221_0 == 1)
    #ifndef CUSTOM_HUM_INSTANCE_0
      #define CUSTOM_HUM_INSTANCE_0 CUSTOM_HTS221_0
      #ifndef HUM_TEMP_UNICLEO_ID
        #define HUM_TEMP_UNICLEO_ID HTS221_UNICLEO_ID
      #endif
    #endif
    #ifndef CUSTOM_TEMP_INSTANCE_0
      #define CUSTOM_TEMP_INSTANCE_0 CUSTOM_HTS221_0
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
      #ifndef PRESS_UNICLEO_ID
        #define PRESS_UNICLEO_ID LPS27HHTW_UNICLEO_ID
      #endif
    #endif
  #endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CUSTOM_MEMS_CONF_APP_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
