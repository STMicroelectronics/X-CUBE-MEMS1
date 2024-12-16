/**
  ******************************************************************************
  * @file           : cube_hal_f4.c
  * @brief          : Specific Cube settings for STM32F4 Nucleo boards
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "com.h"

/** @addtogroup X_NUCLEO_IKS4A1_Examples X_NUCLEO_IKS4A1 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  System Clock Configuration
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = 16;
  RCC_OscInitStruct.PLL.PLLN            = 336;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ            = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
                                      RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
 * @brief  Get the DMA Stream pending flags
 * @param  handle_dma DMA handle
 * @retval The state of FLAG (SET or RESET)
 */
uint32_t Get_DMA_Flag_Status(DMA_HandleTypeDef *handle_dma)
{
  return (__HAL_DMA_GET_FLAG(handle_dma, __HAL_DMA_GET_FE_FLAG_INDEX(handle_dma)));
}

/**
 * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer
 * @param  handle_dma DMA handle
 * @retval The number of remaining data units in the current DMA Stream transfer
 */
uint32_t Get_DMA_Counter(DMA_HandleTypeDef *handle_dma)
{
  return (__HAL_DMA_GET_COUNTER(handle_dma));
}

/**
 * @brief  Configure the DMA handler for reception process
 * @param  handle_dma DMA handle
 * @retval None
 */
void Config_DMA_RxHandler(DMA_HandleTypeDef *handle_dma)
{
  /* USART2 DMA Init */
  /* USART2_RX Init */
  handle_dma->Instance                 = DMA1_Stream5;
  handle_dma->Init.Channel             = DMA_CHANNEL_4;
  handle_dma->Init.Direction           = DMA_PERIPH_TO_MEMORY;
  handle_dma->Init.PeriphInc           = DMA_PINC_DISABLE;
  handle_dma->Init.MemInc              = DMA_MINC_ENABLE;
  handle_dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  handle_dma->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  handle_dma->Init.Mode                = DMA_CIRCULAR;
  handle_dma->Init.Priority            = DMA_PRIORITY_MEDIUM;
  handle_dma->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
}

/**
 * @brief  Configure the DMA handler for transmission process
 * @param  handle_dma DMA handle
 * @retval None
 */
void Config_DMA_TxHandler(DMA_HandleTypeDef *handle_dma)
{
  /* USART2 DMA Init */
  /* USART2_TX Init */
  handle_dma->Instance                 = DMA1_Stream6;
  handle_dma->Init.Channel             = DMA_CHANNEL_4;
  handle_dma->Init.Direction           = DMA_MEMORY_TO_PERIPH;
  handle_dma->Init.PeriphInc           = DMA_PINC_DISABLE;
  handle_dma->Init.MemInc              = DMA_MINC_ENABLE;
  handle_dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  handle_dma->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  handle_dma->Init.Mode                = DMA_NORMAL;
  handle_dma->Init.Priority            = DMA_PRIORITY_MEDIUM;
  handle_dma->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
}

/**
 * @}
 */

/**
 * @}
 */
