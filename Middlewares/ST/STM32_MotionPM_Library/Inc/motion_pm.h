/**
  ******************************************************************************
  * @file    motion_pm.h
  * @author  MEMS Application Team
  * @version V2.4.1
  * @date    24-August-2021
  * @brief   Header for motion_pm module
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
#ifndef _MOTION_PM_H_
#define _MOTION_PM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_PM MOTION_PM
  * @{
  */

/** @defgroup MOTION_PM_Exported_Types MOTION_PM_Exported_Types
 * @{
 */

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
} MPM_input_t;

typedef struct
{
  uint8_t Cadence;      /* [steps/min] */
  uint32_t Nsteps;
} MPM_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_PM_Exported_Functions MOTION_PM_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionPM engine
 * @param  none
 * @retval none
 */
void MotionPM_Initialize(void);

/**
 * @brief  Run pedometer algorithm
 * @param  data_in  pointer to accaleration in [g]
 * @param  data_out  pointer to MPM_output_t structure
 * @retval none
 */
void MotionPM_Update(MPM_input_t *data_in, MPM_output_t *data_out);

/**
 * @brief  Reset step count
 * @param  none
 * @retval none
 */
void MotionPM_ResetStepCount(void);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionPM_GetLibVersion(char *version);


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

#endif /* _MOTION_PM_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
