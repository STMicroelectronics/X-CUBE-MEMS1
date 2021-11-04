/**
  ******************************************************************************
  * @file    motion_fx_cm0p.h
  * @author  MEMS Application Team
  * @version V2.6.1
  * @date    10-August-2021
  * @brief   Header for motion_fx_cm0p module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement SLA0077,
  * the “License”. You may not use this component except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        www.st.com/sla0077
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_FX_CM0P_H_
#define _MOTION_FX_CM0P_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_FX_CM0P MOTION_FX_CM0P
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MFX_CM0P_MCU_STM32 = 0,
  MFX_CM0P_MCU_BLUE_NRG1,
  MFX_CM0P_MCU_BLUE_NRG2,
  MFX_CM0P_MCU_BLUE_NRG_LP,
} MFX_CM0P_mcu_type_t;

typedef enum
{
  MFX_CM0P_ENGINE_DISABLE = 0,
  MFX_CM0P_ENGINE_ENABLE = 1
} MFX_CM0P_engine_state_t;

typedef struct
{
  float mag[3];                  /* Calibrated mag [uT]/50 */
  float acc[3];                  /* Acceleration in [g] */
  float gyro[3];                 /* Angular rate [dps] */
} MFX_CM0P_input_t;

typedef struct
{
  float quaternion_9X[4];         	/* 9 axes quaternion */
  float gravity_9X[3];             	/* 9 axes device frame gravity */
  float linear_acceleration_9X[3]; 	/* 9 axes device frame linear acceleration */
  float rotation_9X[3];            	/* 9 axes yaw, pitch and roll */
  float quaternion_6X[4];         	/* 6 axes quaternion */
  float gravity_6X[3];             	/* 6 axes device frame gravity */
  float linear_acceleration_6X[3]; 	/* 6 axes device frame linear acceleration */
  float rotation_6X[3];            	/* 6 axes yaw, pitch and roll */
} MFX_CM0P_output_t;

typedef enum
{
  MFX_CM0P_CALQSTATUSUNCALIBRATED = 0,
  MFX_CM0P_CALQSTATUSPOOR,
  MFX_CM0P_CALQSTATUSGOOD,
  MFX_CM0P_CALQSTATUSBEST
} MFX_CM0P_MagCal_quality_t;

typedef struct {
  float Mag[3];  /* Magnetometer sensor output [uT/50] */
} MFX_CM0P_MagCal_input_t;

typedef struct {
  float HI_Bias[3];                      /* Hard iron offset array [uT/50] */
  MFX_CM0P_MagCal_quality_t CalQuality;  /* Calibration quality factor */
} MFX_CM0P_MagCal_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_FX_CM0P_Exported_Functions MOTION_FX_CM0P_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFX_CM0P engine
 * @param  mcu_type MCU type
 * @retval none
 */
void MotionFX_CM0P_initialize(MFX_CM0P_mcu_type_t mcu_type);

/**
 * @brief  Set sensor orientation, default orientation is ENU (x - east, y - north, z - up)
 * @param  acc_orientation string with reference to set
 * @param  gyr_orientation string with reference to set
 * @param  mag_orientation string with reference to set
 * @retval none
 */
void MotionFX_CM0P_setOrientation(const char acc_orientation[4], const char gyro_orientation[4], const char mag_orientation[4]);

/**
 * @brief  Get the status of the 6 axes library
 * @param  none
 * @retval 1 if enabled, 0 if disabled
 */
MFX_CM0P_engine_state_t MotionFX_CM0P_getStatus_6X(void);

/**
 * @brief  Get the status of the 9 axes library
 * @param  none
 * @retval 1 if enabled, 0 if disabled
 */
MFX_CM0P_engine_state_t MotionFX_CM0P_getStatus_9X(void);

/**
 * @brief  Get the status of the euler angles calculation
 * @param  none
 * @retval 1 if enabled, 0 if disabled
 */
MFX_CM0P_engine_state_t MotionFX_CM0P_getStatus_euler(void);

/**
 * @brief  Get the status of the gyroscope calibration
 * @param  none
 * @retval 1 if enabled, 0 if disabled
 */
MFX_CM0P_engine_state_t MotionFX_CM0P_getStatus_gbias(void);

/**
 * @brief  Enable or disable the 6 axes function (ACC + GYRO)
 * @param  enable 1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_CM0P_enable_6X(MFX_CM0P_engine_state_t enable);

/**
 * @brief  Enable or disable the 9 axes function (ACC + GYRO + MAG)
 * @param  enable 1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_CM0P_enable_9X(MFX_CM0P_engine_state_t enable);

/**
 * @brief  Enable or disable euler angles calculation
 * @param  1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_CM0P_enable_euler(MFX_CM0P_engine_state_t enable);

/**
 * @brief  Enable or disable gyroscope calibration
 * @param  1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_CM0P_enable_gbias(MFX_CM0P_engine_state_t enable);

/**
 * @brief  Set the initial gbias
 * @param  gbias pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_CM0P_setGbias(float *gbias);

/**
 * @brief  Get the initial gbias
 * @param  pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_CM0P_getGbias(float *gbias);

/**
 * @brief  This function runs one step of the sensor fusion algorithm
 * @param  data_out pointer to the MFX_CM0P_output structure
 * @param  data_in pointer to the MFX_CM0P_input structure
 * @param  deltatime delta time between two propagate calls [sec]
 * @retval none
 */
void MotionFX_CM0P_update(MFX_CM0P_output_t *data_out, MFX_CM0P_input_t *data_in, float deltatime);

/**
 * @brief  Initialize the compass calibration library
 * @param  sampletime  period in milliseconds [ms] between the update function call
 * @param  enable  enable (1) or disable (0) library
 * @retval none
 */
void MotionFX_CM0P_MagCal_init(int sampletime, unsigned short int enable);

/**
 * @brief  Run magnetic calibration algorithm
 * @param  data_in  structure containing input data
 * @retval none
 */
void MotionFX_CM0P_MagCal_run(MFX_CM0P_MagCal_input_t *data_in);

/**
 * @brief  Get magnetic calibration parameters
 * @param  data_out  structure containing output data
 * @retval none
 */
void MotionFX_CM0P_MagCal_getParams(MFX_CM0P_MagCal_output_t *data_out);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionFX_CM0P_GetLibVersion(char *version);

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

#endif /* _MOTION_FX_CM0P_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
