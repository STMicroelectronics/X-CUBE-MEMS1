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
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
