/**
  @page GyroscopeCalibration application based on sensor expansion board X-NUCLEO-IKS02A1 and STM32 Nucleo Boards

  @verbatim
  ******************** (C) COPYRIGHT 2022 STMicroelectronics *******************
  * @file       readme.txt
  * @brief      This application contains an example which shows how to use
  *             Sensor expansion board to send Gyroscope Calibration 
  *             data from a Nucleo board using UART to a connected PC and
  *             display it on Unicleo-GUI specific application, which is
  *             developed by STMicroelectronics.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

Main function is to show how to use gyroscope calibration library to 
correct gyroscope sensor data by using gyroscope and accelerometer sensors.

To run the application, it is needed to install and launch the Unicleo-GUI on the PC.
The Unicleo-GUI is available on https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/sensor-software-development-tools/unicleo-gui.html

@par Keywords

MEMS, I2C, VCOM

@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with MEMS sensor expansion board (X-NUCLEO-IKS02A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board
    - NUCLEO-L073RZ RevC board
    - NUCLEO-L152RE RevC board
    - NUCLEO-U575ZI-Q RevC board

ADDITIONAL_BOARD : X-NUCLEO-IKS02A1 https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-iks02a1.html
ADDITIONAL_COMP : ISM330DHCX https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/ism330dhcx.html

@par How to use it ? 

This package contains projects for 3 IDEs: IAR, Keil µVision and STM32CubeIDE. In order to make the 
program work, you must do the following:
  - WARNING: before opening the project with any toolchain be sure your folder
    installation path is not too in-depth since the toolchain may report errors
    after building.
  - Open STM32CubeIDE or alternatively the Keil uVision toolchain or the IAR toolchain.
  - Rebuild all files and load your image into target memory.
  - Run the example.
  - Alternatively, you can download the pre-built binaries in "Binary" 
    folder included in the distributed package.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */