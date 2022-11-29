/**
  ******************************************************************************
  * @file    iks02a1_audio.h
  * @author  SRA - Central Labs
  * @brief   This file contains the common defines and functions prototypes for
  *          the iks02a1_audio.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS02A1_AUDIO_H
#define IKS02A1_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_conf.h"
#include <stdlib.h>

/* Include PDM to PCM lib header file */
#include "pdm2pcm_glo.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @defgroup IKS02A1_AUDIO_ IKS02A1 AUDIO
  * @{
  */

/** @defgroup IKS02A1_AUDIO_Exported_Variables IKS02A1_AUDIO_ Exported Variables
  * @{
  */
#ifdef USE_STM32L4XX_NUCLEO
extern SAI_HandleTypeDef                hAudioInSai;
#elif defined(USE_STM32F4XX_NUCLEO)
extern I2S_HandleTypeDef                hAudioInI2s;
#endif

#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)
#define HTONS(A)  ((((A) & (uint16_t)0xff00) >> 8) | (((A) & (uint16_t)0x00ff) << 8))

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_Exported_Types IKS02A1_AUDIO_ Exported Types
  * @{
  */

typedef struct
{
  int32_t Z;
  int32_t oldOut;
  int32_t oldIn;
}
HP_FilterState_TypeDef;

typedef struct
{
  uint32_t                    Device;
  uint32_t                    SampleRate;
  uint32_t                    BitsPerSample;
  uint32_t                    ChannelsNbr;
  uint32_t                    Volume;
} IKS02A1_AUDIO_Init_t;

typedef struct
{
  uint32_t                    Instance;            /* Audio IN instance              */
  uint32_t                    Device;              /* Audio IN device to be used     */
  uint32_t                    SampleRate;          /* Audio IN Sample rate           */
  uint32_t                    BitsPerSample;       /* Audio IN Sample resolution     */
  uint32_t                    ChannelsNbr;         /* Audio IN number of channel     */
  uint16_t                    *pBuff;              /* Audio IN record buffer         */
  uint8_t                     **pMultiBuff;        /* Audio IN multi-buffer          */
  uint32_t                    Size;                /* Audio IN record buffer size    */
  uint32_t                    Volume;              /* Audio IN volume                */
  uint32_t                    State;               /* Audio IN State                 */
  uint32_t                    IsMultiBuff;         /* Audio IN multi-buffer usage    */
  uint32_t                    IsMspCallbacksValid; /* Is Msp Callbacks registred     */
  HP_FilterState_TypeDef    HP_Filter;       /*!< HP filter state for each channel*/
  uint32_t DecimationFactor;
} AUDIO_IN_Ctx_t;

typedef struct
{
  uint32_t Mode;
  uint32_t Standard;
  uint32_t DataFormat;
  uint32_t MCLKOutput;
  uint32_t AudioFreq;
  uint32_t CPOL;
  uint32_t ClockSource;
  uint32_t FullDuplexMode;
} MX_I2S_IN_Config;

typedef struct
{
  uint32_t Protocol;
  uint32_t AudioMode;
  uint32_t DataSize;
  uint32_t FirstBit;
  uint32_t ClockStrobing;
  uint32_t Synchro;
  uint32_t OutputDrive;
  uint32_t NoDivider;
  uint32_t FIFOThreshold;
  uint32_t AudioFrequency;
  uint32_t Mckdiv;
  uint32_t SynchroExt;
  uint32_t MonoStereoMode;
  uint32_t CompandingMode;
  uint32_t FrameLength;
  uint32_t ActiveFrameLength;
  uint32_t FSDefinition;
  uint32_t FSPolarity;
  uint32_t FSOffset;
  uint32_t FirstBitOffset;
  uint32_t SlotSize;
  uint32_t SlotNumber;
  uint32_t SlotActive;

} MX_SAI_IN_Config;

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_Exported_Constants IKS02A1_AUDIO_ Exported Constants
  * @{
  */

/* AUDIO FREQUENCY */

#ifndef AUDIO_FREQUENCY_48K
#define AUDIO_FREQUENCY_48K       (uint32_t)48000U
#endif
#ifndef AUDIO_FREQUENCY_32K
#define AUDIO_FREQUENCY_32K       (uint32_t)32000U
#endif
#ifndef AUDIO_FREQUENCY_16K
#define AUDIO_FREQUENCY_16K       (uint32_t)16000U
#endif
#ifndef AUDIO_FREQUENCY_8K
#define AUDIO_FREQUENCY_8K         (uint32_t)8000U
#endif

/* AUDIO RESOLUTION */
#ifndef AUDIO_RESOLUTION_16b
#define AUDIO_RESOLUTION_16b                16U
#endif
#ifndef AUDIO_RESOLUTION_24b
#define AUDIO_RESOLUTION_24b                24U
#endif
#ifndef AUDIO_RESOLUTION_32b
#define AUDIO_RESOLUTION_32b                32U
#endif

/*------------------------------------------------------------------------------
                          USER I2S / SPI defines parameters
 -----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                        AUDIO IN defines parameters
------------------------------------------------------------------------------*/
#ifdef USE_STM32L4XX_NUCLEO /* defines valid only for STM32WB */

/* I2S Configuration defines */
#define AUDIO_IN_SAI_INSTANCE                                   SAI1_Block_B
#define AUDIO_IN_SAI_CLK_ENABLE()                               __SAI1_CLK_ENABLE()
#define AUDIO_IN_SAI_SCK_PIN                                    GPIO_PIN_3
#define AUDIO_IN_SAI_SCK_GPIO_PORT                              GPIOB
#define AUDIO_IN_SAI_SCK_GPIO_CLK_ENABLE()                      __GPIOB_CLK_ENABLE()
#define AUDIO_IN_SAI_SCK_AF                                     GPIO_AF13_SAI1
#define AUDIO_IN_SAI_SD_PIN                                     GPIO_PIN_5
#define AUDIO_IN_SAI_SD_GPIO_PORT                               GPIOB
#define AUDIO_IN_SAI_SD_GPIO_CLK_ENABLE()                       __GPIOB_CLK_ENABLE();
#define AUDIO_IN_SAI_SD_AF                                      GPIO_AF13_SAI1

/* I2S DMA definitions */
#define AUDIO_IN_SAI_DMAx_CLK_ENABLE()                          __DMA2_CLK_ENABLE()
#define AUDIO_IN_SAI_DMAx_INSTANCE                              DMA2_Channel2
#define AUDIO_IN_SAI_DMAx_REQUEST                               DMA_REQUEST_1
#define AUDIO_IN_SAI_DMAx_IRQ                                   DMA2_Channel2_IRQn
#define AUDIO_IN_SAI_DMAx_PERIPH_DATA_SIZE                      DMA_PDATAALIGN_HALFWORD
#define AUDIO_IN_SAI_DMAx_MEM_DATA_SIZE                         DMA_MDATAALIGN_HALFWORD
#define AUDIO_IN_SAI_IRQHandler                                 DMA2_Channel2_IRQHandler

#else

/* I2S Configuration defines */
#define AUDIO_IN_I2S_INSTANCE                                   SPI3
#define AUDIO_IN_I2S_CLK_ENABLE()                               __SPI3_CLK_ENABLE()
#define AUDIO_IN_I2S_SCK_PIN                                    GPIO_PIN_3
#define AUDIO_IN_I2S_SCK_GPIO_PORT                              GPIOB
#define AUDIO_IN_I2S_SCK_GPIO_CLK_ENABLE()                      __GPIOB_CLK_ENABLE()
#define AUDIO_IN_I2S_SCK_AF                                     GPIO_AF6_SPI3
#define AUDIO_IN_I2S_MOSI_PIN                                   GPIO_PIN_5
#define AUDIO_IN_I2S_MOSI_GPIO_PORT                             GPIOB
#define AUDIO_IN_I2S_MOSI_GPIO_CLK_ENABLE()                     __GPIOB_CLK_ENABLE();
#define AUDIO_IN_I2S_MOSI_AF                                    GPIO_AF6_SPI3

/* I2S DMA definitions */
#define AUDIO_IN_I2S_DMAx_CLK_ENABLE()                          __DMA1_CLK_ENABLE()
#define AUDIO_IN_I2S_DMAx_STREAM                                DMA1_Stream0
#define AUDIO_IN_I2S_DMAx_CHANNEL                               DMA_CHANNEL_0
#define AUDIO_IN_I2S_DMAx_IRQ                                   DMA1_Stream0_IRQn
#define AUDIO_IN_I2S_DMAx_PERIPH_DATA_SIZE                      DMA_PDATAALIGN_HALFWORD
#define AUDIO_IN_I2S_DMAx_MEM_DATA_SIZE                         DMA_MDATAALIGN_HALFWORD
#define AUDIO_IN_I2S_IRQHandler                                 DMA1_Stream0_IRQHandler

#endif

/* Audio In devices */
#ifndef AUDIO_IN_CHANNELS
#define AUDIO_IN_CHANNELS 1
#endif

#ifndef AUDIO_IN_SAMPLING_FREQUENCY
#define AUDIO_IN_SAMPLING_FREQUENCY 16000
#endif

#ifndef AUDIO_VOLUME_INPUT
#define AUDIO_VOLUME_INPUT              4U
#endif

#ifndef IKS02A1_AUDIO_INSTANCE
#define IKS02A1_AUDIO_INSTANCE 0U
#endif

#ifndef IKS02A1_AUDIO_IN_IT_PRIORITY
#define IKS02A1_AUDIO_IN_IT_PRIORITY    6U
#endif

/* MP34DT05TR digital microphone on PCB */
#define AUDIO_IN_DIGITAL_MIC1      0x10U
#define AUDIO_IN_DIGITAL_MIC_LAST  AUDIO_IN_DIGITAL_MIC1
#define AUDIO_IN_DIGITAL_MIC       (AUDIO_IN_DIGITAL_MIC1)
#define DFSDM_MIC_NUMBER           AUDIO_IN_CHANNELS

/* Buffer size defines for F4 and F7*/

#define MAX_MIC_FREQ                    3072  /*KHz*/
#define MAX_AUDIO_IN_CHANNEL_NBR_PER_IF   1
#define MAX_AUDIO_IN_CHANNEL_NBR_TOTAL    1

#ifndef PDM_FREQ_16K
#define PDM_FREQ_16K 1280
#endif

/*Number of millisecond of audio at each DMA interrupt*/
#ifndef N_MS_PER_INTERRUPT
#define N_MS_PER_INTERRUPT               (1U)
#endif

/* Default Audio IN internal buffer size */
#define DEFAULT_AUDIO_IN_BUFFER_SIZE        (AUDIO_IN_SAMPLING_FREQUENCY/1000)*2*N_MS_PER_INTERRUPT

/*BSP internal buffer size in half words (16 bits)*/
#define PDM_INTERNAL_BUFFER_SIZE_SAI          ((MAX_MIC_FREQ / 8) * MAX_AUDIO_IN_CHANNEL_NBR_PER_IF * N_MS_PER_INTERRUPT)
#define PDM_INTERNAL_BUFFER_SIZE_I2S          ((MAX_MIC_FREQ / 8) * MAX_AUDIO_IN_CHANNEL_NBR_PER_IF * N_MS_PER_INTERRUPT)

/* Audio In states */
#define AUDIO_IN_STATE_RESET               0U
#define AUDIO_IN_STATE_RECORDING           1U
#define AUDIO_IN_STATE_STOP                2U
#define AUDIO_IN_STATE_PAUSE               3U

/* Audio In instances number:
   Instance 0 is SAI-I2S / SPI path
   Instance 1 is DFSDM path
   Instance 2 is PDM path
 */
/*tODO DEFINES FIR INSTANES */
#define AUDIO_IN_INSTANCES_NBR             3U
/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_Exported_Macros IKS02A1_AUDIO_ Exported Macros
  * @{
  */
#ifndef POS_VAL
#define POS_VAL(VAL)                  (POSITION_VAL(VAL) - 4U)
#endif
#define VOLUME_IN_CONVERT(Volume)     (((Volume) >= 100)? 239:((uint8_t)(((Volume) * 239) / 100)))

/**
  * @}
  */
/** @addtogroup IKS02A1_AUDIO_Exported_Variables
  * @{
  */
/* Recording context */
extern AUDIO_IN_Ctx_t                         AudioInCtx[];
/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_IN_Exported_Functions IKS02A1_AUDIO_IN Exported Functions
  * @{
  */
int32_t IKS02A1_AUDIO_IN_Init(uint32_t Instance, IKS02A1_AUDIO_Init_t *AudioInit);
int32_t IKS02A1_AUDIO_IN_DeInit(uint32_t Instance);

int32_t IKS02A1_AUDIO_IN_Record(uint32_t Instance, uint8_t *pBuf, uint32_t NbrOfBytes);
int32_t IKS02A1_AUDIO_IN_Stop(uint32_t Instance);
int32_t IKS02A1_AUDIO_IN_Pause(uint32_t Instance);
int32_t IKS02A1_AUDIO_IN_Resume(uint32_t Instance);

int32_t IKS02A1_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes);
int32_t IKS02A1_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device);
int32_t IKS02A1_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device);
int32_t IKS02A1_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device);

int32_t IKS02A1_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device);
int32_t IKS02A1_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device);
int32_t IKS02A1_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t SampleRate);
int32_t IKS02A1_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate);
int32_t IKS02A1_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample);
int32_t IKS02A1_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample);
int32_t IKS02A1_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr);
int32_t IKS02A1_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr);
int32_t IKS02A1_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume);
int32_t IKS02A1_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume);
int32_t IKS02A1_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State);

/* Specific PDM recodr APIs */
int32_t IKS02A1_AUDIO_IN_PDMToPCM_Init(uint32_t Instance, uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
int32_t IKS02A1_AUDIO_IN_PDMToPCM(uint32_t Instance, uint16_t *PDMBuf, uint16_t *PCMBuf);
int32_t IKS02A1_AUDIO_IN_RecordPDM(uint32_t Instance, uint8_t *pBuf, uint32_t NbrOfBytes);

void IKS02A1_AUDIO_IN_IRQHandler(uint32_t Instance, uint32_t Device);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void IKS02A1_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance);
void IKS02A1_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void IKS02A1_AUDIO_IN_Error_CallBack(uint32_t Instance);

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_IN_Private_Functions IKS02A1_AUDIO_IN Private Functions
  * @{
  */

#ifdef USE_STM32L4XX_NUCLEO
/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_SAI_IN_ClockConfig(SAI_HandleTypeDef *hSai, uint32_t PDM_rate);
HAL_StatusTypeDef MX_SAI_IN_Init(SAI_HandleTypeDef *hSai, MX_SAI_IN_Config *MXConfig);

#else
HAL_StatusTypeDef MX_I2S_IN_ClockConfig(I2S_HandleTypeDef *hi2s, uint32_t PDM_rate);
HAL_StatusTypeDef MX_I2S_IN_Init(I2S_HandleTypeDef *hi2s, MX_I2S_IN_Config *MXConfig);
#endif

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

#endif /* IKS02A1_AUDIO_H */
