/**
  ******************************************************************************
  * @file    motion_di.h
  * @author  MEMS Application Team
  * @version V1.1.1
  * @date    10-August-2021
  * @brief   Header for motion_di module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_DI_H_
#define _MOTION_DI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
#define MDI_NUM_AXES    3
#define MDI_QNUM_AXES   4

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_DI MOTION_DI
  * @{
  */

/** @defgroup MOTION_DI_Exported_Types MOTION_DI_Exported_Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MDI_ENGINE_DISABLE = 0,
  MDI_ENGINE_ENABLE = 1
} MDI_engine_state_t;

typedef enum
{
  MDI_ENGINE_OUTPUT_NED = 0,
  MDI_ENGINE_OUTPUT_ENU = 1
} MDI_engine_output_ref_sys_t;

typedef enum
{
  MDI_CAL_NONE = 0,
  MDI_CAL_ONETIME = 1,
  MDI_CAL_CONTINUOUS = 2
} MDI_cal_type_t;

typedef struct
{
  int64_t Timestamp; /* Timestamp of current sample in [us] */
  float Acc[3];      /* Acceleration in X, Y, Z axis in [g] */
  float Gyro[3];     /* Angular rate along X, Y, Z axis in [dps] */
} MDI_input_t;

typedef struct
{
  float AccThr;          /* Accelerometer threshold to detect steady state in [g] in range 0.003-0.05 g, defualt value 0.004 g */
  float GyroThr;         /* Gyroscope threshold to detect steady state in [dps] in range 0.008-0.4 dps , default value 0.1 dps */
  float MaxGyro;         /* Maximum expected angular rate offset when still in [dps], default value 15 dps */
  float GBiasDiffAVTh;
  float GBiasDiffVelTh;
  MDI_cal_type_t CalType; /* 0: No Cal, 1: oneTime Cal, 2: Continuous Cal (default) */
} MDI_gyro_cal_knobs_t;

typedef struct
{
  float MoveThresh_g;     /* Recommended between 0.15 - 0.30g, higher value will relax condition on data selection
                             for calibration but reduce the accuracy which will be around (moveThresh_g / 10) */
  MDI_cal_type_t CalType; /* 0: No Cal, 1: oneTime Cal (default), 2: Continuous Cal */
} MDI_acc_cal_knobs_t;

typedef struct
{
  float ATime;                              /* merge rate to the accel */
  float FrTime;                             /* merge rate to the accel when external accelerations occours */
  unsigned char modx;                       /* setting to indicate the decimation, set to 1 in smartphone/tablet, set to >=1 in embedded solutions */
  MDI_engine_output_ref_sys_t output_type;  /* 0: NED, 1: ENU */
} MDI_fusion_knobs_t;

typedef struct
{
  MDI_acc_cal_knobs_t AccKnob;              /* Knobs Settings for accelerometer calibration */
  MDI_gyro_cal_knobs_t GyrKnob;             /* Knobs Settings for gyroscope calibration */
  MDI_fusion_knobs_t SFKnob;                /* Knobs Settings for Sensor Fusion */
  char AccOrientation[MDI_QNUM_AXES];      /* accelerometer data orientation */
  char GyroOrientation[MDI_QNUM_AXES];     /* gyroscope data orientation */
} MDI_knobs_t;

typedef struct
{
  float rotation[MDI_NUM_AXES];            /* 6 axes yaw, pitch and roll */
  float quaternion[MDI_QNUM_AXES];         /* 6 axes quaternion */
  float gravity[MDI_NUM_AXES];             /* 6 axes device frame gravity */
  float linear_acceleration[MDI_NUM_AXES]; /* 6 axes device frame linear acceleration */
} MDI_output_t;

typedef enum
{
  MDI_CAL_UNKNOWN = 0,
  MDI_CAL_POOR,
  MDI_CAL_OK,
  MDI_CAL_GOOD
} MDI_cal_quality_t;

typedef struct {
  float Bias[3];                /* Bias/offset array dps, g */
  float SF_Matrix[3][3];        /* Scale factor correction matrix (diagonal matrix) */
  MDI_cal_quality_t CalQuality; /* Calibration quality factor */
} MDI_cal_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_DI_Exported_Functions
  * @{
  */

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize the MOTION_DI engine
  * @param  freq frequency of input data
  * @retval none
  */
void MotionDI_Initialize(float *freq);

/**
  * @brief  Run-time knobs change
  * @param  knobs knobs structure
  * @retval none
  */
void MotionDI_setKnobs(MDI_knobs_t *knobs);

/**
  * @brief  Return current internal knobs
  * @param  knobs pointer to knobs structure
  * @retval none
  */
void MotionDI_getKnobs(MDI_knobs_t *knobs);

/**
  * @brief  Get accelerometer calibration parameters
  * @param  acc_cal  structure containing calibration data
  * @retval none
  */
void MotionDI_AccCal_getParams(MDI_cal_output_t *acc_cal);

/**
  * @brief  Set accelerometer calibration parameters
  * @param  acc_cal  structure containing calibration data
  * @retval none
  */
void MotionDI_AccCal_setParams(MDI_cal_output_t *acc_cal);
    
/**
  * @brief  Restart accelerometer calibration algorithm
  * @param  none
  * @retval none
  */
void MotionDI_AccCal_reset(void);

/**
  * @brief  Get gyroscope calibration parameters
  * @param  gyro_cal  structure containing calibration data
  * @retval none
  */
void MotionDI_GyrCal_getParams(MDI_cal_output_t *gyro_cal);

/**
  * @brief  Set the initial gyroscope compensation parameters
  * @param  gyro_cal  structure containing calibration data
  * @retval none
  */
void MotionDI_GyrCal_setParams(MDI_cal_output_t *gyro_cal);

/**
  * @brief  Restart gyroscope calibration algorithm
  * @param  none
  * @retval none
  */
void MotionDI_GyrCal_reset(void);

/**
  * @brief  Update the MotionDI engine
  * @param  data_out pointer to the MDI_output structure
  * @param  data_in pointer to the MDI_input structure
  * @retval none
  */
void MotionDI_update(MDI_output_t *data_out, MDI_input_t *data_in);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionDI_GetLibVersion(char *version);

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

#endif /* _MOTION_DI_H_ */
