/**
  @page DataLogTerminal example based on sensor expansion board X-NUCLEO-IKS01A3 and STM32 Nucleo Boards
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    readme.txt  
  * @brief   This application contains an example which shows how to obtain data
  *          from various sensors on sensor expansion board.
  *          The communication is done using a UART connection with PC.
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

This example shows how to use sensor expansion board to send sensor data from a Nucleo board 
using UART to a connected PC or Desktop and display it on generic applications like TeraTerm.

After connection has been established:
  - the user can view the data from various on-board environment sensors like Temperature, Humidity and Pressure
  - the user can also view data from various on-board inertial sensors as well like Accelerometer, Gyroscope and Magnetometer

The sample application uses the following serial settings:
  - Baud Rate: 115200
  - Data Bits: 8
  - Parity: None
  - Stop Bits: 1

------------------------------------
WARNING: When starting the project from Example Selector in STM32CubeMX and regenerating 
it from ioc file, you may face a build issue. To solve it, remove from the IDE project 
the file stm32f4xx_nucleo.c in Application/User virtual folder and delete from Src and 
Inc folders the files: stm32f4xx_nucleo.c, stm32f4xx_nucleo.h and stm32f4xx_nucleo_errno.h.
------------------------------------

@par Keywords

MEMS, I2C, VCOM

@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with MEMS sensor expansion board (X-NUCLEO-IKS01A3)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board
    - NUCLEO-L476RG RevC board

ADDITIONAL_BOARD : X-NUCLEO-IKS01A3 https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-iks01a3.html
ADDITIONAL_COMP : LSM6DSO https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/lsm6dso.html
ADDITIONAL_COMP : LIS2DW12 https://www.st.com/content/st_com/en/products/mems-and-sensors/accelerometers/lis2dw12.html
ADDITIONAL_COMP : LIS2MDL https://www.st.com/content/st_com/en/products/mems-and-sensors/e-compasses/lis2mdl.html
ADDITIONAL_COMP : LPS22HH https://www.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps22hh.html
ADDITIONAL_COMP : HTS221 https://www.st.com/content/st_com/en/products/mems-and-sensors/humidity-sensors/hts221.html
ADDITIONAL_COMP : STTS751 https://www.st.com/content/st_com/en/products/mems-and-sensors/temperature-sensors/stts751.html

@par How to use it ? 

This package contains projects for 3 IDEs: IAR, Keil �Vision and STM32CubeIDE. In order to make the 
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
