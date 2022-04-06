/**
  ******************************************************************************
  * @file    iks02a1_audio.c
  * @author  SRA - Central Labs
  * @version v1.0.0
  * @date    5-July-19
  * @brief   This file provides the audio driver for the iks02a1
  *          board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iks02a1_audio.h"
#include "iks02a1_conf.h"
#include "audio.h"
#include "arm_math.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup IKS02A1 IKS02A1
  * @{
  */

/** @defgroup IKS02A1_AUDIO_ IKS02A1 AUDIO
  * @{
  */

/** @defgroup IKS02A1_AUDIO_Private_Macros IKS02A1 AUDIO Private Macros
  * @{
  */
/*### RECORD ###*/

#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

#ifdef USE_STM32L4XX_NUCLEO

#define SAI_DIVIDER(__FREQUENCY__) \
  ((__FREQUENCY__) == (AUDIO_FREQUENCY_8K))  ? (4U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_16K)) ? (4U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_32K)) ? (9U) \
  : ((__FREQUENCY__) == (AUDIO_FREQUENCY_48K)) ? (6U) : (4U)

#endif

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_Exported_Variables IKS02A1 AUDIO Exported Variables
  * @{
  */
AUDIO_IN_Ctx_t                         AudioInCtx[AUDIO_IN_INSTANCES_NBR] = {0};
/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_Private_Variables IKS02A1_AUDIO_ Private Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_Private_Function_Prototypes IKS02A1 AUDIO Private Function Prototypes
  * @{
  */
static int16_t volume_level_to_dB(uint16_t level);

#ifdef USE_STM32L4XX_NUCLEO
SAI_HandleTypeDef            hAudioInSai;
static uint16_t SAI_InternalBuffer[PDM_INTERNAL_BUFFER_SIZE_SAI];
static void SAI_MspInit(SAI_HandleTypeDef *hsai);
#else
I2S_HandleTypeDef               hAudioInI2s;
static uint16_t I2S_InternalBuffer[PDM_INTERNAL_BUFFER_SIZE_I2S];
static void I2S_MspInit(I2S_HandleTypeDef *hi2s);
#endif

#define DECIMATOR_NUM_TAPS (16)
#define DECIMATOR_BLOCK_SIZE (16*N_MS_PER_INTERRUPT)
#define DECIMATOR_FACTOR 2
#define DECIMATOR_STATE_LENGTH (DECIMATOR_BLOCK_SIZE + (DECIMATOR_NUM_TAPS) -1)
static arm_fir_decimate_instance_q15 ARM_Decimator_State;

/* PDM filters params */
static PDM_Filter_Handler_t  PDM_FilterHandler;
static PDM_Filter_Config_t   PDM_FilterConfig;

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_IN_Exported_Functions IKS02A1 AUDIO Exported Functions
  * @{
  */


/**
  * @brief  Initialize wave recording.
  * @param  Instance  AUDIO IN Instance. It can be:
  *       - 0 when I2S is used
  *       - 1 if DFSDM is used
  *       - 2 if PDM is used
  * @param  AudioInit Init structure
  * @retval BSP status
  */
__weak int32_t IKS02A1_AUDIO_IN_Init(uint32_t Instance, IKS02A1_AUDIO_Init_t *AudioInit)
{
  int32_t ret =  BSP_ERROR_NONE;

  if (Instance != 0 || AudioInit->ChannelsNbr != 1)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Store the audio record context */
    AudioInCtx[Instance].Device          = AudioInit->Device;
    AudioInCtx[Instance].ChannelsNbr     = AudioInit->ChannelsNbr;
    AudioInCtx[Instance].SampleRate      = AudioInit->SampleRate;
    AudioInCtx[Instance].BitsPerSample   = AudioInit->BitsPerSample;
    AudioInCtx[Instance].Volume          = AudioInit->Volume;
    AudioInCtx[Instance].State           = AUDIO_IN_STATE_RESET;

    if (Instance == 0U) /* I2S SAI */
    {
      uint32_t PDM_Clock_Freq;

      switch (AudioInit->SampleRate)
      {
        case AUDIO_FREQUENCY_8K:
          PDM_Clock_Freq = 1280;
          break;

        case AUDIO_FREQUENCY_16K:
          PDM_Clock_Freq = PDM_FREQ_16K;
          break;

        case AUDIO_FREQUENCY_32K:
          PDM_Clock_Freq = 2048;
          break;

        case AUDIO_FREQUENCY_48K:
          PDM_Clock_Freq = 3072;
          break;

        default:
          PDM_Clock_Freq = 1280;
          ret =  BSP_ERROR_WRONG_PARAM;
          break;
      }

      AudioInCtx[Instance].DecimationFactor = (PDM_Clock_Freq * 1000U) / AudioInit->SampleRate;
      AudioInCtx[Instance].Size = (PDM_Clock_Freq / 8U) * 2U * N_MS_PER_INTERRUPT;


#ifdef USE_STM32L4XX_NUCLEO

      MX_SAI_IN_Config sai_config;

      sai_config.Protocol          = SAI_FREE_PROTOCOL;
      sai_config.AudioMode         = SAI_MODEMASTER_RX;
      sai_config.DataSize          = SAI_DATASIZE_16;
      sai_config.FirstBit          = SAI_FIRSTBIT_MSB;
      sai_config.ClockStrobing     = SAI_CLOCKSTROBING_RISINGEDGE;
      sai_config.Synchro           = SAI_ASYNCHRONOUS;
      sai_config.OutputDrive       = SAI_OUTPUTDRIVE_DISABLE;
      sai_config.NoDivider         = SAI_MASTERDIVIDER_ENABLE;
      sai_config.FIFOThreshold     = SAI_FIFOTHRESHOLD_EMPTY;
      sai_config.AudioFrequency    = SAI_AUDIO_FREQUENCY_MCKDIV;
      sai_config.Mckdiv            = SAI_DIVIDER(AudioInCtx[Instance].SampleRate);
      sai_config.SynchroExt        = SAI_SYNCEXT_DISABLE;
      sai_config.MonoStereoMode    = SAI_STEREOMODE;
      sai_config.CompandingMode    = SAI_NOCOMPANDING;
      sai_config.FrameLength       = 256;
      sai_config.ActiveFrameLength = 128;
      sai_config.FSDefinition      = SAI_FS_STARTFRAME;
      sai_config.FSPolarity        = SAI_FS_ACTIVE_LOW;
      sai_config.FSOffset          = SAI_FS_FIRSTBIT;
      sai_config.FirstBitOffset    = 0;
      sai_config.SlotSize          = SAI_SLOTSIZE_DATASIZE;
      sai_config.SlotNumber        = 16;
      sai_config.SlotActive        = 0x0000FFFF;

      /* Initialize SAI */
      __HAL_SAI_RESET_HANDLE_STATE(&hAudioInSai);

      /* PLL clock is set depending by the AudioFreq */
      if (MX_SAI_IN_ClockConfig(&hAudioInSai, PDM_Clock_Freq) != HAL_OK)
      {
        ret =  BSP_ERROR_CLOCK_FAILURE;
      }

      hAudioInSai.Instance = AUDIO_IN_SAI_INSTANCE;
      __HAL_SAI_DISABLE(&hAudioInSai);

      if (HAL_SAI_GetState(&hAudioInSai) == HAL_SAI_STATE_RESET)
      {
        SAI_MspInit(&hAudioInSai);
      }


      if (MX_SAI_IN_Init(&hAudioInSai, &sai_config) != HAL_OK)
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }


      if (HAL_SAI_Init(&hAudioInSai) != HAL_OK)
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }

      /* Enable SAI to generate clock used by audio driver */
      __HAL_SAI_ENABLE(&hAudioInSai);

#elif defined (USE_STM32F4XX_NUCLEO)
      MX_I2S_IN_Config i2s_config;

      i2s_config.DataFormat   = I2S_DATAFORMAT_16B;
      i2s_config.AudioFreq    = ((PDM_Clock_Freq * 1000U) / 32U);
      i2s_config.CPOL         = I2S_CPOL_HIGH;
      i2s_config.MCLKOutput   = I2S_MCLKOUTPUT_DISABLE;
      i2s_config.Mode         = I2S_MODE_MASTER_RX;
      i2s_config.Standard     = I2S_STANDARD_MSB;
      i2s_config.FullDuplexMode   = I2S_FULLDUPLEXMODE_DISABLE;
      i2s_config.ClockSource  = I2S_CLOCK_PLL;


      /* PLL clock is set depending by the AudioFreq */
      if (MX_I2S_IN_ClockConfig(&hAudioInI2s, PDM_Clock_Freq) != HAL_OK)
      {
        ret =  BSP_ERROR_CLOCK_FAILURE;
      }

      /* I2S Peripheral configuration */
      hAudioInI2s.Instance          = AUDIO_IN_I2S_INSTANCE;
      __HAL_I2S_DISABLE(&hAudioInI2s);
      I2S_MspInit(&hAudioInI2s);

      if (MX_I2S_IN_Init(&hAudioInI2s, &i2s_config) != HAL_OK)
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }
      if (HAL_I2S_Init(&hAudioInI2s) != HAL_OK)
      {
        ret =  BSP_ERROR_PERIPH_FAILURE;
      }


#endif
      if (IKS02A1_AUDIO_IN_PDMToPCM_Init(Instance, AudioInCtx[0].SampleRate, AudioInCtx[0].ChannelsNbr,
                                         AudioInCtx[0].ChannelsNbr) != BSP_ERROR_NONE)
      {
        ret =  BSP_ERROR_NO_INIT;
      }

    }
    else if (Instance == 1U)
    {
      ret =  BSP_ERROR_WRONG_PARAM;

    }
    else /* Instance = 2 */
    {
      ret =  BSP_ERROR_WRONG_PARAM;

    }

    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_STOP;
    /* Return BSP status */
  }
  return ret;
}

/**
  * @brief  Deinit the audio IN peripherals.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @retval BSP status
  */

__weak int32_t IKS02A1_AUDIO_IN_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= AUDIO_IN_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Instance == 0U)
    {

    }

    else /* (All other cases) */
    {
      ret =  BSP_ERROR_WRONG_PARAM;
    }
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_RESET;
  }
  /* Return BSP status */
  return ret;
}

#ifdef USE_STM32L4XX_NUCLEO





/**
  * @brief  Clock Config.
  * @param  hSai: SAI handle if required
  * @param  SampleRate: Audio frequency used to play the audio stream.
  * @note   This API is called by IKS02A1_AUDIO_IN_Init()
  *         Being __weak it can be overwritten by the application
  * @retval HAL_OK if no problem during execution, HAL_ERROR otherwise
  */
__weak HAL_StatusTypeDef MX_SAI_IN_ClockConfig(SAI_HandleTypeDef *hSai, uint32_t PDM_rate)
{
  UNUSED(hSai);

  HAL_StatusTypeDef ret = HAL_OK;
  /*SAI PLL Configuration*/
  RCC_PeriphCLKInitTypeDef rccclkinit;
  HAL_RCCEx_GetPeriphCLKConfig(&rccclkinit);

  if ((PDM_rate % 1280U) == 0U)
  {
    rccclkinit.PLLSAI1.PLLSAI1N = 29;
    rccclkinit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
  }
  else
  {
    rccclkinit.PLLSAI1.PLLSAI1N = 43;
    rccclkinit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  }

  rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  rccclkinit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  rccclkinit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  rccclkinit.PLLSAI1.PLLSAI1M = 8;
  rccclkinit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;

  if (HAL_RCCEx_PeriphCLKConfig(&rccclkinit) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}


__weak HAL_StatusTypeDef MX_SAI_IN_Init(SAI_HandleTypeDef *hSai, MX_SAI_IN_Config *MXConfig)
{
  static DMA_HandleTypeDef hdma_saiRx;
  HAL_StatusTypeDef ret = HAL_OK;

  hSai->Init.Protocol               = MXConfig->Protocol;
  hSai->Init.AudioMode              = MXConfig->AudioMode;
  hSai->Init.DataSize               = MXConfig->DataSize;
  hSai->Init.FirstBit               = MXConfig->FirstBit;
  hSai->Init.ClockStrobing          = MXConfig->ClockStrobing;
  hSai->Init.Synchro                = MXConfig->Synchro;
  hSai->Init.OutputDrive            = MXConfig->OutputDrive;
  hSai->Init.NoDivider              = MXConfig->NoDivider;
  hSai->Init.FIFOThreshold          = MXConfig->FIFOThreshold;
  hSai->Init.AudioFrequency         = MXConfig->AudioFrequency;
  hSai->Init.Mckdiv                 = MXConfig->Mckdiv;
  hSai->Init.SynchroExt             = MXConfig->SynchroExt;
  hSai->Init.MonoStereoMode         = MXConfig->MonoStereoMode;
  hSai->Init.CompandingMode         = MXConfig->CompandingMode;
  hSai->FrameInit.FrameLength       = MXConfig->FrameLength;
  hSai->FrameInit.ActiveFrameLength = MXConfig->ActiveFrameLength;
  hSai->FrameInit.FSDefinition      = MXConfig->FSDefinition;
  hSai->FrameInit.FSPolarity        = MXConfig->FSPolarity;
  hSai->FrameInit.FSOffset          = MXConfig->FSOffset;
  hSai->SlotInit.FirstBitOffset     = MXConfig->FirstBitOffset;
  hSai->SlotInit.SlotSize           = MXConfig->SlotSize;
  hSai->SlotInit.SlotNumber         = MXConfig->SlotNumber;
  hSai->SlotInit.SlotActive         = MXConfig->SlotActive;


  /* Enable the DMA clock */
  AUDIO_IN_SAI_DMAx_CLK_ENABLE();

  if (hSai->Instance == AUDIO_IN_SAI_INSTANCE)
  {
    /* Enable the DMA clock */
    /* DMA controller clock enable */

    AUDIO_IN_SAI_DMAx_CLK_ENABLE();

    /* Configure the hSaiDma handle parameters */
    hdma_saiRx.Instance                 = AUDIO_IN_SAI_DMAx_INSTANCE;
    hdma_saiRx.Init.Request             = AUDIO_IN_SAI_DMAx_REQUEST;
    hdma_saiRx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_saiRx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_saiRx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_saiRx.Init.PeriphDataAlignment = AUDIO_IN_SAI_DMAx_PERIPH_DATA_SIZE;
    hdma_saiRx.Init.MemDataAlignment    = AUDIO_IN_SAI_DMAx_MEM_DATA_SIZE;
    hdma_saiRx.Init.Mode                = DMA_CIRCULAR;
    hdma_saiRx.Init.Priority            = DMA_PRIORITY_HIGH;
    /* Associate the DMA handle */
    __HAL_LINKDMA(hSai, hdmarx, hdma_saiRx);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hdma_saiRx);

    /* Configure the DMA Stream */
    HAL_DMA_Init(&hdma_saiRx);

    /* I2S DMA IRQ Channel configuration */
    HAL_NVIC_SetPriority(AUDIO_IN_SAI_DMAx_IRQ, IKS02A1_AUDIO_IN_IT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(AUDIO_IN_SAI_DMAx_IRQ);

  }
  else
  {
    ret = HAL_ERROR;
  }

  return ret;
}

#else

/**
  * @brief  Clock Config.
  * @param  hi2s: I2S handle if required
  * @param  SampleRate: Audio frequency used to play the audio stream.
  * @note   This API is called by IKS02A1_AUDIO_IN_Init()
  *         Being __weak it can be overwritten by the application
  * @retval HAL_OK if no problem during execution, HAL_ERROR otherwise
  */
__weak HAL_StatusTypeDef MX_I2S_IN_ClockConfig(I2S_HandleTypeDef *hi2s, uint32_t PDM_rate)
{
  UNUSED(hi2s);

  HAL_StatusTypeDef ret = HAL_OK;
  /*I2S PLL Configuration*/
  RCC_PeriphCLKInitTypeDef rccclkinit;
  HAL_RCCEx_GetPeriphCLKConfig(&rccclkinit);

#if defined(STM32F446xx)
  rccclkinit.PLLI2S.PLLI2SQ = 2;
  rccclkinit.PLLI2SDivQ = 1;
#endif
  if ((PDM_rate % 1280U) == 0U)
  {
#if defined(STM32F411xE) || defined (STM32F446xx)
    rccclkinit.PLLI2S.PLLI2SM = 10;
    rccclkinit.PLLI2S.PLLI2SN = 96;
#else
    rccclkinit.PLLI2S.PLLI2SN = 192;
#endif
    rccclkinit.PLLI2S.PLLI2SR = 5;
  }
  else
  {
#if defined(STM32F411xE) || defined (STM32F446xx)

    rccclkinit.PLLI2S.PLLI2SM = 8;
#endif
    rccclkinit.PLLI2S.PLLI2SN = 258;
    rccclkinit.PLLI2S.PLLI2SR = 3;
  }

#if defined(STM32F446xx)
  rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB2;
#else
  rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_I2S;
#endif

  if (HAL_RCCEx_PeriphCLKConfig(&rccclkinit) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}


__weak HAL_StatusTypeDef MX_I2S_IN_Init(I2S_HandleTypeDef *hi2s, MX_I2S_IN_Config *MXConfig)
{
  static DMA_HandleTypeDef hdma_i2sRx;
  HAL_StatusTypeDef ret = HAL_OK;

  hi2s->Init.DataFormat = MXConfig->DataFormat;
  hi2s->Init.AudioFreq = MXConfig->AudioFreq;
  hi2s->Init.ClockSource = MXConfig->ClockSource;
  hi2s->Init.CPOL = MXConfig->CPOL;
  hi2s->Init.MCLKOutput = MXConfig->MCLKOutput;
  hi2s->Init.Mode = MXConfig->Mode;
  hi2s->Init.Standard = MXConfig->Standard;
#ifdef USE_STM32F4XX_NUCLEO
  hi2s->Init.FullDuplexMode = MXConfig->FullDuplexMode;
#endif

  /* Enable the DMA clock */
  AUDIO_IN_I2S_DMAx_CLK_ENABLE();

  if (hi2s->Instance == AUDIO_IN_I2S_INSTANCE)
  {
    /* Configure the hdma_i2sRx handle parameters */
    hdma_i2sRx.Init.Channel             = AUDIO_IN_I2S_DMAx_CHANNEL;
    hdma_i2sRx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_i2sRx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_i2sRx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_i2sRx.Init.PeriphDataAlignment = AUDIO_IN_I2S_DMAx_PERIPH_DATA_SIZE;
    hdma_i2sRx.Init.MemDataAlignment    = AUDIO_IN_I2S_DMAx_MEM_DATA_SIZE;
    hdma_i2sRx.Init.Mode                = DMA_CIRCULAR;
    hdma_i2sRx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_i2sRx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_i2sRx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_i2sRx.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma_i2sRx.Init.PeriphBurst         = DMA_MBURST_SINGLE;

    hdma_i2sRx.Instance = AUDIO_IN_I2S_DMAx_STREAM;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hi2s, hdmarx, hdma_i2sRx);

    /* Deinitialize the Stream for new transfer */
    if (HAL_DMA_DeInit(&hdma_i2sRx) != HAL_OK)
    {
      ret = HAL_ERROR;
    }

    /* Configure the DMA Stream */
    if (HAL_DMA_Init(&hdma_i2sRx) != HAL_OK)
    {
      ret = HAL_ERROR;
    }
  }
  else
  {
    ret = HAL_ERROR;
  }

  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_IN_I2S_DMAx_IRQ, IKS02A1_AUDIO_IN_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(AUDIO_IN_I2S_DMAx_IRQ);

  return ret;
}

#endif



/**
  * @brief  Initialize the PDM library.
  * @param Instance    AUDIO IN Instance
  * @param  AudioFreq  Audio sampling frequency
  * @param  ChnlNbrIn  Number of input audio channels in the PDM buffer
  * @param  ChnlNbrOut Number of desired output audio channels in the  resulting PCM buffer
  * @retval BSP status
  */
__weak int32_t IKS02A1_AUDIO_IN_PDMToPCM_Init(uint32_t Instance, uint32_t AudioFreq, uint32_t ChnlNbrIn,
                                              uint32_t ChnlNbrOut)
{
  int32_t ret =  BSP_ERROR_NONE;

  if (Instance != 0U)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {


    static int16_t aState_ARM[DECIMATOR_STATE_LENGTH];
    static int16_t aCoeffs[] = { -1406, 1634, -1943, 2386, -3080, 4325, -7223, 21690, 21690, -7223, 4325, -3080, 2386, -1943, 1634, -1406, };

    /* Enable CRC peripheral to unlock the PDM library */
    __HAL_RCC_CRC_CLK_ENABLE();

    volatile uint32_t error = 0;
    /* Init PDM filters */
    PDM_FilterHandler.bit_order  = PDM_FILTER_BIT_ORDER_LSB;
    if (ChnlNbrIn == 1)
    {
      PDM_FilterHandler.endianness = PDM_FILTER_ENDIANNESS_BE;
    }
    else
    {
      PDM_FilterHandler.endianness = PDM_FILTER_ENDIANNESS_LE;
    }
    PDM_FilterHandler.high_pass_tap = 2122358088;
    PDM_FilterHandler.out_ptr_channels = (uint16_t)ChnlNbrOut;
    PDM_FilterHandler.in_ptr_channels  = (uint16_t)ChnlNbrIn;

    /* PDM lib config phase */
    PDM_FilterConfig.output_samples_number = (uint16_t)((AudioFreq / 1000U) * N_MS_PER_INTERRUPT);
    PDM_FilterConfig.mic_gain = volume_level_to_dB(AUDIO_VOLUME_INPUT);

    switch (AudioInCtx[0].DecimationFactor)
    {
      case 16:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_16;
        break;
      case 24:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_24;
        break;
      case 32:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_32;
        break;
      case 48:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_48;
        break;
      case 64:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_64;
        break;
      case 80:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_80;
        break;
      case 128:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_128;
        break;
      case 160:
        PDM_FilterConfig.decimation_factor = PDM_FILTER_DEC_FACTOR_80;
        PDM_FilterConfig.output_samples_number *= 2U;
        PDM_FilterHandler.out_ptr_channels = 1;
        (void)arm_fir_decimate_init_q15(&ARM_Decimator_State, DECIMATOR_NUM_TAPS, DECIMATOR_FACTOR,
                                        aCoeffs, aState_ARM, DECIMATOR_BLOCK_SIZE);
        break;
      default:
        ret =  BSP_ERROR_WRONG_PARAM;
        break;
    }

    error = PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM_FilterHandler));
    if (error != 0U)
    {
      ret =  BSP_ERROR_NO_INIT;
    }

    error = PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM_FilterHandler, &PDM_FilterConfig);
    if (error != 0U)
    {
      ret =  BSP_ERROR_NO_INIT;
    }
  }
  return ret;
}



/**
  * @brief  Converts audio format from PDM to PCM.
  * @param  Instance  AUDIO IN Instance
  * @param  PDMBuf    Pointer to PDM buffer data
  * @param  PCMBuf    Pointer to PCM buffer data
  * @retval BSP status
  */
__weak int32_t IKS02A1_AUDIO_IN_PDMToPCM(uint32_t Instance, uint16_t *PDMBuf, uint16_t *PCMBuf)
{
  int32_t ret =  BSP_ERROR_NONE;

  if (Instance != 0U)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (AudioInCtx[Instance].SampleRate == 8000U)
    {
      uint16_t Decimate_Out[8 * N_MS_PER_INTERRUPT];
      uint32_t ii;
      uint16_t PDM_Filter_Out[16 * N_MS_PER_INTERRUPT];

      (void)PDM_Filter(((uint8_t *)(PDMBuf)), PDM_Filter_Out, &PDM_FilterHandler);
      (void)arm_fir_decimate_q15(&ARM_Decimator_State, (q15_t *) & (PDM_Filter_Out), (q15_t *) & (Decimate_Out),
                                 DECIMATOR_BLOCK_SIZE);
      for (ii = 0; ii < 8U * N_MS_PER_INTERRUPT; ii++)
      {
        PCMBuf[(ii * AudioInCtx[Instance].ChannelsNbr)] = Decimate_Out[ii];
      }
    }
    else
    {
      (void)PDM_Filter(((uint8_t *)(PDMBuf)), (uint16_t *)(PCMBuf), &PDM_FilterHandler);
    }

  }
  return ret;
}



/**
  * @brief  Start audio recording.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  pbuf     Main buffer pointer for the recorded data storing
  * @param  NbrOfBytes     Size of the record buffer. Parameter not used when Instance is 0
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_Record(uint32_t Instance, uint8_t *pBuf, uint32_t NbrOfBytes)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    AudioInCtx[Instance].pBuff = (uint16_t *)pBuf;

    UNUSED(NbrOfBytes);

#ifdef USE_STM32L4XX_NUCLEO

    if (HAL_SAI_Receive_DMA(&hAudioInSai, (uint8_t *)SAI_InternalBuffer,
                            (uint16_t)AudioInCtx[Instance].Size / 2U) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }

#elif defined(USE_STM32F4XX_NUCLEO)

    if (HAL_I2S_Receive_DMA(&hAudioInI2s, I2S_InternalBuffer, (uint16_t)AudioInCtx[Instance].Size / 2U) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }

#endif
    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_RECORDING;

  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Stop audio recording.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_Stop(uint32_t Instance)
{
  int32_t ret;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {


    ret = BSP_ERROR_NONE;

#ifdef USE_STM32L4XX_NUCLEO

    if (HAL_SAI_DMAStop(&hAudioInSai) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }

#elif defined(USE_STM32F4XX_NUCLEO)

    if (HAL_I2S_DMAStop(&hAudioInI2s) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }

#endif

    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_STOP;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Pause the audio file stream.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_Pause(uint32_t Instance)
{
  int32_t ret;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {


    ret = BSP_ERROR_NONE;

#ifdef USE_STM32L4XX_NUCLEO

    if (HAL_SAI_DMAPause(&hAudioInSai) != HAL_OK)
    {
      ret = BSP_ERROR_WRONG_PARAM;
    }
#elif defined(USE_STM32F4XX_NUCLEO)

    if (HAL_I2S_DMAPause(&hAudioInI2s) != HAL_OK)
    {
      ret = BSP_ERROR_WRONG_PARAM;
    }

#endif

    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_PAUSE;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Resume the audio file stream.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_Resume(uint32_t Instance)
{
  int32_t ret;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {


    ret = BSP_ERROR_NONE;

#ifdef USE_STM32L4XX_NUCLEO

    if (HAL_SAI_DMAResume(&hAudioInSai) != HAL_OK)
    {
      ret = BSP_ERROR_WRONG_PARAM;
    }

#elif defined(USE_STM32F4XX_NUCLEO)

    if (HAL_I2S_DMAResume(&hAudioInI2s) != HAL_OK)
    {
      ret = BSP_ERROR_WRONG_PARAM;
    }

#endif


    /* Update BSP AUDIO IN state */
    AudioInCtx[Instance].State = AUDIO_IN_STATE_RECORDING;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Starts audio recording.
  * @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
  * @param  pBuf      Main buffer pointer for the recorded data storing
  * @param  NbrOfBytes      Size of the recorded buffer. Parameter not used when Instance is 0
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_RecordChannels(uint32_t Instance, uint8_t **pBuf, uint32_t NbrOfBytes)
{
  /* Return BSP status */
  return BSP_ERROR_WRONG_PARAM;
}

/**
  * @brief  Stop audio recording.
  * @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
  * @param  Device    Digital input device to be stopped
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_StopChannels(uint32_t Instance, uint32_t Device)
{
  /* Return BSP status */
  return BSP_ERROR_WRONG_PARAM;
}

/**
  * @brief  Pause the audio file stream.
  * @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
  * @param  Device    Digital mic to be paused
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_PauseChannels(uint32_t Instance, uint32_t Device)
{
  /* Return BSP status */
  return BSP_ERROR_WRONG_PARAM;
}

/**
  * @brief  Resume the audio file stream
  * @param  Instance  AUDIO IN Instance. It can be 1(DFSDM used)
  * @param  Device    Digital mic to be resumed
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_ResumeChannels(uint32_t Instance, uint32_t Device)
{
  /* Return BSP status */
  return BSP_ERROR_WRONG_PARAM;
}

/**
  * @brief  Start audio recording.
  * @param  Instance  AUDIO IN SAI PDM Instance. It can be only 2
  * @param  pbuf     Main buffer pointer for the recorded data storing
  * @param  NbrOfBytes     Size of the record buffer. Parameter not used when Instance is 0
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_RecordPDM(uint32_t Instance, uint8_t *pBuf, uint32_t NbrOfBytes)
{
  /* Return BSP status */
  return BSP_ERROR_WRONG_PARAM;
}


/**
  * @brief  Set Audio In device
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  Device    The audio input device to be used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_SetDevice(uint32_t Instance, uint32_t Device)
{
  int32_t ret = BSP_ERROR_NONE;
  IKS02A1_AUDIO_Init_t audio_init;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (AudioInCtx[Instance].State == AUDIO_IN_STATE_STOP)
  {

    audio_init.Device = Device;
    audio_init.ChannelsNbr   = AudioInCtx[Instance].ChannelsNbr;
    audio_init.SampleRate    = AudioInCtx[Instance].SampleRate;
    audio_init.BitsPerSample = AudioInCtx[Instance].BitsPerSample;
    audio_init.Volume        = AudioInCtx[Instance].Volume;

    if (IKS02A1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  else
  {
    ret = BSP_ERROR_BUSY;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In device
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  Device    The audio input device used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_GetDevice(uint32_t Instance, uint32_t *Device)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio Input Device */
    *Device = AudioInCtx[Instance].Device;
  }
  return ret;
}

/**
  * @brief  Set Audio In frequency
  * @param  Instance     Audio IN instance
  * @param  SampleRate  Input frequency to be set
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_SetSampleRate(uint32_t Instance, uint32_t  SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;
  IKS02A1_AUDIO_Init_t audio_init;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (AudioInCtx[Instance].State == AUDIO_IN_STATE_STOP)
  {
    audio_init.Device        = AudioInCtx[Instance].Device;
    audio_init.ChannelsNbr   = AudioInCtx[Instance].ChannelsNbr;
    audio_init.SampleRate    = SampleRate;
    audio_init.BitsPerSample = AudioInCtx[Instance].BitsPerSample;
    audio_init.Volume        = AudioInCtx[Instance].Volume;
    if (IKS02A1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  else
  {
    ret = BSP_ERROR_BUSY;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In frequency
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  SampleRate  Audio Input frequency to be returned
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_GetSampleRate(uint32_t Instance, uint32_t *SampleRate)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio in frequency */
    *SampleRate = AudioInCtx[Instance].SampleRate;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Audio In Resolution
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  BitsPerSample  Input resolution to be set
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_SetBitsPerSample(uint32_t Instance, uint32_t BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;
  IKS02A1_AUDIO_Init_t audio_init;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (AudioInCtx[Instance].State == AUDIO_IN_STATE_STOP)
  {

    audio_init.Device        = AudioInCtx[Instance].Device;
    audio_init.ChannelsNbr   = AudioInCtx[Instance].ChannelsNbr;
    audio_init.SampleRate    = AudioInCtx[Instance].SampleRate;
    audio_init.BitsPerSample = BitsPerSample;
    audio_init.Volume        = AudioInCtx[Instance].Volume;
    if (IKS02A1_AUDIO_IN_Init(Instance, &audio_init) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  else
  {
    ret = BSP_ERROR_BUSY;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In Resolution
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  BitsPerSample  Input resolution to be returned
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_GetBitsPerSample(uint32_t Instance, uint32_t *BitsPerSample)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Return audio in resolution */
    *BitsPerSample = AudioInCtx[Instance].BitsPerSample;
  }
  return ret;
}

/**
  * @brief  Set Audio In Channel number
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  ChannelNbr  Channel number to be used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_SetChannelsNbr(uint32_t Instance, uint32_t ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance  != 0U) || (ChannelNbr > 1U))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Update AudioIn Context */
    AudioInCtx[Instance].ChannelsNbr = ChannelNbr;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In Channel number
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  ChannelNbr  Channel number to be used
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_GetChannelsNbr(uint32_t Instance, uint32_t *ChannelNbr)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Channel number to be returned */
    *ChannelNbr = AudioInCtx[Instance].ChannelsNbr;
  }
  return ret;
}

/**
  * @brief  Set the current audio in volume level.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  Volume    Volume level to be returnd
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_SetVolume(uint32_t Instance, uint32_t Volume)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    int16_t volume_dB = volume_level_to_dB(Volume);
    if (PDM_FilterConfig.mic_gain != volume_dB)
    {
      PDM_FilterConfig.mic_gain = volume_dB;
      (void)PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM_FilterHandler, &PDM_FilterConfig);
    }

    /* Update AudioIn Context */
    AudioInCtx[Instance].Volume = Volume;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the current audio in volume level.
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  Volume    Volume level to be returnd
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_GetVolume(uint32_t Instance, uint32_t *Volume)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Input Volume to be returned */
    *Volume = AudioInCtx[Instance].Volume;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get Audio In device
  * @param  Instance  AUDIO IN Instance. It can be 0 when I2S / SPI is used or 1 if DFSDM is used
  * @param  State     Audio Out state
  * @retval BSP status
  */
int32_t IKS02A1_AUDIO_IN_GetState(uint32_t Instance, uint32_t *State)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance  != 0U)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Input State to be returned */
    *State = AudioInCtx[Instance].State;
  }
  return ret;
}

#ifdef USE_STM32L4XX_NUCLEO


/**
  * @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into
byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is
written into the buffer that the user indicates when calling the IKS02A1_AUDIO_IN_Start(...) function.
  * @param hSai: SAI handle. Not used
  * @retval None
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hSai)
{
  UNUSED(hSai);
  uint32_t index;

  uint16_t *DataTempSAI = &SAI_InternalBuffer[AudioInCtx[0].Size / 4U] ;
  for (index = 0; index < (AudioInCtx[0].Size / 4U); index++)
  {
    AudioInCtx[0].pBuff[index] = (DataTempSAI[index]);
  }

  IKS02A1_AUDIO_IN_TransferComplete_CallBack(0);
}

/**
  * @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into
byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is
written into the buffer that the user indicates when calling the IKS02A1_AUDIO_IN_Start(...) function.
  * @param hSai: SAI handle. Not used
  * @retval None
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hSai)
{
  UNUSED(hSai);
  uint32_t index;

  uint16_t *DataTempSAI = SAI_InternalBuffer;
  for (index = 0; index < (AudioInCtx[0].Size / 4U); index++)
  {
    AudioInCtx[0].pBuff[index] = (DataTempSAI[index]);
  }

  IKS02A1_AUDIO_IN_HalfTransfer_CallBack(0);
}

#elif defined (USE_STM32F4XX_NUCLEO)
/**
  * @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into
byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is
written into the buffer that the user indicates when calling the IKS02A1_AUDIO_IN_Start(...) function.
  * @param hi2s: I2S handle
  * @retval None
  */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);
  uint32_t index;

  uint16_t *DataTempI2S = &I2S_InternalBuffer[AudioInCtx[0].Size / 4U] ;
  for (index = 0; index < (AudioInCtx[0].Size / 4U); index++)
  {
    AudioInCtx[0].pBuff[index] = (DataTempI2S[index]);
  }

  IKS02A1_AUDIO_IN_TransferComplete_CallBack(0);
}

/**
  * @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into
byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is
written into the buffer that the user indicates when calling the IKS02A1_AUDIO_IN_Start(...) function.
  * @param hi2s: I2S handle
  * @retval None
  */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);
  uint32_t index;

  uint16_t *DataTempI2S = I2S_InternalBuffer;
  for (index = 0; index < (AudioInCtx[0].Size / 4U); index++)
  {
    AudioInCtx[0].pBuff[index] = (DataTempI2S[index]);
  }

  IKS02A1_AUDIO_IN_HalfTransfer_CallBack(0);
}

#endif



/**
  * @brief  User callback when record buffer is filled.
  * @retval None
  */
__weak void IKS02A1_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @retval None
  */
__weak void IKS02A1_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
  * @brief  Audio IN Error callback function.
  * @retval None
  */
__weak void IKS02A1_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* This function is called when an Interrupt due to transfer error on or peripheral
  error occurs. */
}

/**
  * @}
  */

/** @defgroup IKS02A1_AUDIO_IN_Private_Functions IKS02A1_AUDIO_IN Private Functions
  * @{
  */

/*******************************************************************************
Static Functions
  *******************************************************************************/


#ifdef USE_STM32L4XX_NUCLEO

/**
  * @brief AUDIO IN SAI MSP Init
  * @param None
  * @retval None
  */
void SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  static DMA_HandleTypeDef hSaiDma;

  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the SAI peripheral clock */
  AUDIO_IN_SAI_CLK_ENABLE();

  /* Enable SAI GPIO clocks */
  AUDIO_IN_SAI_SCK_GPIO_CLK_ENABLE();
  AUDIO_IN_SAI_SD_GPIO_CLK_ENABLE();


  /* SAI pins configuration: SCK and SD pins ------------------------------*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Pin       = AUDIO_IN_SAI_SCK_PIN;
  GPIO_InitStruct.Alternate = AUDIO_IN_SAI_SCK_AF;
  HAL_GPIO_Init(AUDIO_IN_SAI_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = AUDIO_IN_SAI_SD_PIN ;
  GPIO_InitStruct.Alternate = AUDIO_IN_SAI_SD_AF;
  HAL_GPIO_Init(AUDIO_IN_SAI_SD_GPIO_PORT, &GPIO_InitStruct);

  /* Enable the DMA clock */
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();


  /* Configure the hSaiDma handle parameters */
  hSaiDma.Instance = DMA2_Channel2;
  hSaiDma.Init.Request             = DMA_REQUEST_1;
  hSaiDma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
  hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
  hSaiDma.Init.PeriphDataAlignment = AUDIO_IN_SAI_DMAx_PERIPH_DATA_SIZE;
  hSaiDma.Init.MemDataAlignment    = AUDIO_IN_SAI_DMAx_MEM_DATA_SIZE;
  hSaiDma.Init.Mode                = DMA_CIRCULAR;
  hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;
  /* Associate the DMA handle */
  __HAL_LINKDMA(hsai, hdmarx, hSaiDma);

  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&hSaiDma);

  /* Configure the DMA Stream */
  HAL_DMA_Init(&hSaiDma);


  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, IKS02A1_AUDIO_IN_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
}

#elif defined(USE_STM32F4XX_NUCLEO)

static void I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
  UNUSED(hi2s);
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the I2S2 peripheral clock */
  AUDIO_IN_I2S_CLK_ENABLE();

  /* Enable I2S GPIO clocks */
  AUDIO_IN_I2S_SCK_GPIO_CLK_ENABLE();
  AUDIO_IN_I2S_MOSI_GPIO_CLK_ENABLE();

  /* I2S2 pins configuration: SCK and MOSI pins ------------------------------*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;


  GPIO_InitStruct.Pin       = AUDIO_IN_I2S_SCK_PIN;
  GPIO_InitStruct.Alternate = AUDIO_IN_I2S_SCK_AF;
  HAL_GPIO_Init(AUDIO_IN_I2S_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = AUDIO_IN_I2S_MOSI_PIN ;
  GPIO_InitStruct.Alternate = AUDIO_IN_I2S_MOSI_AF;
  HAL_GPIO_Init(AUDIO_IN_I2S_MOSI_GPIO_PORT, &GPIO_InitStruct);

}

#endif

static int16_t volume_level_to_dB(uint16_t level)
{
  static int16_t VolumeGain[] =
  {
    -12, -12, -6, -3, 0, 2, 3, 5, 6, 7, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 15,
    16, 16, 17, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 21,
    22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25,
    25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29,
    29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 31
  };

  return VolumeGain[level];

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
