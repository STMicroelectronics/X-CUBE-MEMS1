/**
  ******************************************************************************
  * @file    audio_application.h
  * @author  SRA - Central Labs
  * @version v5.0.0
  * @date    6-May-19
  * @brief   Header for audio_application.c module.
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
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_APPLICATION_H
#define __AUDIO_APPLICATION_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "cube_hal.h"

/** @addtogroup X_CUBE_MEMSMIC1_Applications
  * @{
  */

/** @addtogroup Microphones_Acquisition
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t NewAcquisition;
extern volatile float MeanLevel;

/** @defgroup AUDIO_APPLICATION_Exported_Defines
 * @{
 */


/*Comment this define if you want to configure and start acquisition
depending on USB functionalities implemented by user*/
#define AUDIO_CFG_SMR_AVGTIME_MAX                  10000
#define AUDIO_CFG_SMR_AVGTIME_MIN                  0
#define AUDIO_CFG_SMR_AVGTIME_DEFAULT              100

#define IMP34DT05_AOP                             (float)(122.5)


/**
 * @}
 */
/* Exported functions ------------------------------------------------------- */
void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
void Init_Sound_Meter_Module(uint32_t AudioFreq, uint32_t ChnlNbrIn);

void Start_Acquisition(void);
void Error_Handler(void);
void AudioProcess(void);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



#endif /* __AUDIO_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
