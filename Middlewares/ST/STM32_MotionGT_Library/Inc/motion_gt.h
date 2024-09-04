/**
  ******************************************************************************
  * @file    motion_gt.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @date    08-April-2019
  * @brief   Header for motion_gt module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ********************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_GT_H_
#define _MOTION_GT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_GT MOTION_GT
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_GT_Exported_Types MOTION_GT_Exported_Types
 * @{
 */

typedef struct
{
  uint64_t TimeStamp;   /* Timestamp in [ms] */
  float GyroBiasX;      /* Gyroscope offset value in X axis in [dps] */
  float GyroBiasY;      /* Gyroscope offset value in Y axis in [dps] */
  float GyroBiasZ;      /* Gyroscope offset value in Z axis in [dps] */
  float Temp;           /* Temperature in [deg] */
} MGT_input_t;

typedef struct
{
  float GyroBiasX;     /* Gyroscope offset value in X axis in [dps] */
  float GyroBiasY;     /* Gyroscope offset value in Y axis in [dps] */
  float GyroBiasZ;     /* Gyroscope offset value in Z axis in [dps] */
} MGT_output_t;

typedef struct
{
  uint64_t LastUpdateTime;  /* Time when the internal model was updated last time */
  float GyroBias[3];        /* Gyroscope offset in [dps] */
  float Scale[3];           /* Offeset vs temperature slope */
  float Quality;            /* Approximation quality */
  uint8_t ModelEstimated;   /* 1 ... if a model was estimated, 0 ... otherwise */
} MGT_state_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_GT_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionGT engine
  * @param  none
  * @retval none
  */
void MotionGT_Initialize(void);

/**
  * @brief  Run gyroscope tempeareture bias calibration algorithm
  * @param  data_in  pointer to time stamp [ms], gyroscope offset values [dps] and temperarure [deg]
  * @param  bias_update pointer to an integer that is set to 1 if the gyroscope bias (T) slope is estimated, 0 otherwise
  * @retval none
  */
void MotionGT_Update(MGT_input_t *data_in, int *bias_update);

/**
* @brief  Returns temperature compensated bias for selected temperature
* @param  temp  temperature in [deg]
* @param  data_out  pointer to compenstated gyroscope offset values [dps]
* @retval none
*/
void MotionGT_GetCompensatedBias(float *temp, MGT_output_t *data_out);

/**
  * @brief  Get the gyroscope temperature compensation parameters
  * @param  state  pointer to actual gyroscope tempearature parameters
  * @retval none
  */
void MotionGT_GetCalState(MGT_state_t *state);

/**
* @brief  Set the gyroscope temperature compensation model at the start. should be called after init
* @param  state pointer to prestored gyroscope tempearature model
* @retval char, 1 if model accepted, 0 otherwise
*/
char MotionGT_SetCalState(const MGT_state_t *state);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionGT_GetLibVersion(char *version);

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

#endif /* _MOTION_GC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
