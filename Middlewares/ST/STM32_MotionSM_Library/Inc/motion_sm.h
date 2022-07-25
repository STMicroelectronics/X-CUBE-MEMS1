/**
  ******************************************************************************
  * @file    motion_sm.h
  * @author  MEMS Application Team
  * @version V1.0.1
  * @date    15-February-2019
  * @brief   Header for motion_sm module
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
#ifndef _MOTION_SM_H_
#define _MOTION_SM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_SM MOTION_SM
  * @{
  */

/** @defgroup MOTION_SM_Exported_Types MOTION_SM_Exported_Types
 * @{
 */

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MSM_NOSLEEP   = 0x00,
  MSM_SLEEP     = 0x01
} MSM_sleep_t;

typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
} MSM_input_t;

typedef struct
{
  MSM_sleep_t SleepFlag;
  uint32_t TotalSleepTime;
} MSM_output_t;


/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_SM_Exported_Functions MOTION_SM_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionSM engine
 * @param  none
 * @retval none
 */
void MotionSM_Initialize(void);

/**
 * @brief  Set the MotionSM accelerometer data orientation
 * @param  *acc_orientation: reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval none
 */
void MotionSM_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Run sleep monitoring algorithm
 * @param  data_in  Pointer to accaleration in [g]
 * @param  data_out pointer to output data (sleep indication, total sleep time)
 * @retval none
 */
void MotionSM_Update(MSM_input_t *data_in, MSM_output_t *data_out);

/**
 * @brief  Reset sleep monitorin algorithm
 * @param  none
 * @retval none
 */
void MotionSM_Reset(void);

/**
 * @brief  Get the library version
 * @param  version  Pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionSM_GetLibVersion(char *version);

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

#endif /* _MOTION_SM_H_ */
