
/**
  ******************************************************************************
  * @file    custom_mems_control_ex.c
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
#include "custom_mems_control_ex.h"

/**
  * @brief  Get the register value from accelerometer
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_ACC_Read_Register(uint8_t Reg, uint8_t *Data)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_SENSOR_Read_Register(CUSTOM_ACC_INSTANCE_0, Reg, Data);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Read_Register(CUSTOM_ACC_INSTANCE_0, Reg, Data);
  #endif
#endif
#endif
}

/**
  * @brief  Get the register value from gyroscope
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_GYR_Read_Register(uint8_t Reg, uint8_t *Data)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Read_Register(CUSTOM_GYR_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Get the register value from magnetometer
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_MAG_Read_Register(uint8_t Reg, uint8_t *Data)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Read_Register(CUSTOM_MAG_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Get the register value from pressure sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_PRESS_Read_Register(uint8_t Reg, uint8_t *Data)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Read_Register(CUSTOM_PRESS_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Get the register value from temperature sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_TEMP_Read_Register(uint8_t Reg, uint8_t *Data)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_SENSOR_Read_Register(CUSTOM_TEMP_INSTANCE_0, Reg, Data);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Read_Register(CUSTOM_TEMP_INSTANCE_0, Reg, Data);
  #endif
#endif
#endif
}

/**
  * @brief  Get the register value from humidity sensor
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_SENSOR_HUM_Read_Register(uint8_t Reg, uint8_t *Data)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Read_Register(CUSTOM_HUM_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Set the register value
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_ACC_Write_Register(uint8_t Reg, uint8_t Data)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_HYBRID_SENSOR_Write_Register(CUSTOM_ACC_INSTANCE_0, Reg, Data);
  #endif
#else
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Write_Register(CUSTOM_ACC_INSTANCE_0, Reg, Data);
  #endif
#endif
#endif
}

/**
  * @brief  Set the register value
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_GYR_Write_Register(uint8_t Reg, uint8_t Data)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Write_Register(CUSTOM_GYR_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Set the register value
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_MAG_Write_Register(uint8_t Reg, uint8_t Data)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
  (void)CUSTOM_MOTION_SENSOR_Write_Register(CUSTOM_MAG_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Set the register value in pressure sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_PRESS_Write_Register(uint8_t Reg, uint8_t Data)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_PRESS_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Write_Register(CUSTOM_PRESS_INSTANCE_0, Reg, Data);
  #endif
#endif
}

/**
  * @brief  Set the register value in temperature sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_TEMP_Write_Register(uint8_t Reg, uint8_t Data)
{
#if (defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_HYBRID_SENSOR_Write_Register(CUSTOM_TEMP_INSTANCE_0, Reg, Data);
  #endif
#else
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_TEMP_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Write_Register(CUSTOM_TEMP_INSTANCE_0, Reg, Data);
  #endif
#endif
#endif
}

/**
  * @brief  Set the register value in humidity sensor
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval None
  */
void BSP_SENSOR_HUM_Write_Register(uint8_t Reg, uint8_t Data)
{
#if (defined BSP_ENV_SENSORS)
  #ifdef CUSTOM_HUM_INSTANCE_0
  (void)CUSTOM_ENV_SENSOR_Write_Register(CUSTOM_HUM_INSTANCE_0, Reg, Data);
  #endif
#endif
}

#if ((defined CUSTOM_ALGOBUILDER_FW_TEMPLATE) || (defined CUSTOM_DATALOGFUSION_DEMO) || (defined CUSTOM_TILTSENSING_DEMO) || (defined CUSTOM_GESTURERECOGNITION_DEMO) || (defined CUSTOM_CARRYPOSITION_DEMO) || (defined CUSTOM_ECOMPASS_DEMO) || (defined CUSTOM_DYNAMICINCLINOMETER_DEMO) || (defined CUSTOM_ACTIVITYRECOGNITION_DEMO) || (defined CUSTOM_ACTIVITYRECOGNITIONWRIST_DEMO) || (defined CUSTOM_FITNESSACTIVITIES_DEMO) || (defined CUSTOM_PEDOMETERWRIST_DEMO) || (defined CUSTOM_POSEESTIMATION_DEMO) || (defined CUSTOM_STANDINGSITTINGDESK_DEMO))
/**
  * @brief  Get accelerometer sensor orientation
  * @param  Orientation Pointer to sensor orientation
  * @retval None
  */
void BSP_SENSOR_ACC_GetOrientation(char *Orientation)
{
#if (defined BSP_MOTION_SENSORS || defined BSP_HYBRID_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
    Orientation[0] = 's';
    Orientation[1] = 'e';
    Orientation[2] = 'u';
  #endif
#endif
}
#endif

#if ((defined CUSTOM_ALGOBUILDER_FW_TEMPLATE) || (defined CUSTOM_DATALOGFUSION_DEMO) || (defined CUSTOM_DYNAMICINCLINOMETER_DEMO) || (defined CUSTOM_FITNESSACTIVITIES_DEMO))
/**
  * @brief  Get gyroscope sensor orientation
  * @param  Orientation Pointer to sensor orientation
  * @retval None
  */
void BSP_SENSOR_GYR_GetOrientation(char *Orientation)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
    Orientation[0] = 's';
    Orientation[1] = 'e';
    Orientation[2] = 'u';
  #endif
#endif
}
#endif

#if ((defined CUSTOM_ALGOBUILDER_FW_TEMPLATE) || (defined CUSTOM_DATALOGFUSION_DEMO) || (defined CUSTOM_ECOMPASS_DEMO))
/**
  * @brief  Get magnetometer sensor orientation
  * @param  Orientation Pointer to sensor orientation
  * @retval None
  */
void BSP_SENSOR_MAG_GetOrientation(char *Orientation)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_MAG_INSTANCE_0
    Orientation[0] = 'n';
    Orientation[1] = 'e';
    Orientation[2] = 'u';
  #endif
#endif
}
#endif

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
/**
  * @brief  Set DRDY interrupt mode for accelerometer
  * @param  Mode Mode to be set (1 means pulsed mode otherwise latched mode)
  * @retval None
  */
void BSP_SENSOR_ACC_SetDRDYMode(uint8_t Mode)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
/*
  Example:
  if (Mode == 1)
  {
    (void)CUSTOM_MOTION_SENSOR_DRDY_Set_Mode(CUSTOM_ACC_INSTANCE_0, DRDY_PULSED);
  }
  else
  {
    (void)CUSTOM_MOTION_SENSOR_DRDY_Set_Mode(CUSTOM_ACC_INSTANCE_0, DRDY_LATCHED);
  }
*/
  #endif
#endif
}
#endif

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
/**
  * @brief  Set DRDY interrupt mode for gyroscope
  * @param  Mode Mode to be set (1 means pulsed mode otherwise latched mode)
  * @retval None
  */
void BSP_SENSOR_GYR_SetDRDYMode(uint8_t Mode)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
/*
  Example:
  if (Mode == 1)
  {
    (void)CUSTOM_MOTION_SENSOR_DRDY_Set_Mode(CUSTOM_GYR_INSTANCE_0, DRDY_PULSED);
  }
  else
  {
    (void)CUSTOM_MOTION_SENSOR_DRDY_Set_Mode(CUSTOM_GYR_INSTANCE_0, DRDY_LATCHED);
  }
*/
  #endif
#endif
}
#endif

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
/**
  * @brief  Enables/Disables DRDY signal from accelerometer
  * @param  Enable Define if DRDY signal is enabled or disabled
  * @retval None
  */
void BSP_SENSOR_ACC_SetDRDYInt(uint8_t Enable)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_ACC_INSTANCE_0
/*
  Example:
  uint8_t reg = 0;

  (void)CUSTOM_MOTION_SENSOR_Read_Register(CUSTOM_ACC_INSTANCE_0, INT1_CTRL, &reg);

  if (Enable == 1)
  {
    reg = reg | 0x01;
  }
  else
  {
    reg = reg & ~0x01;
  }

  (void)CUSTOM_MOTION_SENSOR_Write_Register(CUSTOM_ACC_INSTANCE_0, INT1_CTRL, reg);
*/
  #endif
#endif
}
#endif

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
/**
  * @brief  Enables/Disables DRDY signal from gyroscope
  * @param  Enable Define if DRDY signal is enabled or disabled
  * @retval None
  */
void BSP_SENSOR_GYR_SetDRDYInt(uint8_t Enable)
{
#if (defined BSP_MOTION_SENSORS)
  #ifdef CUSTOM_GYR_INSTANCE_0
/*
  Example:
  uint8_t reg = 0;

  (void)CUSTOM_MOTION_SENSOR_Read_Register(CUSTOM_GYR_INSTANCE_0, INT1_CTRL, &reg);

  if (Enable == 1)
  {
    reg = reg | 0x02;
  }
  else
  {
    reg = reg & ~0x02;
  }

  (void)CUSTOM_MOTION_SENSOR_Write_Register(CUSTOM_GYR_INSTANCE_0, INT1_CTRL, reg);
*/
  #endif
#endif
}
#endif

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
/**
  * @brief  Reads data from FSM
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_ACC_GYR_Read_FSM_Data(uint8_t *Data)
{
#if (defined BSP_MOTION_SENSORS)
  #if (defined(CUSTOM_ACC_INSTANCE_0) | defined (CUSTOM_GYR_INSTANCE_0))
/*
  Example for LSM6DSOX:
  (void)CUSTOM_MOTION_SENSOR_Write_Register(LSM6DSOX_0, LSM6DSOX_FUNC_CFG_ACCESS, 0x80);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS1, &Data[0]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS2, &Data[1]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS3, &Data[2]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS4, &Data[3]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS5, &Data[4]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS6, &Data[5]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS7, &Data[6]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS8, &Data[7]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS9, &Data[8]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS10, &Data[9]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS11, &Data[10]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS12, &Data[11]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS13, &Data[12]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS14, &Data[13]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS15, &Data[14]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_FSM_OUTS16, &Data[15]);
  (void)CUSTOM_MOTION_SENSOR_Write_Register(LSM6DSOX_0, LSM6DSOX_FUNC_CFG_ACCESS, 0x00);
*/
  #endif
#endif
}
#endif

#if (defined CUSTOM_ALGOBUILDER_FW_TEMPLATE)
/**
  * @brief  Reads data from MLC
  * @param  Data pointer where the value is written to
  * @retval None
  */
void BSP_ACC_GYR_Read_MLC_Data(uint8_t *Data)
{
#if (defined BSP_MOTION_SENSORS)
  #if (defined(CUSTOM_ACC_INSTANCE_0) | defined (CUSTOM_GYR_INSTANCE_0))
/*
  Example for LSM6DSOX:
  (void)CUSTOM_MOTION_SENSOR_Write_Register(LSM6DSOX_0, LSM6DSOX_FUNC_CFG_ACCESS, 0x80);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC0_SRC, &Data[0]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC1_SRC, &Data[1]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC2_SRC, &Data[2]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC3_SRC, &Data[3]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC4_SRC, &Data[4]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC5_SRC, &Data[5]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC6_SRC, &Data[6]);
  (void)CUSTOM_MOTION_SENSOR_Read_Register(LSM6DSOX_0, LSM6DSOX_MLC7_SRC, &Data[7]);
  (void)CUSTOM_MOTION_SENSOR_Write_Register(LSM6DSOX_0, LSM6DSOX_FUNC_CFG_ACCESS, 0x00);
*/
  #endif
#endif
}
#endif
