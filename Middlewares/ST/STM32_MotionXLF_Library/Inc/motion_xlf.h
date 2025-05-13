/**
  ******************************************************************************
  * @file    motion_xlf.h
  * @author  MEMS Application Team
  * @brief   Header for motion_xlf module
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
#ifndef _MOTION_XLF_H_
#define _MOTION_XLF_H_

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

/** @defgroup MOTION_XLF MOTION_XLF
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_XLF_Exported_Types MOTION_XLF_Exported_Types
  * @{
  */

#define ACCEL_FUSION_ODR_TEMP 960  /* Initial ODR of the library */

typedef void *XLF_Instance_t;

typedef enum
{
  MOTION_XLF_MCU_STM32 = 0,
  MOTION_XLF_MCU_BLUE_NRG1,
  MOTION_XLF_MCU_BLUE_NRG2,
  MOTION_XLF_MCU_BLUE_NRG_LP,
  MOTION_XLF_MCU_STM32WB0,
} XLF_mcu_type_t;

typedef enum
{
  MOTION_XLF_ALGO_OK = 0x00,       /* No error */
  MOTION_XLF_ALGO_ERROR = 0x01,    /* Algotithm error */
  MOTION_XLF_ALGO_READY = 0x02,    /* Algorithm is ready */
  MOTION_XLF_ALGO_NOT_READY = 0x03 /* Algorithm is not ready */
} XLF_return_t;

typedef struct
{
    float x;    /* Units: accelerometer: [mg], gyroscope: [dps], magnetometer: [micro tesla] */
    float y;    /* Units: accelerometer: [mg], gyroscope: [dps], magnetometer: [micro tesla] */
    float z;    /* Units: accelerometer: [mg], gyroscope: [dps], magnetometer: [micro tesla] */
}XLF_imu_data_t;

typedef struct
{
	int x[20];    /* Binary array 20-bit */
	int y[20];    /* Binary array 20-bit */
	int z[20];    /* Binary array 20-bit */
}XLF_imu_data_digital_t;

typedef struct
{
	XLF_imu_data_t low_g_data_mg;    /* Data from low-g sensor in mg */
	XLF_imu_data_t high_g_data_mg;   /* Data from high-g sensor in mg */
	char FSM_OUT1;                   /* Value in FSM_OUTS1 register (output of FSM1) */
	char FSM_OUT2;                   /* Value in FSM_OUTS2 register (output of FSM2) */
} XLF_in_t;

typedef struct
{
	XLF_imu_data_t fused_imu;                 /* Output of the library in mg (float) */
	XLF_imu_data_digital_t fused_imu_digital; /* Output of the library in 20-bit binary */
} XLF_out_t;

typedef struct XLF_algo_settings
{
	int32_t ACCEL_FUSION_CALIB_WINDOW_SIZE;             /* number of samples to use for initial calibration */
	int32_t ACCEL_FUSION_DIGITAL_OUTPUT;                /* enable (1) or disable (0) 20-bit output (if disabled, output is float) */
	int32_t ACCEL_FUSION_ENABLE_NOISE_REMOVAL;          /* enable (1) or disable (0) noise handling for high-g data */
	int32_t ACCEL_FUSION_ENABLE_DISCONTINUITY_REMOVAL;  /* enable (1) or disable discontinuity removal module */
	int32_t ACCEL_FUSION_ENABLE_OFFSET_CALCULATOR;      /* enable (1) or disable (0) real-time offset compensation module */
	int32_t ACCEL_FUSION_CONTINUOUS_TRACKING;           /* if enabled (1), then high-g sensor won't be turned off when not needed */
	int32_t ACCEL_FUSION_LOWER_THRESHOLD;
} XLF_algo_settings;

typedef void (*XLF_high_g_enable_disable_pointer_t)(void); /* Function pointer type definition to be used
 	 	 	                    to pass functions that enable or disable the high-g sensor to the library */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_XLF_Exported_Functions MOTION_XLF_Exported_Functions
  * @{
  */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize MotionXLF library
  * @param  mcu_type  MCU type
  * @retval None
  */
void MotionXLF_Initialize(XLF_mcu_type_t mcu_type);

/**
  * @brief  Reset the state of the library including all buffers
  * @retval Error code
  */
XLF_return_t MotionXLF_Reset(void);

/**
  * @brief  Start the MotionXLF engine
  * @retval Error code
  */
XLF_return_t MotionXLF_Start(void);

/**
  * @brief  Execute one step of the algorithms
  * @param  data_in  algorithm input data
  * @param  data_out  algorithm output data
  * @param  enable_high_g  function pointer for enabling high-g sensor
  * @param  disable_high_g  function pointer for disabling high-g sensor
  * @retval Error code
  */
XLF_return_t MotionXLF_Update(XLF_in_t *data_in, XLF_out_t *data_out, XLF_high_g_enable_disable_pointer_t enable_high_g, XLF_high_g_enable_disable_pointer_t disable_high_g, XLF_algo_settings *algo_set);

/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionXLF_GetLibVersion(char *version);

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

#endif /* _MOTION_XLF_H_ */
