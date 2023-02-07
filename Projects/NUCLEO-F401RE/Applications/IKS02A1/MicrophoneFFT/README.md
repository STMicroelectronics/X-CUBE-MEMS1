
## <b>MicrophoneFFT Application Description</b>

This application shows how to perform acquisition of the on-board digital MEMS microphones, PDM to PCM decimation, FFT processing and streaming of the result toward a host PC. A digital MEMS microphone can be acquired by using different peripherals, such as SPI, I2S, SAI, GPIO or DFSDM. It requires an input clock and it outputs a PDM stream at the same frequency of the input clock. This PDM stream is further filtered and decimated in order to be converted in PCM format, that is the main standard for audio transmission; this application is based on I2S or SAI acquisition and in any case DMA is used to transfer data from acquisition peripherals to internal SRAM. Using BSP layer and Middlewares functionalities, all the required peripherals and libraries are configured (PDM to PCM library, FFT library), basing on the desired sampling frequency to be achieved, which is, in this case, equal to 48 KHz. Two tasks of different priorities are created exploiting sw-generated interrupts allowing to continue microphone acquisition seamlessly, even when high amount of processing power is needed for the algorithms.
To run the application, it is needed to install and launch the [Unicleo-GUI](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/sensor-software-development-tools/unicleo-gui.html) on the PC.
After acquisition and PDM to PCM conversion, a complete spectral analysis is performed, based on the following steps:

  - Windowing
  - Fast Fourier Transform (FFT)
  - Normalization
  - Averaging


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

  - This example runs on STM32 Nucleo devices with MEMS sensor expansion board (X-NUCLEO-IKS02A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board


ADDITIONAL_BOARD : X-NUCLEO-IKS02A1 https://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-sense-hw/x-nucleo-iks02a1.html
ADDITIONAL_COMP : IMP34DT05 https://www.st.com/content/st_com/en/products/mems-and-sensors/mems-microphones/imp34dt05.html


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
