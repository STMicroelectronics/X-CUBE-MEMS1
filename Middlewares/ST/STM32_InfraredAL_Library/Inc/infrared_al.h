/**
  ******************************************************************************
  * @file    infrared_al.h
  * @author  MEMS Application Team
  * @brief   Header for infrared_al module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _INFRARED_AL_H_
#define _INFRARED_AL_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup INFRARED_AL INFRARED_AL
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup INFRARED_AL_Exported_Types INFRARED_AL_Exported_Types
  * @{
  */

typedef void *IAL_Instance_t;

typedef enum
{
  IAL_MCU_STM32 = 0,
  IAL_MCU_BLUE_NRG1,
  IAL_MCU_BLUE_NRG2,
  IAL_MCU_BLUE_NRG_LP,
  IAL_MCU_STM32WB0,
} IAL_mcu_type_t;

typedef enum
{
  IAL_INIT_OK = 0,      /* No error */
  IAL_INIT_ERR_ALLOC, 	/* Instance NULL */
  IAL_INIT_ERR_ODR,     /* Wrong ODR value */
  IAL_INIT_ERR_RES,     /* Reserved error code */
} IAL_init_err_t;

typedef enum {
	IAL_RUN_OK = 0,			/* No error */
	IAL_RUN_ERR_ALLOC, 		/* Instance NULL */
	IAL_RUN_ERR_UNINIT, 	/* Instance not initialized */
	IAL_RUN_ERR_RES, 		/* Reserved error code */
} IAL_run_err_t;

typedef enum {
	IAL_PRES_STATE_UNKNOWN = 0,     /* Unknown presence state */
	IAL_PRES_STATE_PRESENCE,        /* Presence state: presence */
	IAL_PRES_STATE_ABSENCE,         /* Presence state: absence */
} IAL_pres_state_t;

typedef struct {
	uint8_t odr;                /* ODR [Hz] */
	uint8_t tau;                /* Transmittance of the optical sistem [%] */
} IAL_device_conf_t;

typedef struct {
	uint16_t ths;                  /* Threshold for presence detection [LSB] */
	uint16_t abs_lat;			   /* Latency for absence trigger [ms] */
	IAL_pres_state_t pres_init;    /* Initial presence state */
} IAL_algo_conf_t;

typedef struct
{
  int16_t t_obj;  /* Tobject data [LSB] */
} IAL_input_t;

typedef struct
{
  uint8_t pres_flag;     /* Presence detection flag [0: absence, 1: presence] */
  uint8_t pres_conf;     /* Presence detection confidence [%] */
} IAL_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup INFRARED_AL_Exported_Functions INFRARED_AL_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize InfraredAL library
  * @param  mcu_type  MCU type
  * @retval None
  */
void InfraredAL_Initialize(IAL_mcu_type_t mcu_type);

/**
  * @brief  Create instance of InfraredAL library
  * @param  algo_config  Configuration of the algorithms of the instance
  * @retval Pointer to the algorithm instance
  */
IAL_Instance_t InfraredAL_CreateInstance(IAL_algo_conf_t *algo_conf);

/**
  * @brief  Delete instance of InfraredAL library
  * @param  instance  The algorithm instance
  * @retval None
  */
void InfraredAL_DeleteInstance(IAL_Instance_t instance);

/**
  * @brief  Start the InfraredAL engine
  * @param  instance  The algorithm instance
  * @param  device_config  Configuration of the device in use with the
                           instance
  * @param  algo_config  Configuration of the algorithms of the instance
  * @retval Error code
  */
IAL_init_err_t InfraredAL_Start(IAL_Instance_t instance, IAL_device_conf_t *device_conf, IAL_algo_conf_t *algo_conf);

/**
  * @brief  Execute one step of the algorithms
  * @param  instance  The algorithm instance
  * @param  data_in  algorithm input data
  * @param  data_out  algorithm output data
  * @retval None
  */
IAL_run_err_t InfraredAL_Update(IAL_Instance_t instance, IAL_input_t *data_in, IAL_output_t *data_out);

/**
  * @brief Force a presence state value.
  *
  * @param instance Pointer to the algorithm instance
  * @param pres_state Presence state value to be forced
  * @retval None
  */
void InfraredAL_ForcePresState(IAL_Instance_t instance, IAL_pres_state_t pres_state);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t InfraredAL_GetLibVersion(char *version);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _INFRARED_AL_H_ */
