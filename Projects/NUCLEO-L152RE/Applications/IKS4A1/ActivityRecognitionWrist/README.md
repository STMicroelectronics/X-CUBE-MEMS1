
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}


## <b>ActivityRecognitionWrist Application Description</b>

This application shows how to use Activity Recognition for Wrist library to detect activity performed by user using accelerometer only.
Data are sent from a Nucleo board using UART to a connected PC or Desktop.
To run the application, it is needed to install and launch the [Unicleo-GUI](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/sensor-software-development-tools/unicleo-gui.html) on the PC.
After connection has been established:

  - the user can view the data from various on-board environment sensors like Temperature, Humidity, and Pressure
  - the user can view data from various on-board inertial sensors as well like Accelerometer, Gyroscope, and Magnetometer
  - the user can view activity data
  - the user can store the data in csv file
  - the user can load the offline data from csv file


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

  - This example runs on STM32 Nucleo devices with MEMS sensor expansion board (X-NUCLEO-IKS4A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board
    - NUCLEO-L152RE RevC board
    - NUCLEO-U575ZI-Q RevC board


ADDITIONAL_BOARD : X-NUCLEO-IKS4A1 https://www.st.com/en/ecosystems/x-nucleo-iks4a1.html
ADDITIONAL_COMP : LSM6DSV16X https://www.st.com/en/mems-and-sensors/lsm6dsv16x.html


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

Copyright (c) 2023 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

:::
:::

