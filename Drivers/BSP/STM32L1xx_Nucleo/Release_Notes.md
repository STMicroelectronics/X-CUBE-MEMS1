---
pagetitle: Release Notes for STM32L1xx_Nucleo Board Drivers
lang: en
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

::: {.card .fluid}
::: {.sectione .dark}
<center>
# <small>Release Notes for</small> <mark>STM32L1xx_Nucleo Board Drivers</mark>
Copyright &copy; 2017 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>
:::
:::

# License

Licensed by ST under BSD 3-Clause license (the \"License\"). You may
not use this package except in compliance with the License. You may
obtain a copy of the License at:

[https://opensource.org/licenses/BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

# Purpose

This directory contains the board drivers to demonstrate the capabilities of the STM32L1xx Nucleo Kit.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section7" checked aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V1.1.3 / 05-April-2019</label>
<div>

## Main Changes

  - Update format of Release_Notes.html file.
  - Update CHM User Manual for BSP/STM32L1xx_Nucleo.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V1.1.2 / 21-April-2017</label>
<div>

## Main Changes

  - Remove DATE and VERSION fields from header files.
  - Update CHM User Manual for BSP/STM32L1xx_Nucleo.

</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V1.1.1 / 21-April-2017</label>
<div>

## Main Changes

  - Update CHM User Manual for BSP/STM32L1xx_Nucleo.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V1.1.0 / 04-March-2016</label>
<div>

## Main Changes

  - Rewrite driver to support 4 Gb micro sd card.
  - Add functions:
    - BSP_LED_DeInit
    - BSP_PB_DeInit
    - BSP_JOY_DeInit

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.0.2 / 09-October-2015</label>
<div>

## Main Changes

  - Added aliases to use either BUTTON_USER or BUTTON_KEY.
  - The polling of ADC is improved inside BSP_JOY_GetState function.
  - The LED GPIO is set to GPIO_NOPULL instead of GPIO_PULLUP to improve the power consumption.
  - Added check on parameter given to :
    - LEDx_GPIO_CLK_ENABLE(__LED__)
    - LEDx_GPIO_CLK_DISABLE(__LED__)
    - BUTTONx_GPIO_CLK_ENABLE(__BUTTON__)
    - BUTTONx_GPIO_CLK_DISABLE(__BUTTON__)

## Other compatibilities

  - Due to changes in the gpio speed definition, this version requires **STM32L1xx_HAL_Driver V1.1.2**

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.0.1 / 31-March-2015</label>
<div>

## Main Changes

  - Alignment to latest changes done on HAL driver.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 05-September-2014</label>
<div>

## Main Changes

  - First official release dedicated to **STM32CubeL1 based development**. Supported devices is STM32L152RE.

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
