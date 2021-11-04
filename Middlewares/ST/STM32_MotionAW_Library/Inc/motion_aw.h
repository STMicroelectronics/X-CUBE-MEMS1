/**
  ******************************************************************************
  * @file    motion_aw.h
  * @author  MEMS Application Team
  * @version V2.3.1
  * @date    24-August-2021
  * @brief   Header for motion_aw module
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
#ifndef _MOTION_AW_H_
#define _MOTION_AW_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_AW MOTION_AW
  * @{
  */


/** @defgroup MOTION_AW_Exported_Types MOTION_AW_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/
#define NUM_ACTIVITIES 9

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
} MAW_input_t;

typedef enum
{
  MAW_NOACTIVITY     = 0x00,
  MAW_STATIONARY     = 0x01,
  MAW_STANDING       = 0x02,
  MAW_SITTING        = 0x03,
  MAW_LYING          = 0x04,
  MAW_WALKING        = 0x05,
  MAW_FASTWALKING    = 0x06,
  MAW_JOGGING        = 0x07,
  MAW_BIKING         = 0x08
} MAW_activity_t;

typedef struct {
  MAW_activity_t current_activity;
  uint16_t confidence;
  uint32_t current_activity_duration;
  uint32_t activity_total_duration[NUM_ACTIVITIES];
} MAW_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AW_Exported_Functions MOTION_AW_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionAW engine
 * @param  none
 * @retval none
 */
void MotionAW_Initialize(void);

/**
 * @brief  Set the MotionAW accelerometer data orientation
 * @param  *acc_orientation: reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  none
 */
void MotionAW_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Run activity recognition algorithm
 * @param  data_in  pointer to accaleration in [g]
 * @param  t  timestamp in [ms]
 * @param  data_out  pointer to output activity structure
 * @retval none
 */
void MotionAW_Update(MAW_input_t *data_in, MAW_output_t *data_out, int64_t t);

/**
 * @brief  Resets activity recognition algorithm
 * @param  none
 * @retval none
 */
void MotionAW_Reset(void);

/**
 * @brief  Resets total activity duration counters
 * @param  none
 * @retval none
 */
void MotionAW_Reset_Activity_Duration(void);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionAW_GetLibVersion(char *version);


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

#endif /* _MOTION_AW_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
