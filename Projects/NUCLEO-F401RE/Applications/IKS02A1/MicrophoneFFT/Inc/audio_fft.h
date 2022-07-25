/**
  ******************************************************************************
  * @file    audio_fft.h
  * @author  SRA - Central Labs
  * @brief   header for audio_fft.h file . 
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
#ifndef __AUDIO_FFT_H
#define __AUDIO_FFT_H

#ifndef __FPU_PRESENT
#define __FPU_PRESENT (1)
#endif

/* Includes ------------------------------------------------------------------*/
#include "arm_math.h"
#include "math.h"
#include "stdlib.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

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
typedef struct
{                                   
  arm_rfft_fast_instance_f32 S;
  uint32_t new_data_len;
  uint32_t old_data_len; /* New data Idx */
  uint32_t scratch_idx; 
  
  float * win;       
  float * scratch;
  float * dataIn;
  float * fftIn;
  float * fftOut;  
}
AUDIO_FFT_context_t;

typedef struct
{                                   
  uint32_t sampling_frequency;
  uint32_t FFT_len;
  float overlap;
  uint32_t win_type;  
  uint32_t output;
  AUDIO_FFT_context_t context;  
}
AUDIO_FFT_instance_t;

typedef enum
{
  AUDIO_FTT_RECT_WIN = 0,
  AUDIO_FTT_HAMMING_WIN,
  AUDIO_FTT_HANNING_WIN,  
  AUDIO_FTT_BLACKMAN_HARRIS_WIN,
  AUDIO_FTT_TUKEY_0_25_WIN,    
  AUDIO_FTT_TUKEY_0_75_WIN,    
}
AUDIO_FFT_windows_t;

typedef enum
{
  COMPLEX = 0,
  MAGNITUDE,
  PHASE  
}
AUDIO_FFT_output_t;

typedef enum
{
  FFT_ERROR_OK = 0,
  FFT_ERROR_INVALID_PARAMETER,
  FFT_ERROR_MEMORY  
}
AUDIO_FFT_error_t;



/* Exported macro ------------------------------------------------------------*/
/** @defgroup AUDIO_APPLICATION_Exported_Defines 
 * @{
 */



/**
 * @}
 */
/* Exported functions ------------------------------------------------------- */
int32_t AUDIO_FFT_Init(AUDIO_FFT_instance_t* AUDIO_FFT_instance);
int32_t AUDIO_FFT_Data_Input(int16_t * data, uint32_t len, AUDIO_FFT_instance_t* AUDIO_FFT_instance);
int32_t AUDIO_FFT_Process(AUDIO_FFT_instance_t* AUDIO_FFT_instance, float * output);


//    audio_fft_M1.context.dataIn;64
//    audio_fft_M1.context.scratch;32


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



#endif /* __AUDIO_FFT_H */
