/**
  ******************************************************************************
  * @file    iks02a1_conf.h
  * @author  SRA - Central Labs
  * @brief   This file contains definitions for the MEMSMIC1 applications
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
#ifndef IKS02A1_CONF_H__
#define IKS02A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "nucleo_f401re_bus.h"
#include "nucleo_f401re_errno.h"


/* The N_MS value defines the number of millisecond to be processed at each AudioProcess call,
that must be consistent with the N_MS_PER_INTERRUPT defined in the audio driver
(iks02a1_audio.h).
The default value of the N_MS_PER_INTERRUPT directive in the driver is set to 1, 
for backward compatibility: leaving this values as it is allows to avoid any 
modification in the application layer developed with the older versions of the driver */

#define N_MS (N_MS_PER_INTERRUPT)
  
#define AUDIO_IN_CHANNELS 1
#define AUDIO_IN_SAMPLING_FREQUENCY     16000

#define AUDIO_IN_BUFFER_SIZE            DEFAULT_AUDIO_IN_BUFFER_SIZE  
#define AUDIO_VOLUME_INPUT              4U
#define IKS02A1_AUDIO_IN_IT_PRIORITY    6U
#define IKS02A1_AUDIO_INSTANCE          0U

#if (AUDIO_IN_SAMPLING_FREQUENCY == 8000)
#define MAX_DECIMATION_FACTOR 160
#else
#define MAX_DECIMATION_FACTOR 128
#endif
  
  /*#define USE_SPI3*/
  /*If you want to use SPI3 instead of SPI2 for M3 and M4, uncomment this define and 
  close SB20 and SB21*/
  
  /*#define PDM_FREQ_16K 2048*/
  /*Uncomment if you need to change PDM clock frequency when AUDIO_IN_SAMPLING_FREQUENCY = 16000*/

#ifdef __cplusplus
}
#endif

#endif /* IKS02A1_CONF_H__*/
