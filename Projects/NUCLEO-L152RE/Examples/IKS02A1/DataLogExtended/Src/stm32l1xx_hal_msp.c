/**
  ******************************************************************************
  * @file           : stm32l1xx_hal_msp.c
  * @brief          : HAL MSP module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup X_NUCLEO_IKS02A1_Examples X_NUCLEO_IKS02A1 Examples
 * @{
 */

/** @addtogroup DATALOG_EXTENDED DATALOG EXTENDED
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
   
/* Private define ------------------------------------------------------------*/
   
/* Private macro -------------------------------------------------------------*/
   
/* Private variables ---------------------------------------------------------*/
   
/* Private function prototypes -----------------------------------------------*/
   
/* Private functions ---------------------------------------------------------*/
/**
 * @brief RTC MSP Initialization
 *        This function configures the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 *
 * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
 *        the RTC clock source; in this case the Backup domain will be reset in
 *        order to modify the RTC Clock source, as consequence RTC registers (including
 *        the backup registers) and RCC_BDCR register are set to their reset values.
 *
 * @retval None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Configue LSE/LSI as RTC clock source ###############################*/
  if (UseLSI == 0)
  {
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }
  else
  {
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*##-3- Configure the NVIC for RTC TimeStamp ###################################*/
  HAL_NVIC_SetPriority(TAMPER_STAMP_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(TAMPER_STAMP_IRQn);
}

/**
 * @brief RTC MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();

  /*##-2- Disable the NVIC for RTC TimeStamp #################################*/
  HAL_NVIC_DisableIRQ(TAMPER_STAMP_IRQn);
}

/**
 * @brief  Initializes I2C MSP.
 * @param  hi2c : I2C handler
 * @retval None
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  gpio_init;

  /* Enable I2C clock */
  __HAL_RCC_I2C1_CLK_ENABLE();

  /* Enable GPIO clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure I2C Tx, Rx as alternate function */
  gpio_init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = GPIO_AF4_I2C1;

  HAL_GPIO_Init(GPIOB, &gpio_init);
}

/**
 * @brief  DeInitializes I2C MSP.
 * @param  hi2c : I2C handler
 * @retval None
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  /* Disable I2C clock */
  __HAL_RCC_I2C1_CLK_DISABLE();

  /* DeInitialize peripheral GPIOs */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);
}

/**
 * @}
 */

/**
 * @}
 */
