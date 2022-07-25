/**
  ******************************************************************************
  * @file    motion_tl2.h
  * @author  MEMS Application Team
  * @version V1.3.1
  * @date    24-August-2021
  * @brief   Header for motion_tl2 module
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
#ifndef MOTION_TL2_H_
#define MOTION_TL2_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_TL2 MOTION_TL2
  * @{
  */

/** @defgroup MOTION_TL2_Exported_Types MOTION_TL2_Exported_Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MTL2_SINGLE_PLANE, /* Single Plane output (tilt_1x) */
  MTL2_DUAL_PLANE,   /* Dual Plane output (theta, psi and phi _2x) */
} MTL2_MODE_t;

typedef struct
{
  float acc_x; /* Acceleration in X axis in [g] */
  float acc_y; /* Acceleration in Y axis in [g] */
} MTL2_input_t;

typedef struct
{
  float fullscale;  /* FS of XL in [g] */
  float k;          /* Smoothing factor */
  char orn[2];      /* Orientation of sensor */
  MTL2_MODE_t mode; /* Angle computation mode */
} MTL2_knobs_t;

typedef struct
{
  float theta_2x; /* Angle about X axis */
  float psi_2x;   /* Angle about Y axis */
  float phi_2x;   /* Angle about gravity axis */
  float tilt_1x;  /* Angle about X axis */
  float err_deg;  /* Expected error in output */
  uint8_t valid;  /* 0 invalid, 1 valid */
} MTL2_output_t;

typedef enum
{
  MTL2_MCU_STM32 = 0,
  MTL2_MCU_BLUE_NRG1,
  MTL2_MCU_BLUE_NRG2,
  MTL2_MCU_BLUE_NRG_LP,
} MTL2_mcu_type_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_TL2_Exported_Functions MOTION_TL2_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  Initialize the MotionTL2 engine
  * @param  mcu_type MCU type
  * @retval None
  */
void MotionTL2_Init(MTL2_mcu_type_t mcu_type);

/**
  * @brief  Run tilt algorithm
  * @param  data_in  Pointer to acceleration in [g]
  * @param  timestamp_ms  Timestamp in [ms]
  * @param  data_out  Pointer to the results
  * @retval None
  */
void MotionTL2_Update(MTL2_input_t *data_in, uint64_t timestamp_ms, MTL2_output_t *data_out);

/**
  * @brief  Set the knobs setting. The API can be called after MotionTL2_Init but before MotionTL2_Update
  * @param  knobs  Knobs structure
  * @retval None
  */
void MotionTL2_SetKnobs(MTL2_knobs_t *knobs);

/**
  * @brief  Get the current knobs setting
  * @param  knobs  Knobs structure
  * @retval None
  */
void MotionTL2_GetKnobs(MTL2_knobs_t *knobs);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionTL2_GetLibVersion(char *version);

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

#endif /* MOTION_TL2_H_ */
