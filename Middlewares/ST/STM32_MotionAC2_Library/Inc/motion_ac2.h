/**
  ******************************************************************************
  * @file    motion_ac2.h
  * @author  MEMS Application Team
  * @version V1.3.1
  * @date    24-August-2021
  * @brief   Header for motion_ac2 module
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
#ifndef MOTIONAC2_H_
#define MOTIONAC2_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_AC2 MOTION_AC2
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_AC2_Exported_Types MOTION_AC2_Exported_Types
  * @{
  */

typedef enum
{
  MAC2_CAL_UNKNOWN = 0,
  MAC2_CAL_POOR,
  MAC2_CAL_OK,
  MAC2_CAL_GOOD,
} MAC2_cal_status_t;

typedef struct
{
  float Acc_X;  /* Acceleration in X axis in [g] */
  float Acc_Y;  /* Acceleration in Y axis in [g] */
} MAC2_input_t;

typedef struct
{
  float FullScale;     /* FS of XL in [g] */
  int CalDuration_s;   /* Total duration of calibration [ex : 120s] */
  float XlNoiseScale;  /* Noise sensitivity [default 1, higher for noisy accelerometer] */
} MAC2_knobs_t;

typedef struct
{
  float Bias[2];                /* Offset of X/Y axis in [g] */
  float SF[2];                  /* Scale for X/Y axis */
  MAC2_cal_status_t CalStatus;  /* Calibration status */
} MAC2_cal_params_t;

/* NOTE: Calibrated acceleration can be calculated as follows:
 *       xCal = (xRaw - Bias[0]) * SF[0]
 *       yCal = (yRaw - Bias[1]) * SF[1]
 */

typedef enum
{
  MAC2_MCU_STM32 = 0,
  MAC2_MCU_BLUE_NRG1,
  MAC2_MCU_BLUE_NRG2,
  MAC2_MCU_BLUE_NRG_LP,
} MAC2_mcu_type_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AC2_Exported_Functions MOTION_AC2_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionAC2 engine
  * @param  mcu_type MCU type
  * @param  freq  pointer to accelerometer sample frequency
  * @retval None
  */
void MotionAC2_Init(MAC2_mcu_type_t mcu_type, float *freq);

/**
  * @brief  Run accelerometer calibration algorithm
  * @param  data_in  pointer to acceleration in [g]
  * @param  timestamp_ms  timestamp in ms
  * @retval 1 if calibration is done with current sample, 0 otherwise
  */
uint8_t MotionAC2_Update(MAC2_input_t *data_in, uint64_t timestamp_ms);

/**
  * @brief  Setting the knobs setting. The API can be called after motionAC2_Init but before motionAC2_Update.
  * @param  knobs  pointer to knobs structure
  * @retval None
  */
void MotionAC2_SetKnobs(MAC2_knobs_t *knobs);

/**
  * @brief  Getting the current knobs setting
  * @param  knobs  pointer to knobs structure
  * @retval None
  */
void MotionAC2_GetKnobs(MAC2_knobs_t *knobs);

/**
  * @brief  Setting the initial calibration parameters. The API can be called after motionAC2_Init but before motionAC2_Update.
  * @param  cal_params  pointer to calibration parameters structure
  * @retval None
  */
void MotionAC2_SetCalParams(MAC2_cal_params_t *cal_params);

/**
  * @brief  Getting the current cailbration parameter
  * @param  cal_params  pointer to calibration parameters structure
  * @retval None
  */
void MotionAC2_GetCalParams(MAC2_cal_params_t *cal_params);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionAC2_GetLibVersion(char *version);

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
#endif /*__cplusplus*/

#endif /* MOTIONAC2_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
