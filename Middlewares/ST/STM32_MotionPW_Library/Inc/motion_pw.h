/**
  ******************************************************************************
  * @file    motion_pw.h
  * @author  MEMS Application Team
  * @version V1.4.1
  * @date    24-August-2021
  * @brief   Header for motion_pw module
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
#ifndef _MOTION_PW_H_
#define _MOTION_PW_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 * @{
 */

/** @defgroup MOTION_PW MOTION_PW
 * @{
 */

/** @defgroup MOTION_PW_Exported_Types MOTION_PW_Exported_Types
 * @{
 */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MPW_UNKNOWN_ACTIVITY = 0x00,
  MPW_WALKING          = 0x01,
  MPW_FASTWALKING      = 0x02,
  MPW_JOGGING          = 0x03
} MPW_activity_t;

typedef struct
{
  float AccX;                      /* Acceleration in X axis in [g] */
  float AccY;                      /* Acceleration in Y axis in [g] */
  float AccZ;                      /* Acceleration in Z axis in [g] */
  MPW_activity_t CurrentActivity;  /* Current user activity */
} MPW_input_t;

typedef struct
{
  uint8_t Cadence;      /* [steps/min] */
  uint32_t Nsteps;
  uint8_t Confidence;
} MPW_output_t;

/**
 * @}
 */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_PW_Exported_Functions MOTION_PW_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionPW engine
 * @param  None
 * @retval None
 */
void MotionPW_Initialize(void);

/**
 * @brief  Run pedometer algorithm
 * @param  data_in  pointer to accaleration in [g]
 * @param  data_out  pointer to MPW_output_t structure
 * @retval None
 */
void MotionPW_Update(MPW_input_t *data_in, MPW_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionPW_GetLibVersion(char *version);

/**
 * @brief  Reset library
 * @param  None
 * @retval None
 */
void MotionPW_ResetPedometerLibrary(void);

/**
 * @brief  Reset step count
 * @param  None
 * @retval None
 */
void MotionPW_ResetStepCount(void);

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

#endif /* _MOTION_PW_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
