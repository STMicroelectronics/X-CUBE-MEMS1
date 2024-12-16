/**
  ******************************************************************************
  * @file    motion_gc.h
  * @author  MEMS Application Team
  * @brief   Header for motion_gc module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_GC_H_
#define _MOTION_GC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_GC MOTION_GC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_GC_Exported_Types MOTION_GC_Exported_Types
  * @{
  */

typedef enum
{
  MGC_MCU_STM32 = 0,
  MGC_MCU_BLUE_NRG1,
  MGC_MCU_BLUE_NRG2,
  MGC_MCU_BLUE_NRG_LP,
  MGC_MCU_STM32WB0,
} MGC_mcu_type_t;

typedef struct
{
  float Acc[3];         /* Acceleration in X, Y, Z axis in [g] */
  float Gyro[3];        /* Angular rate along X, Y, Z axis in [dps] */
} MGC_input_t;

typedef struct
{
  float GyroBiasX;     /* Gyroscope offset value in X axis in [dps] */
  float GyroBiasY;     /* Gyroscope offset value in Y axis in [dps] */
  float GyroBiasZ;     /* Gyroscope offset value in Z axis in [dps] */
} MGC_output_t;


typedef struct
{
  float AccThr;         /* Accelerometer threshold to detect steady state in [g] in range 0.003-0.05 g,
                           default value 0.004 g */
  float GyroThr;        /* Gyroscope threshold to detect steady state in [dps] in range 0.008-0.4 dps,
                           default value 0.1 dps */
  float FilterConst;    /* Constant for internal filter [0..1], default value 0.002 */
  int   FastStart;      /* Set to 1 for fast convergence at the initialization, default value 1 */
  float MaxGyro;        /* Maximum expected angular rate offset when still in [dps], default value 15 dps */
  float MaxAcc;         /* Maximum acceleration module when still in [g], default value 1.3g */
} MGC_knobs_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_GC_Exported_Functions MOTION_GC_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionGC engine
  * @param  mcu_type MCU type
  * @param  freq  sampling frequency
  * @retval none
  */
void MotionGC_Initialize(MGC_mcu_type_t mcu_type, float *freq);

/**
  * @brief  Get the knobs setting of the library
  * @param  knobs  pointer to knobs setting structure
  * @retval none
  */
void MotionGC_GetKnobs(MGC_knobs_t *knobs);

/**
  * @brief  Set the knobs setting of the library
  * @param  knobs  pointer to knobs setting structure
  * @retval none
  */
void MotionGC_SetKnobs(MGC_knobs_t *knobs);

/**
  * @brief  Run gyroscope calibration algorithm and return compensation parameters
  * @param  data_in  pointer to acceleration [g] and angular rate values [dps]
  * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
  * @param  bias_update  pointer to an integer that is set to 1 if the gyroscope bias was updated, 0 otherwise
  * @retval none
  */
void MotionGC_Update(MGC_input_t *data_in, MGC_output_t *gyro_bias, int *bias_update);

/**
  * @brief  Get the gyroscope compensation parameters
  * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
  * @retval none
  */
void MotionGC_GetCalParams(MGC_output_t *gyro_bias);

/**
  * @brief  Set the initial gyroscope compensation parameters
  * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
  * @retval none
  */
void MotionGC_SetCalParams(MGC_output_t *gyro_bias);

/**
  * @brief  Set new sample frequency
  * @param  freq  new sample frequency in [Hz]
  * @retval none
  */
void MotionGC_SetFrequency(float *freq);


/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionGC_GetLibVersion(char *version);

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

#endif /* _MOTION_GC_H_ */
