/**
  ******************************************************************************
  * @file    audio_application.c
  * @author  SRA - Central Labs
  * @version v5.0.0
  * @date    6-May-19
  * @brief   Audio  application.
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

/* Includes ------------------------------------------------------------------*/
#include "audio_application.h"


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

/* FFT Manager */

AUDIO_FFT_instance_t audio_fft_M1;
float * FFT_Out;
float * FFT_Average;


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
  /*for L4 PDM to PCM conversion is performed in hardware by DFSDM peripheral*/
  IKS02A1_AUDIO_IN_PDMToPCM(IKS02A1_AUDIO_INSTANCE, (uint16_t * )PDM_Buffer, PCM_Buffer);

  if(AUDIO_FFT_Data_Input((int16_t *)PCM_Buffer, (audio_fft_M1.sampling_frequency/1000) , &audio_fft_M1))
  {
    SW_Task1_Start();
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
void Init_Acquisition_Peripherals(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{
  MicParams.BitsPerSample = 16;
  MicParams.ChannelsNbr = ChnlNbrIn;
  MicParams.Device = AUDIO_IN_DIGITAL_MIC;
  MicParams.SampleRate = AudioFreq;
  MicParams.Volume = AUDIO_VOLUME_INPUT;

  IKS02A1_AUDIO_IN_Init(IKS02A1_AUDIO_INSTANCE, &MicParams);
}

void Init_FFT_Module(uint32_t AudioFreq, uint32_t FFTLen, float Overlap)
{
  audio_fft_M1.sampling_frequency = AudioFreq;
  audio_fft_M1.FFT_len = FFTLen;
  audio_fft_M1.overlap = Overlap;
  audio_fft_M1.win_type = AUDIO_FTT_HAMMING_WIN;
  audio_fft_M1.output = MAGNITUDE;
  AUDIO_FFT_Init(&audio_fft_M1);

  /* Allocate output buffer */
  FFT_Out = calloc(audio_fft_M1.FFT_len / 2, sizeof(float));
  FFT_Average = calloc(audio_fft_M1.FFT_len / 2, sizeof(float));
  FFTAverage = calloc(audio_fft_M1.FFT_len / 2, sizeof(float));

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
* @brief  Initializes two SW interrupt with different priorities
* @param  None
* @retval None
*/
void SW_IRQ_Tasks_Init(void)
{
  HAL_NVIC_SetPriority((IRQn_Type)EXTI1_IRQn, 0x0D, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)EXTI1_IRQn);
}

/**
* @brief  Highest priority interrupt handler routine
* @param  None
* @retval None
*/
void SW_Task1_Callback(void)
{

  uint16_t nAccTotal = (uint16_t)(((float)(4 * audio_fft_M1.sampling_frequency)/((float)(audio_fft_M1.FFT_len) *  (1.0f - (float)(audio_fft_M1.overlap)))));
  static uint16_t nAcc = 0;
  AUDIO_FFT_Process(&audio_fft_M1, FFT_Out);
  arm_add_f32(FFT_Out, FFT_Average, FFT_Average, audio_fft_M1.FFT_len/2);
  nAcc++;

  if (nAcc == nAccTotal)
  {
    arm_scale_f32(FFT_Average, 1.0f/(float)nAcc, FFT_Average, audio_fft_M1.FFT_len/2);

    /* Store data to be sent */
    FFTLen = audio_fft_M1.FFT_len;
    memcpy((void *)FFTAverage, (void *)FFT_Average, sizeof(float) * FFTLen / 2);
    SamplingFreq = (float)audio_fft_M1.sampling_frequency;
    NewData = 1U;

    memset((uint8_t *)FFT_Average, 0, audio_fft_M1.FFT_len/2);
    nAcc = 0;
  }

}

/**
* @brief Throws Highest priority interrupt
* @param  None
* @retval None
*/
void SW_Task1_Start(void)
{
  HAL_NVIC_SetPendingIRQ(EXTI1_IRQn);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
