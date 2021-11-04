/**
  ******************************************************************************
  * @file    motion_ad.h
  * @author  MEMS Application Team
  * @version V1.1.1
  * @date    24-August-2021
  * @brief   Header for motion_ad module
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
#ifndef _MOTION_AD_H_
#define _MOTION_AD_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_AD MOTION_AD
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_AD_Exported_Types MOTION_AD_Exported_Types
  * @{
  */

typedef struct
{
  float Acc[3];        /* Acceleration in X, Y, Z axis in [g] */
  unsigned int Press;  /* Pressure in [Pa] (e.g. 101325) */
  float Temp;          /* Temperature in [degC] */
} MAD_input_t;

typedef enum
{
  MAD_ONLAND  = 0x00,
  MAD_TAKEOFF = 0x01,
  MAD_LANDING = 0x02
} MAD_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AD_Exported_Functions MOTION_AD_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionAD engine
  * @param  xl_odr  accelerometer ODR in Hz (nearrest int)
  * @retval None
  */
void MotionAD_Initialize(int xl_odr);

/**
  * @brief  Run airplane mode algorithm
  * @param  data_in   pointer to acceleration [g], pressure [hBar] and temperature [degC]
  * @param  data_out  pointer to algorithm result (ONLAND, TAKEOFF, LANDING)
  * @retval None
  */
void MotionAD_Update(MAD_input_t *data_in, MAD_output_t *data_out);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionAD_GetLibVersion(char *version);

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

#endif /* _MOTION_AD_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
