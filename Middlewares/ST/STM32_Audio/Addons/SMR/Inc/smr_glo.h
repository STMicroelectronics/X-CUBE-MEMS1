/**
  ******************************************************************************
  * @file    smr_glo.h
  * @author  MCD Application Team
  * @version V1.0.5
  * @date    15-Oct-2017
  * @brief   This file provides SMR routines.
  *          For more details about this Library, please refer to document.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Image SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_image_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
#ifndef SMR_GLO_H
#define SMR_GLO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_fw_glo.h"

/* Exported constants --------------------------------------------------------*/


/*      E R R O R   V A L U E S                                               */

#define SMR_ERROR_NONE                           0
#define SMR_UNSUPPORTED_INTERLEAVING_MODE       -1
#define SMR_UNSUPPORTED_NUMBER_OF_CHANNELS      -2
#define SMR_UNSUPPORTED_NUMBER_OF_BYTEPERSAMPLE -3
#define SMR_UNSUPPORTED_AVERAGING_TIME          -4
#define SMR_UNSUPPORTED_FILTER_TYPE             -5
#define SMR_UNSUPPORTED_SAMPLING_RATE           -6
#define SMR_BAD_HW                              -7


/*      PRE-FILTER VALUES                                                     */

#define SMR_PREFILTER_NONE            0
#define SMR_PREFILTER_AWEIGHTING      1
#define SMR_PREFILTER_CWEIGHTING      2
#define SMR_PREFILTER_DCREMOVE        3


/* Exported types ------------------------------------------------------------*/

struct smr_dynamic_param {
    int32_t enable;
    int16_t averaging_time;
    int16_t filter_type;
    int32_t mean_level_left;
    int32_t mean_level_right;
};
typedef struct smr_dynamic_param smr_dynamic_param_t;

struct smr_static_param {
    int32_t sampling_rate;
};
typedef struct smr_static_param smr_static_param_t;

/* External variables --------------------------------------------------------*/

extern const uint32_t smr_scratch_mem_size;
extern const uint32_t smr_persistent_mem_size;


/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* smr_reset() : initializes static memory, states machines, ... */
extern int32_t smr_reset(void *persistent_mem_ptr, void *scratch_mem_ptr);

/* smr_setConfig() : use to change dynamically some parameters */
extern int32_t smr_setConfig(smr_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);

/* smr_getConfig() : use to get values of dynamic parameters */
extern int32_t smr_getConfig(smr_dynamic_param_t *input_dynamic_param_ptr, void *persistent_mem_ptr);

/* smr_getParam() : use to get values of static parameters */
extern int32_t smr_getParam(smr_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);

/* smr_setParam() : use to set parameters that won't change during processing */
extern int32_t smr_setParam(smr_static_param_t *input_static_param_ptr, void *persistent_mem_ptr);

/* smr_process() : this is the main processing routine */
extern int32_t smr_process(buffer_t *input_buffer, buffer_t *output_buffer, void *persistent_mem_ptr);


#ifdef __cplusplus
}
#endif

#endif /* SMR_GLO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
