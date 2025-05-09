
## <b>LPS22HH_FIFOMode Example Description</b>

This example shows how to use sensor expansion board to store LPS22HH data into a FIFO, get these data from the FIFO and send them from a Nucleo board using UART to a connected PC or Desktop and display them on generic applications like TeraTerm.
After connection has been established:

  - the user can press the user button to store LPS22HH data into the FIFO and display the data stored on the terminal

The sample application uses the following serial settings:

  - Baud Rate: 115200
  - Data Bits: 8
  - Parity: None
  - Stop Bits: 1


### <b>Keywords</b>

MEMS, I2C, VCOM


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
ADDITIONAL_COMP : LPS22HH https://www.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps22hh.html


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
