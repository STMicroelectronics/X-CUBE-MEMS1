/**
  ******************************************************************************
  * @file    motion_fa.h
  * @author  MEMS Application Team
  * @version V1.3.1
  * @date    24-August-2021
  * @brief   Header for motion_fa module
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
#ifndef _MOTION_FA_H_
#define _MOTION_FA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_FA MOTION_FA
  * @{
  */


/** @defgroup MOTION_FA_Exported_Types MOTION_FA_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/
#define NUM_FA_ACTIVITIES 4

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
  float GyrX;           /* Angular velocity in X axis in [dps] */
  float GyrY;           /* Angular velocity in Y axis in [dps] */
  float GyrZ;           /* Angular velocity in Z axis in [dps] */
  float Press;          /* Atmospheric pressure in [hPa] */
} MFA_input_t;

typedef enum
{
  MFA_NOACTIVITY          = 0x00,
  MFA_BICEPCURL           = 0x01,
  MFA_SQUAT               = 0x02,
  MFA_PUSHUP              = 0x03
} MFA_activity_t;

typedef struct {
  uint32_t Counter;
} MFA_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_FA_Exported_Functions MOTION_FA_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFA engine
 * @param  none
 * @retval none
 */
void MotionFA_Initialize(void);

/**
 * @brief  Set the MotionFA accelerometer data orientation
 * @param  *acc_orientation: reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  none
 */
void MotionFA_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Set the MotionFA gyroscope data orientation
 * @param  *gyr_orientation: reference system of the gyroscope raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  none
 */
void MotionFA_SetOrientation_Gyr(const char *gyr_orientation);

/**
 * @brief  Run bicep curl counter
 * @param  data_in  pointer to accaleration in [g] and atmospheric pressure in [hPa]
 * @param  data_out  pointer to bicep curl counter
 * @retval none
 */
void MotionFA_BicepCurl_Update(MFA_input_t *data_in, MFA_output_t *data_out);

/**
 * @brief  Run squat counter
 * @param  data_in  pointer to accaleration in [g] and atmospheric pressure in [hPa]
 * @param  data_out  pointer to squat counter
 * @retval none
 */
void MotionFA_Squat_Update(MFA_input_t *data_in, MFA_output_t *data_out);

/**
 * @brief  Run pushup counter
 * @param  data_in  pointer to accaleration in [g] and atmospheric pressure in [hPa]
 * @param  data_out  pointer to pushup counter
 * @retval none
 */
void MotionFA_Pushup_Update(MFA_input_t *data_in, MFA_output_t *data_out);

/**
 * @brief  Reset bicep curl counter
 * @param  none
 * @retval none
 */
void MotionFA_BicepCurl_Reset(void);

/**
 * @brief  Reset squat counter
 * @param  none
 * @retval none
 */
void MotionFA_Squat_Reset(void);

/**
 * @brief  Reset pushup counter
 * @param  none
 * @retval none
 */
void MotionFA_Pushup_Reset(void);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionFA_GetLibVersion(char *version);


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

#endif /* _MOTION_FA_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
