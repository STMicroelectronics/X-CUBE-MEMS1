/**
  ******************************************************************************
  * @file           : MotionSP_Config_Template.h
  * @author         : System Research & Applications Team - Catania Lab
  * @version        : v2.2.0
  * @date           : 30-May-2019
  * @brief          : MotionSP configuration file for motion signal processing.
  *                   This file should be copied to the application folder and
  *                   renamed to MotionSP_Config.h.
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
#ifndef __MOTIONSP_CONFIG_H
#define __MOTIONSP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Refer to preprocessor defined symbols */
#include "stm32yyxx_hal.h"

/** @addtogroup MIDDLEWARES Middlewares
  * @{
  */

/** @addtogroup ST ST
  * @{
  */

/** @addtogroup STM32_MOTIONSP_LIB STM32 Motion Signal Processing Library
  * @{
  */

/** @addtogroup STM32_MOTIONSP_LIB_EXPORTED_DEFINES STM32 Motion Signal Processing Library Exported Defines
  * @{
  */

/* #define USE_SUBRANGE */                        //!< Uncomment this define for enabling subrange

#define NUM_AXES              3             //!< Number of sensor axes

#define FFT_SIZE_DEFAULT      FFT_SIZE_MAX  //!< Default value for FFT size
#define FFT_OVL_DEFAULT       FFT_OVL_MAX   //!< Default value for OVERLAPPING
#define TAU_DEFAULT           50            //!< Default value for Moving RMS Filtering in ms
#define WINDOW_DEFAULT        HANNING       //!< Default value for Windowing Method
#define TD_DEFAULT            TD_SPEED      //!< Default value for Time Domain Analysis
#define TACQ_DEFAULT          5000          //!< Defualt value for Total acquisition time in ms

#define CIRC_BUFFER_RATIO_NUM 12u           //!< Buffer Ratio numerator
#define CIRC_BUFFER_RATIO_DEN 10u           //!< Buffer Ratio denominator
#define FFT_SIZE_256          256u          //!< FFT will be performed on 256 samples
#define FFT_SIZE_512          512u          //!< FFT will be performed on 512 samples
#define FFT_SIZE_1024         1024u         //!< FFT will be performed on 1024 samples
#define FFT_SIZE_2048         2048u         //!< FFT will be performed on 2048 samples
#define FFT_SIZE_MAX          FFT_SIZE_2048 //!< Max FFT size
#define FFT_OVL_MIN           5             //!< Max FFT overlapping
#define FFT_OVL_MAX           70            //!< Max FFT overlapping
#define MAG_SIZE_MAX          (uint16_t)(FFT_SIZE_MAX/2)                      //!< Max MAG size
#define CIRC_BUFFER_SIZE_MAX  (uint16_t)((FFT_SIZE_MAX*CIRC_BUFFER_RATIO_NUM)/CIRC_BUFFER_RATIO_DEN) //!< Max circular buffer for storing input values for FFT

#define DC_SMOOTH             0.975f        //!< Smooth parameter used for DC filtering  
#define GAMMA                 0.5f          //!< GAMMA parameter used for Integration Algorithm               
  
#define G_CONST               9.80665f                //!< in m/s^2
#define G_CONV                (float)(G_CONST/1000.0) //!< CONSTANT for conversion from mm/s^2 to m/s^2

#ifdef USE_SUBRANGE
  #define SUBRANGE_DEFAULT      8           //!< Default value for FFT output subranges
  #define SUBRANGE_MAX          64          //!< Default value for MAX Subranges to analyze
#endif /* USE_SUBRANGE */

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

#ifdef __cplusplus
}
#endif

#endif /* __MOTIONSP_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
