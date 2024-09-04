/**
  ******************************************************************************
  * @file    motion_sd.h
  * @author  MEMS Application Team
  * @version V1.3.0
  * @date    30-April-2024
  * @brief   Header for motion_sd module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_SD_H_
#define _MOTION_SD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_SD MOTION_SD
  * @{
  */


/** @defgroup MOTION_SD_Exported_Types MOTION_SD_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
  float Press;          /* Atmospheric pressure in [hPa] */
} MSD_input_t;

typedef enum
{
  MSD_UNKNOWN_DESK       = 0x00,
  MSD_SITTING_DESK       = 0x01,
  MSD_STANDING_DESK      = 0x02
} MSD_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_SD_Exported_Functions MOTION_SD_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionSD engine
 * @param  none
 * @retval none
 */
void MotionSD_Initialize(void);

/**
 * @brief  Set the MotionSD accelerometer data orientation
 * @param  *acc_orientation: reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  none
 */
void MotionSD_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Run standing vs sitting desk detection algorithm
 * @param  data_in  pointer to accaleration in [g] and atmospheric pressure in [hPa]
 * @param  data_out  pointer to position flag
 * @retval none
 */
void MotionSD_Update(MSD_input_t *data_in, MSD_output_t *data_out);

/**
 * @brief  Reset standing vs sitting desk detection algorithm
 * @param  none
 * @retval none
 */
void MotionSD_Reset(void);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionSD_GetLibVersion(char *version);


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

#endif /* _MOTION_SD_H_ */
