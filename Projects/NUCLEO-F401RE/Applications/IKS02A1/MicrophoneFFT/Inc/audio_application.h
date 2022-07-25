/**
  ******************************************************************************
  * @file    audio_application.h
  * @author  SRA - Central Labs
  * @brief   Header for audio_application.c module.
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
#ifndef __AUDIO_APPLICATION_H
#define __AUDIO_APPLICATION_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "cube_hal.h"
#include "audio_fft.h"

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
extern volatile uint8_t NewData;
extern volatile float *FFTAverage;
extern volatile float SamplingFreq;
extern volatile uint32_t FFTLen;

/** @defgroup AUDIO_APPLICATION_Exported_Defines
 * @{
 */


/*Comment this define if you want to configure and start acquisition
depending on USB functionalities implemented by user*/
#define DISABLE_USB_DRIVEN_ACQUISITION
#define AUDIO_CFG_SMR_AVGTIME_MAX                  10000
#define AUDIO_CFG_SMR_AVGTIME_MIN                  0
#define AUDIO_CFG_SMR_AVGTIME_DEFAULT              100


/**
 * @}
 */
/* Exported functions ------------------------------------------------------- */
void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
void Init_FFT_Module(uint32_t AudioFreq, uint32_t FFTLen, float Overlap);

void Start_Acquisition(void);
void Error_Handler(void);
void AudioProcess(void);

void SW_IRQ_Tasks_Init(void);
void SW_Task1_Callback(void);
void SW_Task1_Start(void);



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
