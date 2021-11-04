/**
  ******************************************************************************
  * @file    hybrid_sensor.h
  * @author  MCD Application Team
  * @brief   This header file contains the functions prototypes for the
  *          hybrid sensor driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HYBRID_SENSOR_H
#define HYBRID_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup COMPONENTS COMPONENTS
  * @{
  */

/** @addtogroup COMMON COMMON
  * @{
  */

/** @addtogroup HYBRID_SENSOR HYBRID SENSOR
 * @{
 */

/** @addtogroup HYBRID_SENSOR_Public_Types HYBRID SENSOR Public types
  * @{
  */

/**
  * @brief  HYBRID_SENSOR driver structure definition
  */
typedef struct
{
  int32_t ( *Init              ) ( void * );
  int32_t ( *DeInit            ) ( void * );
  int32_t ( *ReadID            ) ( void *, uint8_t * ); 
  int32_t ( *GetCapabilities   ) ( void *, void * );
} HYBRID_SENSOR_CommonDrv_t;

typedef struct
{
  int32_t ( *Enable            ) ( void * );
  int32_t ( *Disable           ) ( void * );
  int32_t ( *GetOutputDataRate ) ( void *, float * );
  int32_t ( *SetOutputDataRate ) ( void *, float );
  int32_t ( *GetValue          ) ( void *, float * );
} HYBRID_ENV_SENSOR_FuncDrv_t;

typedef struct
{
  int32_t ( *Enable            ) ( void * );
  int32_t ( *Disable           ) ( void * );
  int32_t ( *GetSensitivity    ) ( void *, float * );
  int32_t ( *GetOutputDataRate ) ( void *, float * );
  int32_t ( *SetOutputDataRate ) ( void *, float );
  int32_t ( *GetFullScale      ) ( void *, int32_t * );
  int32_t ( *SetFullScale      ) ( void *, int32_t );
  int32_t ( *GetAxes           ) ( void *, void * );
  int32_t ( *GetAxesRaw        ) ( void *, void * );
} HYBRID_MOTION_SENSOR_FuncDrv_t;

/**
  * @}
  */

/**
  * @}
  */

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

#endif /* HYBRID_SENSOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
