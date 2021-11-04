/**
 ******************************************************************************
 * @file    sensor_def.h
 * @author  Martin Polacek
 * @brief   header for sensor_def.c.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed under Software License Agreement
 * SLA0077, (the "License"). You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
 *
 *******************************************************************************
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
