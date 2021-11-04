/**
  ******************************************************************************
  * @file    motion_at.h
  * @author  MEMS Application Team
  * @version V1.2.1
  * @date    24-August-2021
  * @brief   Header for motion_at module
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
#ifndef _MOTION_AT_H_
#define _MOTION_AT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 * @{
 */

/** @defgroup MOTION_AT MOTION_AT
 * @{
 */

/** @defgroup MOTION_AT_Exported_Types MOTION_AT_Exported_Types
 * @{
 */

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float acc_x; /**< @brief Acceleration in X axis in [g] */
  float acc_y; /**< @brief Acceleration in Y axis in [g] */
  float acc_z; /**< @brief Acceleration in Z axis in [g] */
} MAT_input_t;

typedef struct
{
  uint8_t active; /**< @brief 1 if currently active, 0 otherwise */
} MAT_output_t;

/**
 * @}
 */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AT_Exported_Functions MOTION_AT_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize MotionAT engine (dynamically allocate memory)
 * @param  None
 * @retval None
 */
void MotionAT_Initialize(void);

/**
 * @brief  Deinitialize MotionAT engine (free dynamically allocated memory)
 * @param  None
 * @retval None
 */
void MotionAT_Deinitialize(void);

/**
 * @brief  Run Active Time algorithm
 * @param  data_in pointer to MAT_input_t structure
 * @param  data_out pointer to MAT_output_t structure
 * @retval None
 */
void MotionAT_Update(MAT_input_t *data_in, MAT_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionAT_GetLibVersion(char *version);

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

#endif /* _MOTION_AT_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
