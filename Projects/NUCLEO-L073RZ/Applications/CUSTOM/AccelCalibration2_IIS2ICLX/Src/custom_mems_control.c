/**
  ******************************************************************************
  * @file    custom_mems_control.c
  * @author  MEMS Application Team
  * @brief   This file contains the MEMS sensors interface for custom board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "custom_mems_conf.h"
#include "custom_mems_conf_app.h"
#include "custom_mems_control.h"

/**
  * @brief  Initializes accelerometer
  * @param  None
  * @retval None
  */
void BSP_SENSOR_ACC_Init(void)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_SENSOR_Init(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Init(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO);
  #endif
#endif
#endif
}

/**
  * @brief  Initializes gyroscope
  * @param  None
  * @retval None
  */
void BSP_SENSOR_GYR_Init(void)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Init(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO);
  #endif
#endif
}

/**
  * @brief  Initializes magnetometer
  * @param  None
  * @retval None
  */
void BSP_SENSOR_MAG_Init(void)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Init(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO);
  #endif
#endif
}

/**
  * @brief  Initializes pressure sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_PRESS_Init(void)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Init(CUSTOM_PRESS_INSTANCE_0, ENV_PRESSURE);
  #endif
#endif
}

/**
  * @brief  Initializes temperature sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_TEMP_Init(void)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_SENSOR_Init(CUSTOM_TEMP_INSTANCE_0, HYBRID_TEMPERATURE);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Init(CUSTOM_TEMP_INSTANCE_0, ENV_TEMPERATURE);
  #endif
#endif
#endif
}

/**
  * @brief  Initializes humidity sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_HUM_Init(void)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Init(CUSTOM_HUM_INSTANCE_0, ENV_HUMIDITY);
  #endif
#endif
}

/**
  * @brief  Enables accelerometer
  * @param  None
  * @retval None
  */
void BSP_SENSOR_ACC_Enable(void)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_Enable(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Enable(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO);
  #endif
#endif
#endif
}

/**
  * @brief  Enables gyroscope
  * @param  None
  * @retval None
  */
void BSP_SENSOR_GYR_Enable(void)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Enable(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO);
  #endif
#endif
}

/**
  * @brief  Enables magnetometer
  * @param  None
  * @retval None
  */
void BSP_SENSOR_MAG_Enable(void)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Enable(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO);
  #endif
#endif
}

/**
  * @brief  Enables pressure sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_PRESS_Enable(void)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Enable(CUSTOM_PRESS_INSTANCE_0, ENV_PRESSURE);
  #endif
#endif
}

/**
  * @brief  Enables temperature sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_TEMP_Enable(void)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_ENV_SENSOR_Enable(CUSTOM_TEMP_INSTANCE_0, HYBRID_TEMPERATURE);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Enable(CUSTOM_TEMP_INSTANCE_0, ENV_TEMPERATURE);
  #endif
#endif
#endif
}

/**
  * @brief  Enables humidity sensors
  * @param  None
  * @retval None
  */
void BSP_SENSOR_HUM_Enable(void)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Enable(CUSTOM_HUM_INSTANCE_0, ENV_HUMIDITY);
  #endif
#endif
}

/**
  * @brief  Disables accelerometer
  * @param  None
  * @retval None
  */
void BSP_SENSOR_ACC_Disable(void)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_Disable(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Disable(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO);
  #endif
#endif
#endif
}

/**
  * @brief  Disables gyroscope
  * @param  None
  * @retval None
  */
void BSP_SENSOR_GYR_Disable(void)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Disable(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO);
  #endif
#endif
}

/**
  * @brief  Disables magnetometer
  * @param  None
  * @retval None
  */
void BSP_SENSOR_MAG_Disable(void)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Disable(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO);
  #endif
#endif
}

/**
  * @brief  Disables pressure sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_PRESS_Disable(void)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Disable(CUSTOM_PRESS_INSTANCE_0, ENV_PRESSURE);
  #endif
#endif
}

/**
  * @brief  Disables temperature sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_TEMP_Disable(void)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_ENV_SENSOR_Disable(CUSTOM_TEMP_INSTANCE_0, HYBRID_TEMPERATURE);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Disable(CUSTOM_TEMP_INSTANCE_0, ENV_TEMPERATURE);
  #endif
#endif
#endif
}

/**
  * @brief  Disables humidity sensor
  * @param  None
  * @retval None
  */
void BSP_SENSOR_HUM_Disable(void)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Disable(CUSTOM_HUM_INSTANCE_0, ENV_HUMIDITY);
  #endif
#endif
}

/**
  * @brief  Get accelerometer data
  * @param  Axes pointer to axes data structure
  * @retval None
  */
void BSP_SENSOR_ACC_GetAxes(MOTION_SENSOR_Axes_t *Axes)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_GetAxes(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO, (CUSTOM_HYBRID_MOTION_SENSOR_Axes_t *)Axes);
  #else
  Axes->x = 0;
  Axes->y = 0;
  Axes->z = 0;
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetAxes(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO, (CUSTOM_MOTION_SENSOR_Axes_t *)Axes);
  #else
  Axes->x = 0;
  Axes->y = 0;
  Axes->z = 0;
  #endif
#endif
#endif
}

/**
  * @brief  Get gyroscope data
  * @param  Axes pointer to axes data structure
  * @retval None
  */
void BSP_SENSOR_GYR_GetAxes(MOTION_SENSOR_Axes_t *Axes)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetAxes(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO, (CUSTOM_MOTION_SENSOR_Axes_t *)Axes);
  #else
  Axes->x = 0;
  Axes->y = 0;
  Axes->z = 0;
  #endif
#endif
}

/**
  * @brief  Get magnetometer data
  * @param  Axes pointer to axes data structure
  * @retval None
  */
void BSP_SENSOR_MAG_GetAxes(MOTION_SENSOR_Axes_t *Axes)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetAxes(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO, (CUSTOM_MOTION_SENSOR_Axes_t *)Axes);
  #else
  Axes->x = 0;
  Axes->y = 0;
  Axes->z = 0;
  #endif
#endif
}

/**
  * @brief  Get pressure sensor data
  * @param  Value pointer to pressure value
  * @retval None
  */
void BSP_SENSOR_PRESS_GetValue(float *Value)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_GetValue(CUSTOM_PRESS_INSTANCE_0, ENV_PRESSURE, Value);
  #else
  *Value = 0.0f;
  #endif
#endif
}

/**
  * @brief  Get temperature sensor data
  * @param  Value pointer to temperature value
  * @retval None
  */
void BSP_SENSOR_TEMP_GetValue(float *Value)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_ENV_SENSOR_GetValue(CUSTOM_TEMP_INSTANCE_0, HYBRID_TEMPERATURE, Value);
  #else
  *Value = 0.0f;
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_GetValue(CUSTOM_TEMP_INSTANCE_0, ENV_TEMPERATURE, Value);
  #else
  *Value = 0.0f;
  #endif
#endif
#endif
}

/**
  * @brief  Get humidity sensor data
  * @param  Value pointer to humidity value
  * @retval None
  */
void BSP_SENSOR_HUM_GetValue(float *Value)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_GetValue(CUSTOM_HUM_INSTANCE_0, ENV_HUMIDITY, Value);
  #else
  *Value = 0.0f;
  #endif
#endif
}

/**
  * @brief  Set output data rate for accelerometer
  * @param  Odr Output Data Rate value to be set
  * @retval None
  */
void BSP_SENSOR_ACC_SetOutputDataRate(float Odr)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_SetOutputDataRate(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO, Odr);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_SetOutputDataRate(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO, Odr);
  #endif
#endif
#endif
}

/**
  * @brief  Set output data rate for gyroscope
  * @param  Odr Output Data Rate value to be set
  * @retval None
  */
void BSP_SENSOR_GYR_SetOutputDataRate(float Odr)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_SetOutputDataRate(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO, Odr);
  #endif
#endif
}

/**
  * @brief  Set output data rate for magnetometer
  * @param  Odr Output Data Rate value to be set
  * @retval None
  */
void BSP_SENSOR_MAG_SetOutputDataRate(float Odr)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_SetOutputDataRate(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO, Odr);
  #endif
#endif
}

/**
  * @brief  Set output data rate for pressure sensor
  * @param  Odr Output Data Rate value to be set
  * @retval None
  */
void BSP_SENSOR_PRESS_SetOutputDataRate(float Odr)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_SetOutputDataRate(CUSTOM_PRESS_INSTANCE_0, ENV_PRESSURE, Odr);
  #endif
#endif
}

/**
  * @brief  Set output data rate for temperature sensors
  * @param  Odr Output Data Rate value to be set
  * @retval None
  */
void BSP_SENSOR_TEMP_SetOutputDataRate(float Odr)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_ENV_SENSOR_SetOutputDataRate(CUSTOM_TEMP_INSTANCE_0, HYBRID_TEMPERATURE, Odr);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_SetOutputDataRate(CUSTOM_TEMP_INSTANCE_0, ENV_TEMPERATURE, Odr);
  #endif
#endif
#endif
}

/**
  * @brief  Set output data rate for humidity sensor
  * @param  Odr Output Data Rate value to be set
  * @retval None
  */
void BSP_SENSOR_HUM_SetOutputDataRate(float Odr)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_SetOutputDataRate(CUSTOM_HUM_INSTANCE_0, ENV_HUMIDITY, Odr);
  #endif
#endif
}

/**
  * @brief  Get output data rate for accelerometer
  * @param  Odr Output Data Rate value
  * @retval None
  */
void BSP_SENSOR_ACC_GetOutputDataRate(float *Odr)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_GetOutputDataRate(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO, Odr);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetOutputDataRate(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO, Odr);
  #endif
#endif
#endif
}

/**
  * @brief  Get output data rate for gyroscope
  * @param  Odr Output Data Rate value
  * @retval None
  */
void BSP_SENSOR_GYR_GetOutputDataRate(float *Odr)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetOutputDataRate(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO, Odr);
  #endif
#endif
}

/**
  * @brief  Get output data rate for magnetometer
  * @param  Odr Output Data Rate value
  * @retval None
  */
void BSP_SENSOR_MAG_GetOutputDataRate(float *Odr)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetOutputDataRate(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO, Odr);
  #endif
#endif
}

/**
  * @brief  Get output data rate for pressure sensor
  * @param  Odr Output Data Rate value
  * @retval None
  */
void BSP_SENSOR_PRESS_GetOutputDataRate(float *Odr)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_GetOutputDataRate(CUSTOM_PRESS_INSTANCE_0, ENV_PRESSURE, Odr);
  #endif
#endif
}

/**
  * @brief  Get output data rate for temperature sensors
  * @param  Odr Output Data Rate value
  * @retval None
  */
void BSP_SENSOR_TEMP_GetOutputDataRate(float *Odr)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_ENV_SENSOR_GetOutputDataRate(CUSTOM_TEMP_INSTANCE_0, HYBRID_TEMPERATURE, Odr);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_GetOutputDataRate(CUSTOM_TEMP_INSTANCE_0, ENV_TEMPERATURE, Odr);
  #endif
#endif
#endif
}

/**
  * @brief  Get output data rate for humidity sensor
  * @param  Odr Output Data Rate value
  * @retval None
  */
void BSP_SENSOR_HUM_GetOutputDataRate(float *Odr)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_GetOutputDataRate(CUSTOM_HUM_INSTANCE_0, ENV_HUMIDITY, Odr);
  #endif
#endif
}

/**
  * @brief  Set full scale for acclerometer
  * @param  Fullscale Fullscale value to be set
  * @retval None
  */
void BSP_SENSOR_ACC_SetFullScale(int32_t Fullscale)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_SetFullScale(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO, Fullscale);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_SetFullScale(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO, Fullscale);
  #endif
#endif
#endif
}

/**
  * @brief  Set full scale for gyroscope
  * @param  Fullscale Fullscale value to be set
  * @retval None
  */
void BSP_SENSOR_GYR_SetFullScale(int32_t Fullscale)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_SetFullScale(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO, Fullscale);
  #endif
#endif
}

/**
  * @brief  Set full scale for magnetometer
  * @param  Fullscale Fullscale value to be set
  * @retval None
  */
void BSP_SENSOR_MAG_SetFullScale(int32_t Fullscale)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_SetFullScale(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO, Fullscale);
  #endif
#endif
}

/**
  * @brief  Get full scale for acclerometer
  * @param  Fullscale Fullscale value
  * @retval None
  */
void BSP_SENSOR_ACC_GetFullScale(int32_t *Fullscale)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_MOTION_SENSOR_GetFullScale(CUSTOM_ACC_INSTANCE_0, HYBRID_ACCELERO, Fullscale);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetFullScale(CUSTOM_ACC_INSTANCE_0, MOTION_ACCELERO, Fullscale);
  #endif
#endif
#endif
}

/**
  * @brief  Get full scale for gyroscope
  * @param  Fullscale Fullscale value
  * @retval None
  */
void BSP_SENSOR_GYR_GetFullScale(int32_t *Fullscale)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetFullScale(CUSTOM_GYR_INSTANCE_0, MOTION_GYRO, Fullscale);
  #endif
#endif
}

/**
  * @brief  Get full scale for magnetometer
  * @param  Fullscale Fullscale value
  * @retval None
  */
void BSP_SENSOR_MAG_GetFullScale(int32_t *Fullscale)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_GetFullScale(CUSTOM_MAG_INSTANCE_0, MOTION_MAGNETO, Fullscale);
  #endif
#endif
}
