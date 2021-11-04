/**
  ******************************************************************************
  * @file    motion_cp.h
  * @author  MEMS Application Team
  * @version V2.2.1
  * @date    24-August-2021
  * @brief   Header for motion_cp module
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
#ifndef _MOTION_CP_H_
#define _MOTION_CP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_CP MOTION_CP
  * @{
  */


/** @defgroup MOTION_CP_Exported_Types MOTION_CP_Exported_Types
 * @{
 */
/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
 typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
} MCP_input_t;

typedef enum
{
  MCP_UNKNOWN             = 0x00,
  MCP_ONDESK              = 0x01,
  MCP_INHAND              = 0x02,
  MCP_NEARHEAD            = 0x03,
  MCP_SHIRTPOCKET         = 0x04,
  MCP_TROUSERPOCKET       = 0x05,
  MCP_ARMSWING            = 0x06,
  MCP_JACKETPOCKET        = 0x07
} MCP_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_CP_Exported_Functions MOTION_CP_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionCP engine
 * @param  none
 * @retval none
 */
void MotionCP_Initialize(void);

/**
 * @brief  Set the MotionCP accelerometer data orientation
 * @param  *acc_orientation: reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval  none
 */
void MotionCP_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Run carry position recognition algorithm
 * @param  data_in  pointer to accaleration in [g]
 * @param  data_out  pointer to carry position index
 * @retval none
 */
void MotionCP_Update(MCP_input_t *data_in, MCP_output_t *data_out);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionCP_GetLibVersion(char *version);


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

#endif /* _MOTION_CP_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
