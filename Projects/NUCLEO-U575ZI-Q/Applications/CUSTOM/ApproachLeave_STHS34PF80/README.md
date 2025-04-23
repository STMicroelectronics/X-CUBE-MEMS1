
## <b>ApproachLeave_STHS34PF80 Application Description</b>

This application shows how to use [InfraredAL - Approach Leave](https://www.st.com) library to obtain object presence and its confidence using STHS34PF80 infrared temperature TMOS sensor only.
Data are sent from a Nucleo board using UART to a connected PC or Desktop.
To run the application, it is needed to install and launch the [MEMS Studio](https://www.st.com/en/development-tools/mems-studio.html) on the PC.
After connection has been established:

  - the user can view Object Temperature [LSB] data from STHS34PF80 infrared temperature TMOS sensor
  - the user can view Object Presence or Absence and its confidence
  - the user can store the data in csv file


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

  - This example runs on STM32 Nucleo devices with STHS34PF80 connected by wires
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board
    - NUCLEO-L073RZ RevC board
    - NUCLEO-L152RE RevC board
    - NUCLEO-U575ZI-Q RevC board


ADDITIONAL_BOARD : STEVAL-MKI231KA https://www.st.com/en/evaluation-tools/steval-mki231ka.html
ADDITIONAL_COMP : STHS34PF80 https://www.st.com/en/mems-and-sensors/sths34pf80.html


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

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
