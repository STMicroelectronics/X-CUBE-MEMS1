/**
  ******************************************************************************
  * @file    iks02a1_conf_template.h
  * @author  MEMS Application Team
  * @brief   IKS02A1 configuration template file.
  *          This file should be copied to the application folder and renamed
  *          to iks02a1_conf.h.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Replace the header file names with the ones of the target platform */
#include "stm32yyxx_hal.h"
#include "nucleo_xyyyzz_bus.h"
#include "nucleo_xyyyzz_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IKS02A1_CONF_H__
#define __IKS02A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */
#define USE_IKS02A1_MOTION_SENSOR_ISM330DHCX_0         1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2DLPC_0           1U
#define USE_IKS02A1_MOTION_SENSOR_IIS2MDC_0            1U
/* USER CODE END 1 */

#define IKS02A1_I2C_Init BSP_I2C1_Init
#define IKS02A1_I2C_DeInit BSP_I2C1_DeInit
#define IKS02A1_I2C_ReadReg BSP_I2C1_ReadReg
#define IKS02A1_I2C_WriteReg BSP_I2C1_WriteReg
#define IKS02A1_GetTick BSP_GetTick

/* The N_MS value defines the number of millisecond to be processed at each AudioProcess call,
that must be consistent with the N_MS_PER_INTERRUPT defined in the audio driver
(cca02m1_audio.h).
The default value of the N_MS_PER_INTERRUPT directive in the driver is set to 1,
for backward compatibility: leaving this values as it is allows to avoid any
modification in the application layer developed with the older versions of the driver */


#define N_MS (N_MS_PER_INTERRUPT)

#define AUDIO_IN_CHANNELS 4
#define AUDIO_IN_SAMPLING_FREQUENCY 16000

#define AUDIO_IN_BUFFER_SIZE            DEFAULT_AUDIO_IN_BUFFER_SIZE
#define AUDIO_VOLUME_INPUT              64U
#define CCA02M1_AUDIO_INSTANCE      0U /*Select 0U or 1U in base of the peripheral used (0U->I2S/SAI 1U->DFSDM)*/
#define CCA02M1_AUDIO_IN_IT_PRIORITY    6U

#if (AUDIO_IN_SAMPLING_FREQUENCY == 8000)
#define MAX_DECIMATION_FACTOR 160
#else
#define MAX_DECIMATION_FACTOR 128
#endif

//  #define USE_SPI3
/*If you want to use SPI3 instead of SPI2 for M3 and M4, uncomment this define and
close SB20 and SB21*/

//  #define PDM_FREQ_16K 2048
/*Uncomment if you need to change PDM clock frequency when AUDIO_IN_SAMPLING_FREQUENCY = 16000*/

#ifdef __cplusplus
}
#endif

#endif /* __IKS02A1_CONF_H__*/
