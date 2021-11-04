/**
  ******************************************************************************
  * @file    motion_gr.h
  * @author  MEMS Application Team
  * @version V2.2.1
  * @date    24-August-2021
  * @brief   Header for motion_gr module
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
#ifndef _MOTION_GR_H_
#define _MOTION_GR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_GR MOTION_GR
  * @{
  */


/** @defgroup MOTION_GR_Exported_Types MOTION_GR_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
} MGR_input_t;

typedef enum
{
  MGR_NOGESTURE       = 0x00,
  MGR_PICKUP          = 0x01,
  MGR_GLANCE          = 0x02,
  MGR_WAKEUP          = 0x03
} MGR_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_GR_Exported_Functions MOTION_GR_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionGR engine
 * @param  none
 * @retval none
 */
void MotionGR_Initialize(void);

/**
 * @brief  Set the MotionGR accelerometer data orientation
 * @param  *acc_orientation: reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  none
 */
void MotionGR_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Run gesture recognition algorithm
 * @param  data_in  pointer to acceleration in [g]
 * @param  data_out  pointer to gesture index
 * @retval none
 */
void MotionGR_Update(MGR_input_t *data_in, MGR_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionGR_GetLibVersion(char *version);


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

#endif /* _MOTION_GR_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
