/**
  ******************************************************************************
  * @file    motion_tl.h
  * @author  MEMS Application Team
  * @version V1.4.1
  * @date    24-August-2021
  * @brief   Header for motion_tl module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_TL_H_
#define _MOTION_TL_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_TL MOTION_TL
  * @{
  */

/** @defgroup MOTION_TL_Exported_Types MOTION_TL_Exported_Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float acc_x; /* Acceleration in X axis in [g] */
  float acc_y; /* Acceleration in Y axis in [g] */
  float acc_z; /* Acceleration in Z axis in [g] */
} MTL_input_t;

typedef struct
{
  float theta_3x; /* Angle about X axis */
  float psi_3x;   /* Angle about Y axis */
  float phi_3x;   /* Angle about gravity axis */
  float roll_3x;  /* Angle about X axis */
  float pitch_3x; /* Angle about X axis */
  float err_deg;  /* Expected error in output */
  uint8_t valid;  /* 0 invalid, 1 valid */
} MTL_output_t;

typedef enum
{
  MODE_PITCH_ROLL_GRAVITY_INCLINATION, /* Euler angle output (tilt_3x) */
  MODE_THETA_PSI_PHI,                  /* Individual axis output (theta, psi and phi _3x) */
} MTL_angle_mode_t;

typedef struct
{
  float fullscale;       /* FS of XL in [g] */
  float k;               /* Smoothing factor */
  char orn[3];           /* Orientation of sensor */
  MTL_angle_mode_t mode; /* Angle computation mode */
} MTL_knobs_t;

typedef struct
{
  float offset[3];
  float gain[3];
} MTL_acc_cal_t;

typedef enum
{
  X_UP,
  X_DOWN,
  Y_UP,
  Y_DOWN,
  Z_UP,
  Z_DOWN
} MTL_cal_position_t;

typedef enum
{
  CAL_PASS,  /* Calibration passed */
  CAL_NONE,  /* Calibration not finished or not performed at all */
  CAL_FAIL   /* Calibration failed */
} MTL_cal_result_t;

typedef enum
{
  MTL_MCU_STM32 = 0,
  MTL_MCU_BLUE_NRG1,
  MTL_MCU_BLUE_NRG2,
  MTL_MCU_BLUE_NRG_LP,
} MTL_mcu_type_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_TL_Exported_Functions MOTION_TL_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionTL engine
  * @param  mcu_type MCU type
  * @param  acc_orientation  reference system of the accelerometer raw data
  * @retval None
  */
void MotionTL_Initialize(MTL_mcu_type_t mcu_type, const char *acc_orientation);

/**
  * @brief  Set the MotionTL accelerometer data orientation
  * @param  acc_orientation  reference system of the accelerometer raw data
  * @retval None
  */
void MotionTL_SetOrientation_Acc(const char *acc_orientation);

/**
  * @brief  Run tilt algorithm
  * @param  data_in  Pointer to acceleration in [g]
  * @param  timestamp_ms  Timestamp in [ms]
  * @param  data_out  Pointer to the results
  * @retval None
  */
void MotionTL_Update(MTL_input_t *data_in, uint64_t timestamp_ms, MTL_output_t *data_out);

/**
  * @brief  Set the knobs setting. The API can be called after MotionTL_Init but before MotionTL_Update
  * @param  knobs  Knobs structure
  * @retval None
  */
void MotionTL_SetKnobs(MTL_knobs_t *knobs);

/**
  * @brief  Get the current knobs setting
  * @param  knobs  Knobs structure
  * @retval None
  */
void MotionTL_GetKnobs(MTL_knobs_t *knobs);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionTL_GetLibVersion(char *version);

/**
  * @brief  Calibrate accelerometer in specific position
  * @param  cal_data  Pointer to 2D array of calibration data cal_data[num_records][3]
  * @param  num_records  Number of calibration data records (3 axes per each record)
  * @param  cal_position  Calibration position the data belong to
  * @retval None
  */
void MotionTL_CalibratePosition(float cal_data[][3], uint32_t num_records, MTL_cal_position_t cal_position);

/**
  * @brief  Get accelerometer calibration values
  * @param  acc_cal  Pointer to calibration values structure
  * @retval Enum with calibration result
  */
MTL_cal_result_t MotionTL_GetCalValues(MTL_acc_cal_t *acc_cal);

/**
  * @brief  Set accelerometer calibration values
  * @param  acc_cal  Pointer to calibration values structure
  * @retval Enum with calibration result
  */
MTL_cal_result_t MotionTL_SetCalValues(MTL_acc_cal_t *acc_cal);

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

#endif /* _MOTION_TL_H_ */
