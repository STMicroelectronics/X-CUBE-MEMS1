/**
  ******************************************************************************
  * @file    main.c
  * @author  SRA - Central Labs
  * @version v5.0.0
  * @date    6-May-19
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed under Software License Agreement
  * SLA0077, (the "License"). You may not use this file except in compliance
  * with the License. You may obtain a copy of the License at:
  *
  *     www.st.com/content/st_com/en/search.html#q=SLA0077-t=keywords-page=1
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "cube_hal.h"
#include "main.h"
#include "com.h"
#include "DemoSerial.h"
#include "audio_application.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
volatile uint8_t DataLoggerActive = 0;
volatile uint8_t NewData = 0;
volatile float *FFTAverage;
volatile float SamplingFreq;
volatile uint32_t FFTLen;

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);


/** @addtogroup X_CUBE_MEMS1_Applications X-CUBE-MEMS1 Applications
* @{
*/

/** @addtogroup MicrophoneFFT MicrophoneFFT
* @{
*/

/** @defgroup MAIN
* @{
*/

/** @defgroup MAIN_Exported_Variables
* @{
*/

/**
* @}
*/

/** @defgroup MAIN_Private_Functions
* @{
*/

/**
* @}
*/

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
  TMsg msg_dat;
  TMsg msg_cmd;

  /* STM32F4xx HAL library initialization:
  - Configure the Flash prefetch, instruction and Data caches
  - Configure the Systick to generate an interrupt each 1 msec
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize GPIOs */
  MX_GPIO_Init();

  /* Initialize Communication Peripheral for data log */
  USARTConfig();

  /* Initialize microphone acquisition */
  Init_Acquisition_Peripherals(AUDIO_IN_SAMPLING_FREQUENCY, AUDIO_IN_CHANNELS, 0);

  /* Initialize FFT */
  Init_FFT_Module(AUDIO_IN_SAMPLING_FREQUENCY, 1024, 0.5f);

  /* Setup software interrupts to handle several priorities */
  SW_IRQ_Tasks_Init();

  BSP_LED_On(LED2);
  HAL_Delay(500);
  BSP_LED_Off(LED2);

  /* Start audio acquisition and processing */
  Start_Acquisition();

  while (1)
  {
    if (UART_ReceivedMSG((TMsg *)&msg_cmd) != 1)
    {
      if (msg_cmd.Data[0] == DEV_ADDR)
      {
        (void)HandleMSG((TMsg *)&msg_cmd);
      }
    }

    if (DataLoggerActive == 1U && NewData == 1U)
    {
      NewData = 0;

      /* Send data stream */
      INIT_STREAMING_HEADER(&msg_dat);
      memcpy(&msg_dat.Data[msg_dat.Len], (void *)&SamplingFreq, sizeof(SamplingFreq));
      msg_dat.Len += sizeof(SamplingFreq);
      memcpy(&msg_dat.Data[msg_dat.Len], (void *)FFTAverage, sizeof(float) * FFTLen / 2);
      msg_dat.Len += sizeof(float) * FFTLen / 2;
      UART_SendMsg(&msg_dat);
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  GPIO init function.
 * @param  None
 * @retval None
 * @details GPIOs initialized are User LED(PA5) and User Push Button(PC1)
 */
static void MX_GPIO_Init(void)
{
  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize push button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is executed in case of error occurrence
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
  for (;;)
  {
    BSP_LED_On(LED2);
    HAL_Delay(100);
    BSP_LED_Off(LED2);
    HAL_Delay(100);
  }
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
