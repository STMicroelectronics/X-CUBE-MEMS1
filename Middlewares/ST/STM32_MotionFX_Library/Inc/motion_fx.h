/**
  ******************************************************************************
  * @file    motion_fx.h
  * @author  MEMS Application Team
  * @version V2.7.1
  * @date    03-March-2023
  * @brief   Header for motion_fx module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015-2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_FX_H_
#define _MOTION_FX_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 * @{
 */

/** @defgroup MOTION_FX MOTION_FX
 * @{
 */

/* Exported constants --------------------------------------------------------*/
#define MFX_NUM_AXES    3
#define MFX_QNUM_AXES   4

/* Exported types ------------------------------------------------------------*/
typedef void *MFXState_t;

typedef enum
{
  MFX_ENGINE_DISABLE = 0,
  MFX_ENGINE_ENABLE = 1
} MFX_engine_state_t;

typedef enum
{
  MFX_ENGINE_6X = 0,
  MFX_ENGINE_9X = 1
} MFX_engine_fusionType_t;

typedef enum
{
  MFX_ENGINE_OUTPUT_NED = 0,
  MFX_ENGINE_OUTPUT_ENU = 1
} MFX_engine_output_ref_sys;

typedef struct
{
  float ATime;                             /* merge rate to the accel */
  float MTime;                             /* merge rate to the mag */
  float FrTime;                            /* merge rate to the accel when external accelerations occurs */
  unsigned char LMode;                     /* gyro bias learn mode: 1-static learning, 2-dynamic learning */
  float gbias_mag_th_sc;                   /* scaler for the gyro bias mag threshold nominal */
  float gbias_acc_th_sc;                   /* scaler for the gyro bias acc threshold nominal */
  float gbias_gyro_th_sc;                  /* scaler for the gyro bias gyro threshold nominal */
  unsigned char modx;                      /* setting to indicate the decimation, set to 1 in smartphone/tablet, set to >=1 in embedded solutions */
  char acc_orientation[MFX_QNUM_AXES];     /* accelerometer data orientation */
  char gyro_orientation[MFX_QNUM_AXES];    /* gyroscope data orientation */
  char mag_orientation[MFX_QNUM_AXES];     /* magnetometer data orientation */
  MFX_engine_output_ref_sys output_type;   /* 0: NED, 1: ENU */
  int start_automatic_gbias_calculation;
} MFX_knobs_t;

typedef struct
{
  float mag[MFX_NUM_AXES];                 /* Calibrated mag [uT/50] */
  float acc[MFX_NUM_AXES];                 /* Acceleration in [g] */
  float gyro[MFX_NUM_AXES];                /* Angular rate [dps] */
} MFX_input_t;

typedef struct
{
  float rotation[MFX_NUM_AXES];            /* yaw, pitch and roll */
  float quaternion[MFX_QNUM_AXES];         /* quaternion */
  float gravity[MFX_NUM_AXES];             /* device frame gravity */
  float linear_acceleration[MFX_NUM_AXES]; /* device frame linear acceleration */
  float heading;                           /* heading */
  float headingErr;                        /* heading error in deg */
} MFX_output_t;

typedef enum
{
  MFX_MAGCALUNKNOWN = 0,
  MFX_MAGCALPOOR,
  MFX_MAGCALOK,
  MFX_MAGCALGOOD
} MFX_MagCal_quality_t;

typedef struct {
  float mag[MFX_NUM_AXES];                 /* Uncalibrated mag [uT/50] */
  int time_stamp;                          /* Timestamp [ms] */
} MFX_MagCal_input_t;

typedef struct {
  float hi_bias[3];                        /* Hard iron offset array [uT/50] */
  MFX_MagCal_quality_t cal_quality;        /* Calibration quality factor */
} MFX_MagCal_output_t;

/**
 * @}
 */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_FX_Exported_Functions MOTION_FX_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Provide the size of motionFX to allocate
 * @param  void
 * @retval size_t require for each instances
 * This API should be called to get the size to allocate in RAM/stack each instances. MFXState_t state pointer should point to same memory.
 */
size_t MotionFX_GetStateSize(void);

/**
 * @brief  Initialize the MotionFX engine
 * @param  mfxstate_pt Allocated memory for single instance of engine state
 * @retval none
 */
void MotionFX_initialize(MFXState_t mfxstate_pt);

/**
 * @brief  Set the internal knobs
 * @param  knobs knobs structure
 * @retval None
 */
void MotionFX_setKnobs(MFXState_t mfxstate_pt, MFX_knobs_t *knobs);

/**
 * @brief  Get the current internal knobs
 * @param  knobs knobs structure
 * @retval None
 */
void MotionFX_getKnobs(MFXState_t mfxstate_pt, MFX_knobs_t *knobs);

/**
 * @brief  Get the status of the 6 axes library
 * @retval 1 if enabled, 0 if disabled
 */
MFX_engine_state_t MotionFX_getStatus_6X(MFXState_t mfxstate_pt);

/**
 * @brief  Get the status of the 9 axes library
 * @retval 1 if enabled, 0 if disabled
 */
MFX_engine_state_t MotionFX_getStatus_9X(MFXState_t mfxstate_pt);

/**
 * @brief  Enable or disable the 6 axes function (ACC + GYRO)
 * @param  enable 1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_enable_6X(MFXState_t mfxstate_pt, MFX_engine_state_t enable);

/**
 * @brief  Enable or disable the 9 axes function (ACC + GYRO + MAG)
 * @param  enable 1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_enable_9X(MFXState_t mfxstate_pt, MFX_engine_state_t enable);

/**
 * @brief  Set the initial gbias in dps in enu frame
 * @param  gbias pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_setGbias(MFXState_t mfxstate_pt, float *gbias);

/**
 * @brief  Get the initial gbias
 * @param  pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_getGbias(MFXState_t mfxstate_pt, float *gbias);

/**
 * @brief  Run the Kalman filter update
 * @param  data_out pointer to the MFX_output_t structure
 * @param  data_in pointer to the MFX_input_t structure
 * @param  eml_deltatime delta time between two propagate calls [sec]
 * @param  eml_q_update set to NULL
 * @retval none
 */
void MotionFX_update(MFXState_t mfxstate_pt,MFX_output_t *data_out, MFX_input_t *data_in, float *eml_deltatime, float *eml_q_update);

/**
 * @brief  Run the Kalman filter propagate
 * @param  data_out pointer to the MFX_output_t structure
 * @param  data_in pointer to the MFX_input_t structure
 * @param  eml_deltatime delta time between two propagate calls [sec]
 * @retval none
 */
void MotionFX_propagate(MFXState_t mfxstate_pt, MFX_output_t *data_out, MFX_input_t *data_in, float *eml_deltatime);

/**
 * @brief  Initialize magnetic calibration library
 * @param  sampletime  period in milliseconds [ms] between the update function call
 * @param  enable  enable (1) or disable (0) library
 * @retval none
 */
void MotionFX_MagCal_init(int sampletime, unsigned short int enable);

/**
 * @brief  Run magnetic calibration algorithm
 * @param  data_in  structure containing input data
 * @retval none
 */
void MotionFX_MagCal_run(MFX_MagCal_input_t *data_in);

/**
 * @brief  Get magnetic calibration parameters
 * @param  data_out  structure containing output data
 * @retval none
 */
void MotionFX_MagCal_getParams(MFX_MagCal_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionFX_GetLibVersion(char *version);

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

#endif /* _MOTION_FX_H_ */
