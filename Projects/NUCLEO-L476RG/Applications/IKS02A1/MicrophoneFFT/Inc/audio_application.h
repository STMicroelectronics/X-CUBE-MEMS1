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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
