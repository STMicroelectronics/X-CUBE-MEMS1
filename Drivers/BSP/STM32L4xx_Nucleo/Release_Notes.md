---
pagetitle: Release Notes for STM32L4xx_Nucleo Board Drivers
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for
# <mark>STM32L4xx_Nucleo Board Drivers</mark>
Copyright &copy; 2017 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# License

Licensed by ST under BSD 3-Clause license (the \"License\"). You may
not use this package except in compliance with the License. You may
obtain a copy of the License at:

[https://opensource.org/licenses/BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

This directory contains the board drivers to demonstrate the capabilities of the STM32L4xx_Nucleo Kit.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section21" checked aria-hidden="true">
<label for="collapse-section21" aria-hidden="true">V2.1.7 / 11-February-2021</label>
<div>

## Main Changes

- Minor update of Release Notes template
- Add License.md file for GitHub publication

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section20" aria-hidden="true">
<label for="collapse-section20" aria-hidden="true">V2.1.6 / 18-October-2019</label>
<div>

## Main Changes

- Minor update in release notes format

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section19" aria-hidden="true">
<label for="collapse-section19" aria-hidden="true">V2.1.5 / 03-April-2019</label>
<div>

## Main Changes

- stm32l4xx_nucleo.c, stm32l4xx_nucleo.h
  - Update license format

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section18" aria-hidden="true">
<label for="collapse-section18" aria-hidden="true">V2.1.4 / 27-July-2018</label>
<div>

## Main Changes

- Release notes update to new format


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section17" aria-hidden="true">
<label for="collapse-section17" aria-hidden="true">V2.1.3 / 21-June-2018</label>
<div>

## Main Changes

- stm32l4xx_nucleo.c, stm32l4xx_nucleo.h
  - Correct I/O definitions based on NUCLEO-L4xxRx-P schematics
  - Correct SD_IO_Init() and LCD_IO_Init() to set the correct port for LCD card CS pin

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section16" aria-hidden="true">
<label for="collapse-section16" aria-hidden="true">V2.1.2 / 09-March-2018</label>
<div>

## Main Changes

- stm32l4xx_nucleo.c
  - Set ADC peripheral clock selection only if ADCSEL bit of RCC CCIPR register is defined

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section155" aria-hidden="true">
<label for="collapse-section155" aria-hidden="true">V2.1.1 / 13-October-2017</label>
<div>

## Main Changes

- stm32l4xx_nucleo.c, stm32l4xx_nucleo.h
  - Documentation update: STM32L4xx_Nucleo_BSP_User_Manual.chm

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section15" aria-hidden="true">
<label for="collapse-section15" aria-hidden="true">V2.1.0 / 16-June-2017</label>
<div>

## Main Changes

- stm32l4xx_nucleo.c, stm32l4xx_nucleo.h
  - APIs addition to drive external SMPS (ADP5301ACBZ component)

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section12" aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V2.0.0 / 16-September-2015</label>
<div>

## Main Changes

- stm32l4xx_nucleo.c
  - SD over SPI interface rework for Adafruit Shield usage and performance
  - LED2 GPIO pull mode set to GPIO_NOPULL to improve power consumption
  - Apply new GPIO speed frequency define from HAL GPIO (GPIO_SPEED_FREQ_xxxx)
- **NOTE:** This release must be used with:
  - BSP Adafruit_Shield Component driver V2.0.0 or later
  - STM32L4xx HAL driver V1.1.0 or later

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V1.0.0 / 26-June-2015</label>
<div>

## Main Changes

- First official release of **STM32L4xx_Nucleo** board drivers for STM32Cube L4 FW package

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
