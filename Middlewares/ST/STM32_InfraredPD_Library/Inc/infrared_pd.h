/**
  ******************************************************************************
  * @file    infrared_pd.h
  * @author  MEMS Application Team
  * @brief   Header for infrared_pd module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _INFRARED_PD_H_
#define _INFRARED_PD_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup INFRARED_PD INFRARED_PD
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup INFRARED_PD_Exported_Types INFRARED_PD_Exported_Types
  * @{
  */

typedef void *IPD_Instance_t;

typedef enum
{
  IPD_MCU_STM32 = 0,
  IPD_MCU_BLUE_NRG1,
  IPD_MCU_BLUE_NRG2,
  IPD_MCU_BLUE_NRG_LP,
  IPD_MCU_STM32WB0,
} IPD_mcu_type_t;

typedef enum
{
  IPD_INIT_OK = 0,       /* No error */
  IPD_INIT_ERR_ODR,      /* Wrong ODR value */
  IPD_INIT_ERR_AVGTMOS,  /* Wrong number of average for Tobj */
  IPD_INIT_ERR_AVGT,     /* Wrong number of average for Tamb */
  IPD_INIT_ERR_GAIN,     /* Wrong gain reduction factor value */
  IPD_INIT_ERR_TRANS,    /* Wrong transmittance value */
  IPD_INIT_ERR_RES1,     /* Reserved error code */
  IPD_INIT_ERR_RES2      /* Reserved error code */
} IPD_init_err_t;

typedef enum
{
  IPD_COMP_NONE,   /* No compensation */
  IPD_COMP_LIN,    /* Linear compensation */
  IPD_COMP_NONLIN  /* Non-linear compensation */
} IPD_comp_t;

typedef struct
{
  uint8_t  odr;            /* ODR [Hz] (from 1 to 30) */
  uint16_t avg_tmos;       /* Number of average for Tobj (up to 1024) */
  uint8_t  avg_t;          /* Number of average for Tamb */
  uint8_t  gain_factor;    /* Gain reduction factor (either 1 or 8) */
  uint16_t sens_data;      /* Calibrated sensitivity of the unit */
  float    transmittance;  /* Transmittance of the optical system, ranging
                              from 0 [0%] to 1 [100%] */
} IPD_device_conf_t;

typedef struct
{
  IPD_comp_t comp_type;         /* Flag to choose Tobject ambient compensation
                                   type */
  uint8_t    comp_filter_flag;  /* Flag to enable or disable moving average
                                   filter after compensation */
  uint16_t   mot_ths;           /* Threshold for motion detection [LSB] */
  uint16_t   pres_ths;          /* Threshold for presence detection [LSB] */
  uint8_t    abs_static_flag;   /* Flag to enable or disable absence state
                                   trigger in static conditions */
} IPD_algo_conf_t;

typedef struct
{
  int16_t t_amb;  /* Tambient data [LSB] */
  int16_t t_obj;  /* Tobject data [LSB] */
} IPD_input_t;

typedef struct
{
  int16_t t_obj_comp;    /* Ambient-compensated Tobject data [LSB] */
  int16_t t_obj_change;  /* Rate of change of ambient-compensated Tobject
                            data [LSB] */
  uint8_t mot_flag;      /* Motion detection flag [0: standstill, 1: movement] */
  uint8_t pres_flag;     /* Presence detection flag [0: absence, 1: presence] */
} IPD_output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup INFRARED_PD_Exported_Functions INFRARED_PD_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize InfraredPD library
  * @param  mcu_type  MCU type
  * @retval None
  */
void InfraredPD_Initialize(IPD_mcu_type_t mcu_type);

/**
  * @brief  Create instance of InfraredPD library
  * @param  algo_config  Configuration of the algorithms of the instance
  * @retval Pointer to the algorithm instance
  */
IPD_Instance_t InfraredPD_CreateInstance(IPD_algo_conf_t *algo_conf);

/**
  * @brief  Delete instance of InfraredPD library
  * @param  instance  The algorithm instance
  * @retval None
  */
void InfraredPD_DeleteInstance(IPD_Instance_t instance);

/**
  * @brief  Start the InfraredPD engine
  * @param  instance  The algorithm instance
  * @param  device_config  Configuration of the device in use with the
                           instance
  * @param  algo_config  Configuration of the algorithms of the instance
  * @retval Error code
  */
IPD_init_err_t InfraredPD_Start(IPD_Instance_t instance, IPD_device_conf_t *device_conf, IPD_algo_conf_t *algo_conf);

/**
  * @brief  Execute one step of the algorithms
  * @param  instance  The algorithm instance
  * @param  data_in  algorithm input data
  * @param  data_out  algorithm output data
  * @retval None
  */
void InfraredPD_Update(IPD_Instance_t instance, IPD_input_t *data_in, IPD_output_t *data_out);

/**
  * @brief  Reset algorithm for Tobject ambient compensation
  * @param  instance  The algorithm instance
  * @retval None
  */
void InfraredPD_ResetComp(IPD_Instance_t instance);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t InfraredPD_GetLibVersion(char *version);

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

#endif /* _INFRARED_PD_H_ */
