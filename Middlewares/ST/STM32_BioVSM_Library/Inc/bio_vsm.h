/**
  ******************************************************************************
  * @file    bio_vsm.h
  * @author  MEMS Application Team
  * @brief   Header for bio_vsm module
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
#ifndef _BIO_VSM_H_
#define _BIO_VSM_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup BIO_VSM BIO_VSM
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup BIO_VSM_Exported_Types BIO_VSM_Exported_Types
  * @{
  */

typedef void *VSM_Instance_t;

typedef enum
{
  BIO_VSM_MCU_STM32 = 0,
  BIO_VSM_MCU_BLUE_NRG1,
  BIO_VSM_MCU_BLUE_NRG2,
  BIO_VSM_MCU_BLUE_NRG_LP,
  BIO_VSM_MCU_STM32WB0,
} VSM_mcu_type_t;

typedef enum
{
  BIO_VSM_INIT_OK = 0,      	  /* No error */
  BIO_VSM_INIT_ERR_INSTANCE,      /* Instance pointer NULL */
  BIO_VSM_INIT_ERR_ALREADY,       /* Instance already initialized */
  BIO_VSM_INIT_ERR_DEVICE_CONFIG, /* Invalid device configuration parameter */
  BIO_VSM_INIT_ERR_ALGO_CONFIG,   /* Invalid algorithm configuration parameter */
  BIO_VSM_INIT_ERR_RES,           /* Reserved */
} VSM_init_err_t;

typedef enum
{
    BIO_VSM_DEINIT_OK = 0,       /* No error */
    BIO_VSM_DEINIT_ERR_INSTANCE, /* Instance pointer NULL */
    BIO_VSM_DEINIT_ERR_UNINIT,   /* Instance not initialized */
    BIO_VSM_DEINIT_ERR_RES,      /* Reserved */
} VSM_deinit_err_t;

typedef enum {
	BIO_VSM_RUN_OK = 0,			/* No error */
	BIO_VSM_RUN_ERR_INSTANCE, 	/* Instance NULL */
	BIO_VSM_RUN_ERR_UNINIT, 	/* Instance not initialized */
	BIO_VSM_RUN_ERR_RES, 		/* Reserved error code */
} VSM_run_err_t;

typedef enum
{
    BIO_VSM_MAINS_NONE = 0, /* No mains frequency (battery-powered device) */
    BIO_VSM_MAINS_50Hz = 1, /* 50 Hz mains frequency */
    BIO_VSM_MAINS_60Hz = 2, /* 60 Hz mains frequency */
} VSM_mains_t;

typedef struct
{
    uint16_t odr;       /* ODR [Hz] */
    uint16_t vafe_sens; /* vAFE sensitivity [LSB/mV] */
    float ecg_gain;     /* ECG acquisition system gain [-], set to 1 if unknown */
    float ecg_offset;   /* ECG acquisition system offset [mV], set to 0 if unknown */
    VSM_mains_t mains;  /* Mains frequency */
} VSM_device_config_t;

typedef struct
{
    float cutoff_low;  /* ECG band-pass filter low cut-off frequencies */
    float cutoff_high; /* ECG band-pass filter high cut-off frequencies */
    bool lead_off_en;  /* Lead-off detection enabling flag (performances are reduced when disabled) */
    bool res;          /* Reserved */
} VSM_algo_config_t;

typedef struct
{
    int16_t vafe; /* vAFE data [LSB] */
} VSM_in_t;

typedef struct
{
    /* ECG */
    float ecg;       /* Band-pass filtered ECG voltage [mV] */
    bool lead_off;   /* Lead-off detection (if enabled, when this parameter is set to true,
                                all the algorithms are reset) */

    /* Peak detection */
    bool peak;       /* N-peak detection (in the samples where this is set to true,
                                all the following parameters are updated) */
    float nn;        /* Most recent NN-interval value [ms] */

    /* Heart rate */
    uint8_t hbr;     /* Heartbeat rate [bpm] */
    uint8_t hbr_max; /* Maximum heartbeat rate [bpm] */
    uint8_t hbr_min; /* Minimum heartbeat rate [bpm] */

    /* Heart rate variability */
    float sdnn;      /* Standard deviation of NN-intervals [ms] */
    float rmssd;     /* Root mean square of successive differences of NN-intervals [ms] */
    float sdsd;      /* Standard deviation of successive differences of NN-intervals [ms] */
    float pnn50;     /* Percentage of successive differences of NN-intervals
                                that are higher than 50 ms [%] */
    float pnn20;     /* Percentage of successive differences of NN-intervals
                                that are higher than 20 ms [%] */
} VSM_out_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup BIO_VSM_Exported_Functions BIO_VSM_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize BioVSM library
  * @param  mcu_type  MCU type
  * @retval None
  */
void BioVSM_Initialize(VSM_mcu_type_t mcu_type);

/**
  * @brief  Create instance of BioVSM library
  * @param  algo_config  Configuration of the algorithms of the instance
  * @retval Pointer to the algorithm instance
  */
VSM_Instance_t BioVSM_CreateInstance(VSM_algo_config_t *algo_conf);

/**
  * @brief  Delete instance of BioVSM library
  * @param  instance  The algorithm instance
  * @retval None
  */
void BioVSM_DeleteInstance(VSM_Instance_t instance);

/**
  * @brief  Start the BioVSM engine
  * @param  instance  The algorithm instance
  * @param  device_config  Configuration of the device in use with the
                           instance
  * @param  algo_config  Configuration of the algorithms of the instance
  * @retval Error code
  */
VSM_init_err_t BioVSM_Start(VSM_Instance_t instance, VSM_device_config_t *device_conf, VSM_algo_config_t *algo_conf);

/**
  * @brief  Execute one step of the algorithms
  * @param  instance  The algorithm instance
  * @param  data_in  algorithm input data
  * @param  data_out  algorithm output data
  * @retval None
  */
VSM_run_err_t BioVSM_Update(VSM_Instance_t instance, VSM_in_t *data_in, VSM_out_t *data_out);

/**
 * @brief Deinitialize the algorithm of a library instance.
 * @note After an instance has been deinitialized, it can be initialized again.
 *
 * @param state Pointer to the library instance
 * @return Error code
 */
VSM_deinit_err_t BioVSM_deinit(VSM_Instance_t *instance);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t BioVSM_GetLibVersion(char *version);

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

#endif /* _BIO_VSM_H_ */
