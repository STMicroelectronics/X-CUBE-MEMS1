/**
  ******************************************************************************
  * @file    motion_mc_cm0p.h
  * @author  MEMS Application Team
  * @version V2.5.1
  * @date    10-August-2021
  * @brief   Header for motion_mc_cm0p module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement SLA0077,
  * the “License”. You may not use this component except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        www.st.com/sla0077
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_MC_CM0P_H_
#define _MOTION_MC_CM0P_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_MC_CM0P MOTION_MC_CM0P
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MMC_CM0P_MCU_STM32 = 0,
  MMC_CM0P_MCU_BLUE_NRG1,
  MMC_CM0P_MCU_BLUE_NRG2,
  MMC_CM0P_MCU_BLUE_NRG_LP,
} MMC_CM0P_mcu_type_t;

typedef enum
{
  MMC_CM0P_HI_ONLY = 0,
  MMC_CM0P_HI_AND_SI,
} MMC_CM0P_Mode_t;

typedef enum
{
  MMC_CM0P_CALQSTATUSUNCALIBRATED = 0,
  MMC_CM0P_CALQSTATUSPOOR,
  MMC_CM0P_CALQSTATUSGOOD,
  MMC_CM0P_CALQSTATUSBEST
} MMC_CM0P_CalQuality_t;

typedef struct
{
  float DTime;   /* Delta time since previous call [s] */
  float Mag[3];  /* Magnetometer sensor output [uT] */
} MMC_CM0P_Input_t;

typedef struct
{
  float HI_Bias[3];                  /* Hard iron offset array [uT] */
  float SI_Matrix[3][3];             /* Soft iron correction matrix [-] */
  float B;                           /* Estimated magnetic field [uT]
                                        (to be used in HI only mode) */
  MMC_CM0P_CalQuality_t CalQuality;  /* Calibration quality factor */
} MMC_CM0P_Output_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_MC_CM0P_Exported_Functions MOTION_MC_CM0P_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionMC_CM0P engine
  * @param  mcu_type MCU type
  * @param  mmc_algo  pointer to instance of MMC algorithm
  * @param  mode  calibration mode type, hard-iron only mode (faster) or hard-iron + soft-iron
  * @param  enable  Enable (1) or disable (0) library
  * @retval New pointer to instance of MMC algorithm
  */
void *MotionMC_CM0P_Initialize(MMC_CM0P_mcu_type_t mcu_type, void *mmc_algo, MMC_CM0P_Mode_t mode, unsigned short int enable);

/**
  * @brief  Run Magnetic Calibration algorithm
  * @param  mmc_algo  pointer to instance of MMC algorithm
  * @param  data_in  Structure containing input data
  * @retval none
  */
void MotionMC_CM0P_Update(void *mmc_algo, MMC_CM0P_Input_t *data_in);

/**
  * @brief  Get the magnetic compensation for hard/soft iron
  * @param  data_out  Structure containing output data
  * @retval none
  */
void MotionMC_CM0P_GetCalParams(MMC_CM0P_Output_t *data_out);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionMC_CM0P_GetLibVersion(char *version);

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

#endif /* _MOTION_MC_CM0P_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
