/**
  ******************************************************************************
  * @file    motion_pe.h
  * @author  MEMS Application Team
  * @version V2.2.1
  * @date    24-August-2021
  * @brief   Header for motion_pe module
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
#ifndef _MOTION_PE_H_
#define _MOTION_PE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_PE MOTION_PE
  * @{
  */

/** @defgroup MOTION_PE_Exported_Types MOTION_PE_Exported_Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float AccX; /* Acceleration in X axis in [g] */
  float AccY; /* Acceleration in Y axis in [g] */
  float AccZ; /* Acceleration in Z axis in [g] */
} MPE_input_t;

typedef enum
{
  MPE_UNKNOWN    = 0x00,
  MPE_SITTING    = 0x01,
  MPE_STANDING   = 0x02,
  MPE_LYING_DOWN = 0x03
} MPE_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_PE_Exported_Functions MOTION_PE_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionPE engine
  * @param  None
  * @retval None
  */
void MotionPE_Initialize(void);

/**
  * @brief  Set the MotionPE Accelerometer data orientation
  * @param  *acc_orientation: reference system of the Accelerometer raw data
  *         (for instance: south west up became "swu", north east up became "ned")
  * @retval  None
  */
void MotionPE_SetOrientation_Acc(const char *acc_orientation);

/**
  * @brief  Run Pose Estimation algorithm
  * @param  data_in: Structure containing input data
  * @param  data_out: Structure containing output data
  * @retval  None
  */
void MotionPE_Update(MPE_input_t *data_in, MPE_output_t *data_out);

/**
  * @brief  Reset Libray
  * @param  None
  * @retval None
  */
void MotionPE_ResetLib(void);

/**
  * @brief  Get the library version
  * @param  version: pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionPE_GetLibVersion(char *version);

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

#endif /* _MOTION_PE_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
