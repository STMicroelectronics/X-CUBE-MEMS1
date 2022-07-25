/**
  ******************************************************************************
  * @file    sensor_def.h
  * @author  Martin Polacek
  * @brief   header for sensor_def.c.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_DEF_H
#define __SENSOR_DEF_H


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Typedefs ------------------------------------------------------------------*/
typedef enum
{
  X_AXIS = 0,
  Y_AXIS,
  Z_AXIS,
  ALL_ACTIVE
} ACTIVE_AXIS_t;

typedef struct
{
  const char *name;
  const float *odr_list;
  const int32_t *fs_list;
  const ACTIVE_AXIS_t *axis_list;
  const uint32_t *samples_list;
  const uint8_t hp_filter_available;
} sensor_setting_t;

/* Exported variables --------------------------------------------------------*/
extern const sensor_setting_t SensorSetting;

#endif /* __SENSOR_DEF_H */
