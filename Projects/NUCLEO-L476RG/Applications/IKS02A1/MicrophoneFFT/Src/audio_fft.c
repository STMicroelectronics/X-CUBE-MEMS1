/**
  ******************************************************************************
  * @file    audio_fft.c
  * @author  SRA - Central Labs
  * @version v1.0.0
  * @date    4-July-19
  * @brief   FFT helper functions - based on ARM library.
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
#include "audio_fft.h"

static void TukeyWin(uint16_t len, float ratio, float * dest);
static void BlackmanHarrisWin(uint16_t len, float * dest);
static void HammingWin(uint16_t len, float * dest);

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
* @brief  Initialize the FFT processing depending on the desired configuration
* @param  AUDIO_FFT_instance_t* AUDIO_FFT_instance
* @retval None
*/
int32_t AUDIO_FFT_Init(AUDIO_FFT_instance_t* AUDIO_FFT_instance)
{
  /* Initialize FFT ARM structure */
  arm_rfft_fast_init_f32(&(AUDIO_FFT_instance->context.S), AUDIO_FFT_instance->FFT_len);

  /*Compute indexes to be used for overlap */
  AUDIO_FFT_instance->context.new_data_len = (uint32_t)((float)(AUDIO_FFT_instance->FFT_len) *  (1.0f - (float)(AUDIO_FFT_instance->overlap)));
  AUDIO_FFT_instance->context.old_data_len = (uint32_t)((float)(AUDIO_FFT_instance->FFT_len) -  AUDIO_FFT_instance->context.new_data_len);

  /* Initialize scratch buffer index */
  AUDIO_FFT_instance->context.scratch_idx = 0;

  /* Memory allocation */
  if(AUDIO_FFT_instance->win_type != AUDIO_FTT_RECT_WIN)
  {
    AUDIO_FFT_instance->context.win = (float * )calloc(AUDIO_FFT_instance->FFT_len, sizeof(float));
  }
  AUDIO_FFT_instance->context.scratch = (float * )calloc(AUDIO_FFT_instance->context.new_data_len, sizeof(float));
  AUDIO_FFT_instance->context.dataIn = (float * )calloc(AUDIO_FFT_instance->FFT_len, sizeof(float));
  AUDIO_FFT_instance->context.fftIn = (float * )calloc(AUDIO_FFT_instance->FFT_len, sizeof(float));
  if(AUDIO_FFT_instance->output == MAGNITUDE || AUDIO_FFT_instance->output == PHASE)
  {
  AUDIO_FFT_instance->context.fftOut = (float * )calloc(AUDIO_FFT_instance->FFT_len, sizeof(float));
  }

  /* Create window depending on the user choice */
  switch(AUDIO_FFT_instance->win_type)
  {
  case AUDIO_FTT_RECT_WIN:
    break;
  case AUDIO_FTT_HAMMING_WIN:
    HammingWin(AUDIO_FFT_instance->FFT_len, AUDIO_FFT_instance->context.win);
    break;
  case AUDIO_FTT_HANNING_WIN:
    TukeyWin(AUDIO_FFT_instance->FFT_len, 1.0f, AUDIO_FFT_instance->context.win);
    break;
  case AUDIO_FTT_BLACKMAN_HARRIS_WIN:
    BlackmanHarrisWin(AUDIO_FFT_instance->FFT_len, AUDIO_FFT_instance->context.win);
    break;
  case AUDIO_FTT_TUKEY_0_25_WIN:
    TukeyWin(AUDIO_FFT_instance->FFT_len, 0.25f, AUDIO_FFT_instance->context.win);
    break;
  case AUDIO_FTT_TUKEY_0_75_WIN:
    TukeyWin(AUDIO_FFT_instance->FFT_len, 0.75f, AUDIO_FFT_instance->context.win);
    break;
  }

  return 0;
}
/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/
int32_t AUDIO_FFT_Data_Input(int16_t * data, uint32_t len, AUDIO_FFT_instance_t* AUDIO_FFT_instance)
{
  int32_t ret = 0;
  int32_t index = 0;
  uint32_t s_idx = AUDIO_FFT_instance->context.scratch_idx;
  uint32_t new_data_len = AUDIO_FFT_instance->context.new_data_len;

  float * scratch_ptr = &AUDIO_FFT_instance->context.scratch[s_idx];

  for (index = 0; index < len; index ++)
  {
    *scratch_ptr++ = (float)data[index]/32768.0f;
    s_idx++;
    if (s_idx == new_data_len)
    {
      float * data_in = AUDIO_FFT_instance->context.dataIn;
      uint32_t old_data_len = AUDIO_FFT_instance->context.old_data_len;

      memcpy(data_in, &data_in[AUDIO_FFT_instance->FFT_len - old_data_len], old_data_len * sizeof(float));
      memcpy(&data_in[old_data_len], AUDIO_FFT_instance->context.scratch, new_data_len * sizeof(float));

      ret = 1;
      s_idx = 0;
      scratch_ptr = &AUDIO_FFT_instance->context.scratch[0];

    }
  }


  AUDIO_FFT_instance->context.scratch_idx = s_idx;

  return ret;

}


/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/
int32_t AUDIO_FFT_Process(AUDIO_FFT_instance_t* AUDIO_FFT_instance, float * output)
{

  float * win = AUDIO_FFT_instance->context.win;
  float * dataIn = AUDIO_FFT_instance->context.dataIn;
  float * fftIn = AUDIO_FFT_instance->context.fftIn;

  if(AUDIO_FFT_instance->win_type != AUDIO_FTT_RECT_WIN)
  {
  arm_mult_f32(dataIn, win, fftIn, AUDIO_FFT_instance->FFT_len);
  }
  else
  {
    memcpy(fftIn, dataIn, sizeof(float) * AUDIO_FFT_instance->FFT_len);
  }

  if(AUDIO_FFT_instance->output == COMPLEX)
  {
  arm_rfft_fast_f32(&AUDIO_FFT_instance->context.S, fftIn, output, 0);
  }

    if(AUDIO_FFT_instance->output == MAGNITUDE)
  {
    float * fftOut = AUDIO_FFT_instance->context.fftOut;
    arm_rfft_fast_f32(&AUDIO_FFT_instance->context.S, fftIn, fftOut, 0);
    arm_cmplx_mag_f32(fftOut, output, AUDIO_FFT_instance->FFT_len / 2);
  }

    if(AUDIO_FFT_instance->output == PHASE)
  {
    // NOTE: To be implemented by user if needed
  }

  return 0;
}





static void TukeyWin(uint16_t len, float ratio, float * dest)
{
  uint16_t x = 0;
  for ( x = 0; x < len * (ratio / 2) ; x++)
  {
    dest[x] = 0.5f * (1.0f + cosf(2.0f * M_PI / ratio * ( (float)x/(len - 1.0f) - (ratio / 2))));
  }
  for( ; x <  len - len * (ratio / 2); x++)
  {
    dest[x] = 1.0f;
  }
  for( ; x <  len; x++)
  {
    dest[x] = 0.5f * (1.0f + cosf(2.0f * M_PI / ratio * ( (float)x/(len - 1.0f) - 1 + (ratio / 2))));
  }
}

static void HammingWin(uint16_t len, float * dest)
{
  uint16_t x = 0;
  float alpha0 = 0.53836f;
  float alpha1 = 1.0f - alpha0;

  for ( x = 0; x < len ; x++)
  {
    dest[x] = alpha0 - alpha1 * cosf(2.0f * M_PI * (float)x / (float)len);
  }
}

static void BlackmanHarrisWin(uint16_t len, float * dest)
{
  uint16_t x = 0;
  float alpha0 = 0.42f;
  float alpha1 = 0.5;
  float alpha2 = 0.08f;

  for ( x = 0; x < len ; x++)
  {
    dest[x] = alpha0 - alpha1 * cosf(2.0f * M_PI * (float)x / (float)len) + - alpha2 * cosf(4.0f * M_PI * (float)x / (float)len);
  }
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
