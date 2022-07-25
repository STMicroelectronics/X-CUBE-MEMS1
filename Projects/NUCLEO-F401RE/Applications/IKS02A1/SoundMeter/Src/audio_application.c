/**
  ******************************************************************************
  * @file    audio_application.c
  * @author  SRA - Central Labs
  * @brief   Audio  application.
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

/* Includes ------------------------------------------------------------------*/
#include "audio_application.h"
#include "audio_fw_glo.h"
#include "smr_glo.h"

/** @addtogroup X_CUBE_MEMSMIC1_Applications
* @{
*/

/** @addtogroup Microphones_Acquisition
* @{
*/

/** @defgroup AUDIO_APPLICATION
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** @defgroup AUDIO_APPLICATION_Exported_Variables
* @{
*/
uint16_t PDM_Buffer[((((AUDIO_IN_CHANNELS * AUDIO_IN_SAMPLING_FREQUENCY) / 1000) * MAX_DECIMATION_FACTOR) / 16)* N_MS ];
uint16_t PCM_Buffer[((AUDIO_IN_CHANNELS*AUDIO_IN_SAMPLING_FREQUENCY)/1000)  * N_MS ];

IKS02A1_AUDIO_Init_t MicParams;

/* SMR Manager */
uint8_t pSmrPersistentMem[0x188];
uint8_t pSmrScratchMem[0xF04];

static  smr_static_param_t smr_static_param;
static  smr_dynamic_param_t smr_dynamic_param;

static  buffer_t BufferHandler;
static  buffer_t *pBufferHandler = &BufferHandler;



/**
* @}
*/

/** @defgroup AUDIO_APPLICATION_Private_Variables
* @{
*/
/* Private variables ---------------------------------------------------------*/
/**
* @}
*/

/** @defgroup AUDIO_APPLICATION_Exported_Function
* @{
*/

/**
* @brief  Half Transfer user callback, called by BSP functions.
* @param  None
* @retval None
*/
void IKS02A1_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  AudioProcess();
}

/**
* @brief  Transfer Complete user callback, called by BSP functions.
* @param  None
* @retval None
*/
void IKS02A1_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  AudioProcess();
}

/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/
void AudioProcess(void)
{
  float mean_level = 0.0f;

  /*Perform PDM to PCM conversion*/
  IKS02A1_AUDIO_IN_PDMToPCM(IKS02A1_AUDIO_INSTANCE, (uint16_t * )PDM_Buffer, PCM_Buffer);

  /*Run sound meter library*/
  int32_t error = SMR_ERROR_NONE;

  BufferHandler.data_ptr = (int16_t * )PCM_Buffer;

  error = smr_process(pBufferHandler, pBufferHandler, pSmrPersistentMem);

  if (error != SMR_ERROR_NONE)
  {
    while(1);
  }

  smr_getConfig(&smr_dynamic_param, pSmrPersistentMem);
  mean_level  = IMP34DT05_AOP + (float)smr_dynamic_param.mean_level_left/ 4.0f;

  /* Store data to be sent */
  MeanLevel = mean_level;
  NewAcquisition = 1U;
}

/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/
void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{
  MicParams.BitsPerSample = 16;
  MicParams.ChannelsNbr = ChnlNbrIn;
  MicParams.Device = AUDIO_IN_DIGITAL_MIC;
  MicParams.SampleRate = AudioFreq;
  MicParams.Volume = AUDIO_VOLUME_INPUT;

  IKS02A1_AUDIO_IN_Init(IKS02A1_AUDIO_INSTANCE, &MicParams);
}

void Init_Sound_Meter_Module(uint32_t AudioFreq, uint32_t ChnlNbrIn)
{
  int32_t error = SMR_ERROR_NONE;

  /* Enables and resets CRC-32 from STM32 HW */
  __HAL_RCC_CRC_CLK_ENABLE();
  CRC->CR = CRC_CR_RESET;

  /* SMR effect reset */
  error = smr_reset(pSmrPersistentMem, pSmrScratchMem);
  if (error != SMR_ERROR_NONE)
  {
    while(1);
  }

  /* SMR effect static parameters setting */
  smr_static_param.sampling_rate = AudioFreq;
  error = smr_setParam(&smr_static_param, pSmrPersistentMem);
  if (error != SMR_ERROR_NONE)
  {
    while(1);
  }
  /* SMR dynamic parameters that can be updated here every frame if required */
  smr_dynamic_param.enable = 0;                        /* SMR module enabler */
  smr_dynamic_param.averaging_time = AUDIO_CFG_SMR_AVGTIME_DEFAULT;
  smr_dynamic_param.filter_type = SMR_PREFILTER_NONE;
  smr_dynamic_param.mean_level_left = -45;
  smr_dynamic_param.mean_level_right = -45;


  error = smr_setConfig(&smr_dynamic_param, pSmrPersistentMem);
  if (error != SMR_ERROR_NONE)
  {
    while(1);
  }

  /* I/O buffers settings */
  BufferHandler.nb_bytes_per_Sample = 2; /* 16 bits per sample */
  BufferHandler.nb_channels = 1; /* stereo */
  BufferHandler.buffer_size = AudioFreq/1000; /* 1 ms */
  BufferHandler.mode = INTERLEAVED;

  /* SMR module enabler */
  smr_dynamic_param.enable = 1;
  error = smr_setConfig(&smr_dynamic_param, pSmrPersistentMem);
  if (error != SMR_ERROR_NONE)
  {
    while(1);
  }
}

/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/
void Start_Acquisition(void)
{
  IKS02A1_AUDIO_IN_Record(IKS02A1_AUDIO_INSTANCE, (uint8_t *) PDM_Buffer, AUDIO_IN_BUFFER_SIZE);
}
/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/
