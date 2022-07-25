---
pagetitle: Release Notes for STM32L0xx CMSIS
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> <mark>STM32L0xx CMSIS</mark>
Copyright &copy; 2009-2018 ARM Limited - STMicroelectronics\
    
[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

Licensed by ST under Apache-2.0 license (the \"License\"). You may
not use this package except in compliance with the License. You may
obtain a copy of the License at:

[Apache License v2.0](../../../LICENSE.txt)

# Purpose

This driver provides the CMSIS device for the stm32l1xx products. This covers:

  - STM32L010xx devices
  - STM32L011xx and STM32L021xx devices
  - STM32L031xx and STM32L041xx devices
  - STM32L051xx, STM32L052xx, STM32L053xx, STM32L062xx and STM32L063xx devices
  - STM32L071xx, STM32L072xx, STM32L073xx, STM32L082xx and STM32L083xx devices

This driver is composed of the descriptions of the registers under “Include” directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32l0xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR©, KEIL© and SW4STM32©.
- Linker files are provided as example for IAR©, KEIL© and SW4STM32©.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section15" aria-hidden="true">
<label for="collapse-section15" aria-hidden="true">V1.9.2 / 16-July-2021</label>
<div>

## Main Changes

### Maintenance release

## Contents

- Add new atomic register access macros in stm32l0xx.h file.
- Add LSI maximum startup time datasheet value: LSI_STARTUP_TIME.

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section14" aria-hidden="true">
<label for="collapse-section14" aria-hidden="true">V1.9.1 / 28-October-2020</label>
<div>

## Main Changes

### Maintenance release

## Contents

- system_stm32l0xx.c
  - SystemInit(): update to don’t reset RCC registers to its reset values.
  - Protect Vector table modification following SRAM or FLASH preprocessor directive by a generic preprocessor directive: USER_VECT_TAB_ADDRESS.
- All header files
  - Remove unused IS_TIM_SYNCHRO_INSTANCE() assert macro.
- Remove stm32l061xx.h header file.
- Add License.md and Readme.md files required for GitHub publication

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section13" aria-hidden="true">
<label for="collapse-section13" aria-hidden="true">V1.9.0 / 26-October-2018</label>
<div>

## Main Changes

### Maintenance release

### First release supporting L0 Value Lines

## Contents

- Add the support of STM32L010xx devices
  - Add stm32l010xb.h, stm32l010x8.h, stm32l010x6.h and stm32l010x4.h device description files
  - Add startup_stm32l010xb.s, startup_stm32l010x8.s, startup_stm32l010x6.s and startup_stm32l010x4.s startup files for EWARM, MDK-ARM and SW4STM32 toolchains
  - Add EWARM associated linker files for execution from internal RAM or internal FLASH


- stm32l0xx.h
  - Add the following device defines:
    - "#define STM32L010xB" for all STM32L010xB devices
    - "#define STM32L010x8" for all STM32L010x8 devices
    - "#define STM32L010x6" for all STM32L010x6 devices
    - "#define STM32L010x4" for all STM32L010x4 devices
  - Align ErrorStatus typedef to common error handling.


- All stm32l0xxxx.h device description files.h
  - [MISRAC2012-Rule-10.6] Use 'UL' postfix for _Msk definitions and memory/peripheral base addresses
  - Correct comments in the bit definition of RCC_AHBRST, RCC_APB2RSTR and RCC_APB1RSTR registers.
  - Rename RTC_CR_BCK bit to RTC_CR_BKP to be aligned with reference manual.
  - Add missing definition of IS_TSC_ALL_INSTANCE after TSC driver update.
  - Add back the bit definition of SYSCFG_CFGR3_EN_VREFINT in SYSCFG_CFGR3 register.
  - Rename GPIO_AFRL_AFRLx and GPIO_AFRL_AFRHx bit definitions (from GPIO_AFRL/AFRH registers) to GPIO_AFRL_AFSELx.
  - Align IS_TIM_XXX_INSTANCE definitions with other series.
  - Remove cast (uint8_t) in CRC_IDR_IDR definition.
  - Add missing definition of IS_PCD_ALL_INSTANCE macro after USB driver update.
  - Add definition of IS_UART_DRIVER_ENABLE_INSTANCE macro after UART driver update.
  - Add compatibility definition of USART_ICR_NECF / USART_ICR_NCF with others series.
  - Update IS_UART_INSTANCE macro definition.
  - Add definition of IS_LPTIM_ENCODER_INTERFACE_INSTANCE macro after LPTIM driver update.
  - Move definition of FLASH_BANK2_BASE start address to stm32l0xx_hal_flash.h to be dependant on Memory Size register.
  - Update interrupt definition to use DMA1_Channel4_5_IRQn  for STM32L011xx and STM32L021xx devices.
  - Correct PWR_WAKEUP_PIN definitions for L011xx and L021xx devices.


- system_stm32l0xx.c
  - Update file to correct comments for VECT_TAB_OFFSET definition.
  - Update default MSI_VALUE reset value set in SystemCoreClock.
  - Update SystemCoreClockUpdate() function to check HSI16DIVF for HSI divided by 4.

- startup_stm32l0xxxx.s
  - Update startup files to use DMA1_Channel4_5_IRQn/IRQHandler for STM32L011xx and STM32L01xx devices.


</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section12" aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V1.8.0 / 12-January-2018</label>
<div>

## Main Changes

### Internal release


</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section11" aria-hidden="true">
<label for="collapse-section11" aria-hidden="true">V1.7.2 / 25-August-2017</label>
<div>

## Main Changes

### Maintenance release

## Contents

- Removed DATE and VERSION fields from header files.


</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section10" aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">V1.7.1 / 25-November-2016</label>
<div>

## Main Changes

### Maintenance release

## Contents

- Updated IS_COMP_COMMON_INSTANCE() macro.
- Corrected ADC_CFGR2_TOVS bit and mask definitions.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V1.7.0 / 31-May-2016</label>
<div>

## Main Changes

### Maintenance release

## Contents

- Added Pos and Msk macros missing within the CMSIS stm32l083xx.h file.
- Added LCD_CR_BUFEN bit definition in LCD CR register for stm32l053xx, stm32l063xx, stm32l073xx, stm32l083xx devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V1.6.0 / 15-April-2016</label>
<div>

## Main Changes

### Maintenance release

## Contents

- Add Pos and Msk macros within the CMSIS files. 
  - For example, on the previous CMSIS version (V1.5.0), the constant ADC_IER_EOCALIE was defined as follow :
    - #define ADC_IER_EOCALIE                     ((uint32_t)0x00000800U)
  - On this new CMSIS version (V1.6.0), the constant ADC_IER_EOCALIE is now defined as follow :
    - #define ADC_IER_EOCALIE_Pos               (11U)
    - #define ADC_IER_EOCALIE_Msk              (0x1U << ADC_IER_EOCALIE_Pos) 
    - #define ADC_IER_EOCALIE                      ADC_IER_EOCALIE_Msk
  - The same rule applies on all the other constants present inside the CMSIS files.
        
- MISRA C 2004 rule 10.6 compliancy. (A 'U' suffix shall be applied to all constants of unisgned type).

- Several SYSCFG definition changes :
 - SYSCFG_CFGR3_EN_VREFINT enable bit suppressed. (no more needed and must not be used). 
 - SYSCFG_CFGR3_EN_BGAP define suppressed.
 - SYSCFG_CFGR3_REF_HSI48_RDYF, SYSCFG_CFGR3_SENSOR_ADC_RDYF, SYSCFG_CFGR3_VREFINT_ADC_RDYF, SYSCFG_CFGR3_VREFINT_COMP_RDYF flags suppressed, SYSCFG_CFGR3_VREFINT_RDYF must be used instead (this flag is a combination of the 4 suppressed flags).
 - Added SYSCFG_CFGR3_REF_RC48MHz_RDYF, SYSCFG_CFGR3_REF_HSI48_RDYF, SYSCFG_VREFINT_ADC_RDYF, SYSCFG_CFGR3_SENSOR_ADC_RDYF, SYSCFG_CFGR3_VREFINT_ADC_RDYF and SYSCFG_CFGR3_VREFINT_COMP_RDYF defines.

- Aligned register namings with the different L0 Reference Manual (For STM32L0x1 : RM0377 Rev5, for STM32L0x2 : RM0367 Rev2, for STM32L0x3 : RM0367 Rev4).
  The list of the modification is listed hereafter :
  - Introduced new masks in EXTI bit definitions in order to simplify LL source code. 
  - Renamed RCC_CFGR_MCO_x into RCC_CFGR_MCOSEL_x
  - Added FLASHSIZE_BASE, UID_BASE and SRAM_SIZE_MAX defines.
  - Renamed macro IS_DMA_ALL_INSTANCE() to IS_DMA_STREAM_ALL_INSTANCE().
  - Added new macros: IS_I2C_WAKEUP_FROMSTOP_INSTANCE(), IS_ADC_COMMON_INSTANCE() and IS_LPUART_INSTANCE().
  - Added new defines PWR_PVD_SUPPORT to handle the PVD feature.
  - Corrected the value of FLASH_END for STM32L011xx or STM32L021xx devices.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V1.5.0 / 8-January-2016</label>
<div>

## Main Changes

### Maintenance release

## Contents

- MISRA C 2004 rule 5.1 and rule 10.6 compliancy.
- Several renaming in order to be aligned with the Reference Manual.The list of the modification is listed hereafter :
  - Adding of a new COMP_Common_TypeDef structure.
  - Removal of the RCR field inside the TIM_TypeDef structure.
  - Adding of a new define COMP12_COMMON
  - Adding of a new define DAC1 (same as DAC)
  - Adding of a new define ADC1_COMMON
  - Adding of a new define ADC_CHSELR_CHSEL
  - Adding of a new define COMP_CSR_WINMODE
  - Adding of a new define DAC_CHANNEL2_SUPPORT
  - Renaming of EXTI_RTSR_TRx into EXTI_RTSR_RTx with x = {0,..22}
  - Renaming of EXTI_FTSR_TRx into EXTI_FTSR_FTx with x = {0,..22}
  - Renaming of EXTI_SWIER_SWIERx into EXTI_SWIER_SWIx with x = {0,..22}
  - Renaming of EXTI_PR_PRx into EXTI_PR_PIFx with x = {0,..22}
  - Renaming of RCC_IOPRSTR_GPIOxRST into RCC_IOPRSTR_IOPxRST with x = {A,B,C,D,E,H}
  - Add a new define RCC_AHBRSTR_DMA1RST
  - Add a new define RCC_APB2RSTR_ADC1RST
  - Add a new define RCC_APB2RSTR_DBGMCURST
  - Renaming of RCC_IOPENR_GPIOxEN into RCC_IOPENR_IOPxEN with x = {A,B,C,D,E,H}
  - Add a new define RCC_AHBENR_DMA1EN
  - Rename RCC_APB2ENR_MIFIEN into RCC_APB2ENR_FWEN
  - Rename RCC_APB2ENR_ADC1EN into RCC_APB2ENR_ADCEN
  - Rename RCC_APB2ENR_DBGMCUEN into RCC_APB2ENR_DBGEN
  - Rename RCC_IOPSMENR_GPIOxSMEN into RCC_IOPSMENR_IOPxSMEN with x = {A,B,C,D,E,H}
  - Add a new define RCC_AHBSMENR_DMA1SMEN
  - Rename RCC_APB2SMENR_ADC1SMEN into RCC_APB2SMENR_ADCSMEN
  - Rename RCC_APB2SMENR_DBGMCUSMEN into RCC_APB2SMENR_DBGSMEN
  - Add new defines TIM_TIM2_REMAP_HSI_SUPPORT and TIM_TIM2_REMAP_HSI48_SUPPORT
  - Remove the following defines :  TIM_CR2_CCPC, TIM_CR2_CCUS,  TIM_CR2_OIS1, TIM_CR2_OIS1N, TIM_CR2_OIS2,TIM_CR2_OIS2N, TIM_CR2_OIS3, TIM_CR2_OIS3N, TIM_CR2_OIS4
  - Remove TIM_SR_COMIF and TIM_SR_BIF
  - Remove TIM_EGR_COMG and TIM_EGR_BG
  - Remove TIM_CCER_CC1NE, TIM_CCER_CC2NE and TIM_CCER_CC3NE
  - Remove TIM_RCR_REP
  - Rename USART_ISR_LBD into USART_ISR_LBDF
  - Rename WWDG_CR_Tx into WWDG_CR_T_x with x = {0,..6}
  - Rename WWDG_CFR_WDGTBx into WWDG_CFR_WDGTB_x with x = {0,1}
  - Add several macros to check Timer instances (IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(), IS_TIM_CLOCK_DIVISION_INSTANCE(), ...)


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V1.4.0 / 16-October-2015</label>
<div>

## Main Changes

### Maintenance release

- Update all the files to support STM32L011xx and STM32L021xx.

## Contents

- Remove the Debug Monitor handler from the startup files (not supported on L0).
- Renamings and usage of some aliases in order to be compliant with the RefManuals.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V1.3.0 / 9-Sept-2015</label>
<div>

## Main Changes

### Maintenance release

- Update all the files to support **STM32L031xx** and **STM32L041xx**.

## Contents

- Several renamings in order to be compliant with the specifications.
- Adding of new bit definitions (COMP_CSR_COMP2LPTIM1IN1, SYSCFG_CFGR1_UFB, I2C_OAR2_x, LCD_CR_MUX_SEG, RTC_BKP_NUMBER)
- Update of several registers and structures (CRC_TypeDef, TIM_TypeDef)


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V1.2.0 / 6-February-2015</label>
<div>

## Main Changes

### Maintenance release

- Added the set of CMSIS files for the **STM32L07xx** and **STM32L08xx** family

## Contents

- Add IAR set of files STM32L073xx - STM32L072xx - STM32L071xx - STM32L083xx - STM32L082xx - STM32L081xx
- Added MDK-ARM startup files for L071xx, L072xx, L073xx, L081xx, L082xx, L083xx
- Added Atollic startup files for L071xx, L072xx, L073xx, L081xx, L082xx, L083xx


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.1.0 / 18-June-2014</label>
<div>

## Main Changes

### Maintenance release

## Contents

- **Header files**
  - Add defines for memories base and end addresses:
    - FLASH_END, DATA_EEPROM_BASE and DATA_EEPROM_END (instead of  having them in stm32l0xx_hal_flash.h file)
  - Peripheral register structures definition is aligned on 32 bit
  - FLASH_SR_FWWER value fixed to 0x00020000 instead of 0x00010000
  - Add missing EXTI register bits definition
    - IMR and EMR registers: bits 18, 20, 26, 28, 29
    - FTSR, RTSR, PR and SWIER registers: bits 20, 21, 22
  - Update some bits definition to be in line with latest version of the Reference Manual
    - Rename FLASH_SR_ENHV into FLASH_SR_ENDHV 
    - Rename SYSCFG_VREFINT_ADC_RDYF into SYSCFG_CFGR3_VREFINT_ADC_RDYF
    - Rename ADC_SMPR_SMPR_xxx into ADC_SMPR_SMP_xxx
    - Rename SYSCFG_CFGR3_VREFINT_ADC_RDYF into SYSCFG_VREFINT_ADC_RDYF
      Note: aliases has been added to keep compatibility with previous version
- **system_stm32l0xx.c**
  - Use "__IO const" instead of "__I", to avoid any compilation issue when __cplusplus switch is defined

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.0.1 / 24-April-2014</label>
<div>

## Main Changes

### First official release

## Contents

  - Update gcc startup files


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 22-April-2014</label>
<div>

## Main Changes

### First official release


</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
