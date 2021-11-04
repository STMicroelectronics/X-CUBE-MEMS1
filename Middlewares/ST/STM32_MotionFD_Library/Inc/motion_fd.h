/**
  ******************************************************************************
  * @file    motion_fd.h
  * @author  MEMS Application Team
  * @version V1.2.1
  * @date    24-August-2021
  * @brief   Header for motion_fd module
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
#ifndef _MOTION_FD_H_
#define _MOTION_FD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_FD MOTION_FD
  * @{
  */


/** @defgroup MOTION_FD_Exported_Types MOTION_FD_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [mg] */
  float AccY;           /* Acceleration in Y axis in [mg] */
  float AccZ;           /* Acceleration in Z axis in [mg] */
  float Press;          /* Atmospheric pressure in [hPa]  */
} MFD_input_t;

typedef enum
{
  MFD_NOFALL            = 0x00,
  MFD_FALL              = 0x01
} MFD_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_FD_Exported_Functions MOTION_FD_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFD engine
 * @param  none
 * @retval none
 */
void MotionFD_Initialize(void);

/**
 * @brief  Run fall detection algorithm
 * @param  data_in  pointer to acceleration in [g] and atmospheric pressure in [hPa]
 * @param  data_out  pointer to fall detection flag
 * @retval none
 */
void MotionFD_Update(MFD_input_t *data_in, MFD_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionFD_GetLibVersion(char *version);

/**
 * @brief  Get knobs
 * @param  fall_threshold  pointer to fall acceleration threshold in [mg]
 * @param  fall_altitude_delta  pointer to fall altitude delta in [cm]
 * @param  lying_time  pointer to lying time in [s]
 * @retval none
 */
void MotionFD_GetKnobs(float *fall_threshold, int32_t *fall_altitude_delta, float *lying_time);

/**
 * @brief  Set knobs
 * @param  fall_threshold  fall acceleration threshold in [mg]
 * @param  fall_altitude_delta  fall altitude delta in [cm]
 * @param  lying_time  lying time in [s]
 * @retval none
 */
void MotionFD_SetKnobs(float fall_threshold, int32_t fall_altitude_delta, float lying_time);

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

#endif /* _MOTION_FD_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
