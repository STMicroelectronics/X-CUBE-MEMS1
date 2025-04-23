
## <b>LSM6DSO_SingleDoubleTap Example Description</b>

This example shows how to detect the single and double tap events using the sensor expansion board and a STM32 Nucleo board.
After application is started:

  - the user can try to tap the STM32 Nucleo board; when the single tap is detected, the LED is switched on for a while
  - the user can push the button to pass from the single tap detection to the double tap detection feature; when the double tap is detected, the LED is switched on twice for a while
  - the user can push the button again to disable the single/double tap detection feature
  - the user can push the button again to enable the single tap detection feature and so on


### <b>Keywords</b>

MEMS, I2C


### <b>Directory contents</b>

  - Binary - contains binary file to be directly downloaded to corresponding HW
  - EWARM - contains project files for IAR toolchain
  - Inc - contains header files
  - MDK-ARM - contains project files for Keil µVision toolchain
  - Src - contains source files
  - STM32CubeIDE - contains project files for STM32CubeIDE toolchain


### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo devices with MEMS sensor expansion board (X-NUCLEO-IKS01A3)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board
    - NUCLEO-U575ZI-Q RevC board


ADDITIONAL_BOARD : X-NUCLEO-IKS01A3 https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-iks01a3.html
ADDITIONAL_COMP : LSM6DSO https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/lsm6dso.html


### <b>How to use it?</b>

In order to make the program work, you must do the following:

  - WARNING: before opening the project with any toolchain be sure your folder installation path is not too in-depth since the toolchain may report errors after building
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the FW application
  - Alternatively, you can download the pre-built binary in "Binary" folder included in the distributed package

### <b>Author</b>

STMicroelectronics

### <b>License</b>

Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
