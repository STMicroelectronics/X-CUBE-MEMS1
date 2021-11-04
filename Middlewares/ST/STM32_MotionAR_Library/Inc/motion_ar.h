/**
  ******************************************************************************
  * @file    motion_ar.h
  * @author  MEMS Application Team
  * @version V3.2.1
  * @date    24-August-2021
  * @brief   Header for motion_ar module
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
#ifndef _MOTION_AR_H_
#define _MOTION_AR_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_AR MOTION_AR
  * @{
  */

/** @defgroup MOTION_AR_Exported_Types MOTION_AR_Exported_Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float acc_x;        /* Acceleration in X axis in [g] */
  float acc_y;        /* Acceleration in Y axis in [g] */
  float acc_z;        /* Acceleration in Z axis in [g] */
} MAR_input_t;

typedef enum
{
  MAR_NOACTIVITY  = 0x00,
  MAR_STATIONARY  = 0x01,
  MAR_WALKING     = 0x02,
  MAR_FASTWALKING = 0x03,
  MAR_JOGGING     = 0x04,
  MAR_BIKING      = 0x05,
  MAR_DRIVING     = 0x06
} MAR_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AR_Exported_Functions MOTION_AR_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionAR engine
  * @param  None
  * @retval None
  */
void MotionAR_Initialize(void);

/**
  * @brief  Set the MotionAR accelerometer data orientation
  * @param  acc_orientation  reference system of the accelerometer raw data
  * (for instance: south west up became "swu", north east up became "ned")
  * @retval None
  */
void MotionAR_SetOrientation_Acc(const char *acc_orientation);

/**
  * @brief  Run Activity Recognition algorithm
  * @param  data_in  Pointer to acceleration in [g]
  * @param  data_out  Pointer to activity index
  * @param  timestamp  Timestamp in [ms]
  * @retval None
  */
void MotionAR_Update(MAR_input_t *data_in, MAR_output_t *data_out, int64_t timestamp);

/**
  * @brief  Reset Activity Recognition algorithm
  * @param  None
  * @retval None
  */
void MotionAR_Reset(void);

/**
  * @brief  Get the library version
  * @param  version  Pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionAR_GetLibVersion(char *version);

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

#endif /* _MOTION_AR_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
