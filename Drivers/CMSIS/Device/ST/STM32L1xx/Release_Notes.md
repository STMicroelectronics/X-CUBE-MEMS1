---
pagetitle: Release Notes for STM32L1xx CMSIS
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> <mark>STM32L1xx CMSIS</mark>
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

  - STM32L100xx devices
  - STM32L151xx devices
  - STM32L152xx devices
  - STM32L162xx devices

This driver is composed of the descriptions of the registers under “Include” directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.

- Templates/system_stm32l1xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for IAR©, KEIL© and SW4STM32©.
- Linker files are provided as example for IAR©, KEIL© and SW4STM32©.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section13" checked aria-hidden="true">
<label for="collapse-section13" aria-hidden="true">V2.3.2 / 21-May-2021</label>
<div>

## Main Changes

### Maintenance release


## Contents

  - Improve GCC startup files robustness.
  - Add LSI maximum startup time datasheet value: LSI_STARTUP_TIME.
  - Add atomic register access macros.
  - Protect Vector table modification following SRAM or FLASH preprocessor directive by a generic preprocessor directive : USER_VECT_TAB_ADDRESS.
  - Remove bit definition related to PF and PG ports from unsupported devices (L1xxxB devices).
  - Fix SYSCFG_EXTICR1_EXTI3_PF and SYSCFG_EXTICR1_EXTI3_PG defines values.
  - Update the hal_flash.h file to correctly support the FLASH_SIZE of cat.2 devices.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section12" aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V2.3.1 / 24-June-2020</label>
<div>

## Main Changes

### Maintenance release


## Contents

  - All header files
    - Remove NVIC CMSIS bits definitions to avoid duplication with CMSIS Core V5.x
    - Remove GPIOx BRR register from GPIO structure and bit definitions when not supported
    - Add ADC VREFINT/TEMPSENSOR addresses definitions in line with products datasheets:
      - VREFINT_CAL_ADDR_CMSIS
      - TEMPSENSOR_CAL1_ADDR_CMSIS
      - TEMPSENSOR_CAL2_ADDR_CMSIS
    - Remove unused IS_TIM_SYNCHRO_INSTANCE() assert macro
    - Update IS_TIM_MASTER_INSTANCE with all supported instances.
  - system_stm32l1xx.c file
    - Update SystemInit() API to don't reset RCC registers to its reset values


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section11" aria-hidden="true">
<label for="collapse-section11" aria-hidden="true">V2.3.0 / 05-April-2019</label>
<div>

## Main Changes

### Maintenance release


## Contents


  - stm32l0xx.h
    - Align ErrorStatus typedef to common error handling

  - startup_stm32l1xxxxx.s
    - Update header of startup files to remove build error with ARM compiler.

  - Updated Update CMSIS Devices compliancy with MISRA C 2012 rules
    - Rule-10.6: Use 'UL' postfix for _Msk definitions and memory/peripheral base addresses
  - Update CMSIS device description files according to latest version of HAL/LL drivers
    - Correct definitions for USB_COUNT3_TX_0 and USB_COUNT3_TX_1 registers
    - Add IS_PCD_ALL_INSTANCE definition
    - Add IS_TIM_SYNCHRO_INSTANCE macro definition
    - Update macros definition for TIM instances



</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section10" aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">V2.2.3 / 12-January-2018</label>
<div>

## Main Changes

### Patch release

## Contents

  - Corrected devices supporting RI_HYSCR3, RI_HYSCR4, RI_ASMRx, RI_CMRx, RI_CICRx registers in CMSIS files.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V2.2.2 / 25-August-2017</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Removed DATE and VERSION fields from header files.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V2.2.1 / 21-April-2017</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Update CMSIS Devices compliancy with MISRA C 2004 rules:
    - MISRA C 2004 rule 10.6 ('U' suffix applied to all constants of 'unsigned' type)
    - Update system_stm32l1xx.h/.c files.
  - Align Bit definitions for SCB_CFSR register to be compliant with CMSIS Core V4.x and V5.x.
  - Rename RTC_CR_BCK bits in RTC_CR register to RTC_CR_BKP, to be aligned with others series.
  - Rename GPIO_AFRL_AFRLx and GPIO_AFRL_AFRHx bit definitions (from GPIO_AFRL/AFRH registers) to GPIO_AFRL_AFSELx.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V2.2.0 / 01-July-2016</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Add macros _Pos and _Msk for each constants.
    - For instance:
      - #define ADC_LTR_LT     ((uint32_t)0x00000FFFU)
    - Is now provided as:
      - #define ADC_LTR_LT_Pos (0U) 
      - #define ADC_LTR_LT_Msk (0xFFFU << ADC_LTR_LT_Pos)
      - #define ADC_LTR_LT     ADC_LTR_LT_Msk
  - IS_I2S_ALL_INSTANCE is now SPI2 and SPI3 instead of SPI1 and SPI2.
  - Update the TIM_ARR_ARR and TIM_CNT_CNT as they support the 32 bit counter feature.
  - Add macro IS_TIM_ETR_INSTANCE.
  - Add RI_ASCR2_GR6_x with x = 1,2,3,4 if applicable.
  - Add macro IS_SMBUS_ALL_INSTANCE.
  - Set default value for SystemCoreClock to 2097000 instead of 32000000.
  - Correct the presence of TIM9 for some devices inside various TIMER macros.



</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V2.1.3 / 04-March-2016</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Add HardFault_IRQn.
  - Add BKP5R to BKP19R for RTC_TypeDef for stm32l151xba.
  - Align bits naming on all stm32 families (ex: WWDG_CFR_WDGTB0 -> WWDG_CFR_WDGTB_0).
  - Rename RCC_CFGR_MCO_DIVx to RCC_CFGR_MCOPRE_DIVx
  - Align Bits naming on all stm32 families (ex: EXTI_IMR_MR0 --> EXTI_IMR_IM0)
  - Update .icf file to correct empty linker ROM Start/End menu, under IAR, for stm32l151xdx, stm32l152xdx and stm32l162xdx.
  - Rename RCC_CFGR_MCO_x to RCC_CFGR_MCOSEL_x to be aligned with Reference Manual.
  - Update CMSIS drivers to apply MISRA C 2004 rule 10.6. (Use U postfix)
  - Add defines FLASHSIZE_BASE and UID_BASE
  - ADC common instance standardization (new define ADC1_COMMON)
  - Remove bit GPIO_BRR_BR_x from Cat1 to Cat2 devices
  - Literals "ADC_SMPR1_SMP27, ADC_SMPR1_SMP28, ADC_SMPR1_SMP29" are available on Cat4, Cat5 only.
  - Add DMA_CNDTR_NDT, DMA_CPAR_PA and DMA_CMAR_MA definitions present on other stm32 families.
  - Add defines to trig feature inside source code based on CMSIS content:
    - RTC_TAMPER1_SUPPORT
    - RTC_TAMPER2_SUPPORT
    - RTC_TAMPER3_SUPPORT
    - RTC_BACKUP_SUPPORT
    - RTC_WAKEUP_SUPPORT
    - RTC_SMOOTHCALIB_SUPPORT
    - RTC_SUBSECOND_SUPPORT. 
    - PWR_PVD_SUPPORT
  - Set CMSIS variables PLLMulTable, AHBPrescTable and APBPrescTable as external in system_stm32l1xx.h.
  - Bit 23 of all EXTI registers (IMR, EMR, ...) is not supported on Cat 1 & 2.
  - Correct a wrong values for RI_ASCR1_CH_27 to RI_ASCR1_CH_30
  - Add defines for LCD capacitance
  - Suppress SPI1 from I2S_ALL_INSTANCE

               
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V2.1.2 / 09-October-2015</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Removing the __IO attribute for PLLMulTable and AHBPrescTable. This was leading to issue during C++ initialisation.
  - IDR field of CRC_TypeDef changed from uint32_t to uint8_t to comply with register structure.
  - Added TIM10 and TIM11 to IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE).
  - Renaming USB_CNTR_LP_MODE to USB_CNTR_LPMODE for naming consistancy with other bits.
  - Renaming USB_ISTR_PMAOVRM to USB_ISTR_PMAOVR to use the name of the bit in this register.


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V2.1.1 / 31-March-2015</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Ensure compliancy w/ C++
  - Minor update on Timer assert.
  - Adding IS_AES_ALL_INSTANCE macro for product with AES.




</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V2.1.0 / 06-February-2015</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Add CMSIS files for new STM32L1 e**X**tended Devices : **STM32L151xDX, STM32L152xDX and STM32L162xDX**


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V2.0.0 / 05-September-2014</label>
<div>

## Main Changes

### Maintenance release

## Contents

  - Update based on STM32Cube specification

## Other compatibilities

  - This version has to be used only with **STM32CubeL1** based development

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
