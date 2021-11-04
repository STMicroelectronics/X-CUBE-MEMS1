/**
  ******************************************************************************
  * @file    motion_vc.h
  * @author  MEMS Application Team
  * @version V1.2.1
  * @date    24-August-2021
  * @brief   Header for motion_vc module
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
#ifndef _MOTION_VC_H_
#define _MOTION_VC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_VC MOTION_VC
  * @{
  */


/** @defgroup MOTION_VC_Exported_Types MOTION_VC_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
  float Press;          /* Atmospheric pressure in [hPa] */
} MVC_input_t;

typedef enum
{
  MVC_UNKNOWN,          /* Unknown - In case of no pressure sensor data or reliable data */
  MVC_FLOOR,            /* OnFloor - When walking on flat surface */
  MVC_UPDOWN,           /* UpDown - If significant change observed in height but not sufficinet to declare as stairs or elevator */
  MVC_STAIRS,           /* Stairs - Detection of stairs */
  MVC_ELEVATOR,         /* Elevator */
  MVC_ESCALATOR         /* Escalator - Detect Escalator provided no walking on escalator */
} MVC_context_t;

typedef enum
{
  MVC_CONFIDENCE_UNKNOWN,
  MVC_CONFIDENCE_POOR,
  MVC_CONFIDENCE_MED,
  MVC_CONFIDENCE_HIGH
} MVC_confidence_t;

typedef struct
{
  int16_t Speed;        /* Vertical speed in [cm/s] */
  uint16_t Speed_Error; /* Vertical speed error in [cm/s] */
} MVC_speed_t;

typedef struct
{
  uint32_t Timestamp;           /* Timestamp */
  uint8_t Valid;                /* Indicates if results is valid or not */
  int32_t Baro_Altitude;        /* Computed altitude with standard formula in [cm]*/
  int32_t Cal_Altitude;         /* Calibrated altitude with drift correction in [cm] */
  MVC_speed_t Speed;            /* Vertical speed */
  MVC_context_t Context;        /* Vertical context */
  MVC_confidence_t Confidence;  /* Condfidence in context */
  uint32_t NSteps;              /* Number of steps */
} MVC_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_VC_Exported_Functions MOTION_VC_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionVC engine
 * @param  none
 * @retval none
 */
void MotionVC_Initialize(void);

/**
 * @brief  Run vertical context algorithm
 * @param  data_in  pointer to accaleration in [g] and atmospheric pressure in [hPa]
 * @param  data_out  pointer to vertical context output structure
 * @retval none
 */
void MotionVC_Update(MVC_input_t *data_in, MVC_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionVC_GetLibVersion(char *version);


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

#endif /* _MOTION_VC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
