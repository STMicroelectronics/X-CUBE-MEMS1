
## <b>DataLogFusion Application Description</b>

This application shows how to use [MotionFX - Sensor Fusion](https://www.st.com/resource/en/user_manual/um2220-getting-started-with-motionfx-sensor-fusion-library-in-xcubemems1-expansion-for-stm32cube-stmicroelectronics.pdf) library to estimate device position by using accelerometer, gyroscope and magnetometer.
Data are sent from a Nucleo board using UART to a connected PC or Desktop.
To run the application, it is needed to install and launch the [MEMS Studio](https://www.st.com/en/development-tools/mems-studio.html) on the PC.
After connection has been established:

  - the user can view the data from various on-board environment sensors like Temperature, Humidity, and Pressure
  - the user can view data from various on-board inertial sensors as well like Accelerometer, Gyroscope, and Magnetometer
  - the user can view device position data or visualize them using teapot model
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
    - NUCLEO-L073RZ RevC board
    - NUCLEO-L152RE RevC board
    - NUCLEO-U575ZI-Q RevC board


ADDITIONAL_BOARD : X-NUCLEO-IKS4A1 https://www.st.com/en/ecosystems/x-nucleo-iks4a1.html
ADDITIONAL_COMP : LSM6DSV16X https://www.st.com/en/mems-and-sensors/lsm6dsv16x.html
ADDITIONAL_COMP : LIS2MDL https://www.st.com/en/mems-and-sensors/lis2mdl.html


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
