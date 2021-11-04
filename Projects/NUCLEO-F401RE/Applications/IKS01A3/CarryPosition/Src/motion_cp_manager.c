/**
 ******************************************************************************
 * @file    motion_cp_manager.c
 * @author  MEMS Software Solutions Team
 * @brief   This file contains Carry Position interface functions
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the â€œLicenseâ€?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "motion_cp_manager.h"

#include "iks01a3_mems_control_ex.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup CARRY_POSITION CARRY POSITION
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the MotionCP engine
 * @param  None
 * @retval None
 */
void MotionCP_manager_init(void)
{
  char acc_orientation[4];

  MotionCP_Initialize();

  BSP_SENSOR_ACC_GetOrientation(acc_orientation);
  MotionCP_SetOrientation_Acc(acc_orientation);
}

/**
 * @brief  Run Carry Position algorithm
 * @param  data_in  Structure containing input data
 * @param  data_out Structure containing output data
 * @retval None
 */
void MotionCP_manager_run(MCP_input_t *data_in, MCP_output_t *data_out)
{
  MotionCP_Update(data_in, data_out);
}

/**
 * @brief  Get the library version
 * @param  version Library version string (must be array of 35 char)
 * @param  length  Library version string length
 * @retval None
 */
void MotionCP_manager_get_version(char *version, int *length)
{
  *length = (int)MotionCP_GetLibVersion(version);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
