/**
  ******************************************************************************
  * @file    motion_id.h
  * @author  MEMS Application Team
  * @version V2.4.1
  * @date    24-August-2021
  * @brief   Header for motion_id module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016-2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_ID_H_
#define _MOTION_ID_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_ID MOTION_ID
  * @{
  */

/** @defgroup MOTION_ID_Exported_Types MOTION_ID_Exported_Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/

typedef struct
{
  float AccX; /* Acceleration in X axis in [g] */
  float AccY; /* Acceleration in Y axis in [g] */
  float AccZ; /* Acceleration in Z axis in [g] */
} MID_input_t;

typedef enum
{
  MID_ON_DESK                 = 0x00,
  MID_BED_COUCH_PILLOW        = 0x01,
  MID_LIGHT_MOVEMENTS         = 0x02,
  MID_BIKING                  = 0x03,
  MID_TYPING_WRITING          = 0x04,
  MID_HI_TYPING__SLOW_WALKING = 0x05,
  MID_WASHING_HANDS_WALKING   = 0x06,
  MID_FWALKING                = 0x07,
  MID_FWALKING_JOGGING        = 0x08,
  MID_FJOGGING_BRUSHING       = 0x09,
  MID_SPRINTING               = 0x0A
} MID_output_t;

typedef enum
{
  MID_MCU_STM32 = 0,
  MID_MCU_BLUE_NRG1,
  MID_MCU_BLUE_NRG2,
  MID_MCU_BLUE_NRG_LP,
} MID_mcu_type_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_ID_Exported_Functions MOTION_ID_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionID engine
  * @param  mcu_type MCU type
  * @retval None
  */
void MotionID_Initialize(MID_mcu_type_t mcu_type);

/**
  * @brief  Run Intensity Detection algorithm
  * @param  data_in  Structure containing input data
  * @param  data_out  Intensity Detection algorithm result
  * @retval None
  */
void MotionID_Update(MID_input_t *data_in, MID_output_t *data_out);

/**
  * @brief  Reset Libray
  * @param  None
  * @retval None
  */
void MotionID_ResetLib(void);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionID_GetLibVersion(char *version);

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

#endif /* _MOTION_ID_H_ */
