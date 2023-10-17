# Sensirion Gas Index Algorithm

Sensirion's Gas Index Algorithm software provides a VOC and an NOx Index output signal calculated from the SGP40/41 raw
signal inputs `SRAW_VOC` and `SRAW_NOX`. Note: for SGP40, only `SRAW_VOC` is available. This algorithm enables robust detection of
ambient changes of VOCs and NOx with minimal sensor-to-sensor variation. The algorithm is based on a statistical gain-offset
normalization and adapts both parameters constantly applying an exponentially decaying function of the learned parameters to
be able to adapt to changing environments.

For the VOC Index output, the software must be instanced as VOC Algorithm while for the NOx Index output, the software must
be instanced as NOx Algorithm. It is important to feed the raw signals to the corresponding algorithm (i.e., `SRAW_VOC` to the
VOC Algorithm and `SRAW_NOX` to the NOx Algorithm) at a constant sampling interval which must coincide with the sampling
interval that is used to read out the raw signals from the SGP40/41 sensor. The default sampling interval applied in the
algorithm is 1 s. In case, a different sampling interval should be used the definition of the sampling interval in the h.file
of the algorithm must be changed, too.

The algorithm calculates the VOC and NOx Index signals recursively from a single raw tick value of `SRAW_VOC` and `SRAW_NOX`,
respectively, which are both measured by the SGP40/41 sensor at each time step, as well as internal states that are updated
at each time step. These internal states are most importantly the recursively estimated mean and variance of the
corresponding `SRAW` signal as well as some additional internal states such as uptime and other counters. After estimating the
states, the algorithm converts the raw signals in ticks into either VOC or NOx Index, respectively, and applies an adaptive
low-pass filter.

# Quick Start

Steps to calculate a VOC/NOx gas index value:

1. Allocate a struct of type **GasIndexAlgorithmParams** and initialize the parameters to their default values.
   
   a. For **VOC Engine Algorithm** initialize with: 
   
   ```
   GasIndexAlgorithmParams params;
   GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
   ```
   
   b. For **NOx Engine Algorithm** initialze with
   ```
   GasIndexAlgorithmParams params;
   GasIndexAlgorithm_init(&params, GasIndexAlgorithm_ALGORITHM_TYPE_NOX);
   ```
2. Read RH and T values, e.g. from a SHT4x sensor
   
3. **Read VOC or NOx raw value** (ticks) from SGP4x sensor. 
   Provide RH and T values as ticks from step 2 to the VOC read function to get temperature and humidity compensated raw index value.
   NOTE: do consider that e.g. the RH ticks for SHT4x are specified differently than the RH ticks requested by SGP41, checkout the sensor datasheets for more details.

4. **Process raw signal** to get index value
   
   ```
   int32_t voc_raw_value; // read from sensor in step 2-3
   int32_t voc_index_value; 
   GasIndexAlgorithm_process(&params, voc_raw_value, &voc_index_value)
   ```
## Fix point version

If your platform does not support floating point you can use the fixpoint version of the algorithm.
The code can be found in the folder sensirion_gas_index_algorithm_fixpoint.

This version does not provide an interface to change the sampling rate from code to allow minimal
code size. The sampling rate can be change in the header file
`sensirion_gas_index_algorithm_fixpoint/sensirion_gas_index_algorithm.h` by changing the define
`GasIndexAlgorithm_SAMPLING_INTERVAL` on line `69`-


## Raspberry Pi Example VOC and NOx Index

The example measures VOC and NOx ticks with a SGP41 sensor using a SHT4x to compensate temperature and humidity.
The raw VOC and NOx measurement signals are then processed with the gas index algorithm to get VOC Index and NOx Index values.

For more details about the sensors and breakout boards check out http://sensirion.com/my-sgp-ek/.

Steps to run the example on your Raspberry Pi:

1. Connect a SGP41 and SHT4x sensor over I2C to your Raspberry Pi
2. Download this package from [Sensirion Github Page](https://github.com/Sensirion/gas-index-algorithm).
   Extract the ZIP file and navigate to the subfolder `examples/raspberry-pi`
3. Get additional source files:
   1. Run `make download`
   2. If the download with make does not work, you can manually get the needed source files:
      - Download [Raspberyy PI I2C SHT4x driver package](https://github.com/Sensirion/raspberry-pi-i2c-sht4x). 
        Extract ZIP and add all `*.c` and `*.h` files to `examples/raspberry-pi/`
      - Download [Raspberyy PI I2C SGP41 driver package](https://github.com/Sensirion/raspberry-pi-i2c-sgp41).
        Extract ZIP and copy `sgp41.c` and `sgp41.h` to `examples/raspberry-pi/`
      - Copy content of folder `sensirion_gas_index_algorithm` to `examples/raspberry-pi`
4. Copy the `examples/raspberry-pi` folder to your Raspberry Pi
5. Compile the example on your Raspberry Pi
   1. Open a [terminal](https://www.raspberrypi.org/documentation/usage/terminal/?)
   2. Navigate to the example directory. E.g. `cd ~/examples/raspberry-pi`
   3. Run `make all` to compile the driver
6. Run the example with `./algorithm_example_usage`


# Python Example

Find the Python usage and documentation in the [Python README](python-wrapper/README.rst).

# Algorithm usage with SGP40 in low power mode
The provided low power examples (python+raspberry) demonstrate how to run the SGP40 sensor in low power mode and apply 
the VOC index algorithm to the acquired data. Reduced power consumption is achieved by turning off the heater after 
each measurement. The heater is then turned back on by calling for a first ignored measurement that precedes the actual 
measurement call by 170ms. 

To achieve further power consumption reduction, the sampling interval can be increased to 10s (from the default 1s). 
Note that the sampling frequency needs to be passed to the VOC algorithm at initialization.

The following two low power modes have been tested:

Duty cycle | Sampling interval | Average  power  consumption at 1.8V
 --- | --- | --- 
Continuous | 1 s | 6.3mW
20%| 1 s | <2.0mW
2%| 10 s | <0.2mW



# Contributing

**Contributions are welcome!**

We develop and test this algorithm using our company internal tools (version
control, continuous integration, code review etc.) and automatically
synchronize the master branch with GitHub. But this doesn't mean that we don't
respond to issues or don't accept pull requests on GitHub. In fact, you're very
welcome to open issues or create pull requests :)

This Sensirion library uses
[`clang-format`](https://releases.llvm.org/download.html) to standardize the
formatting of all our `.c` and `.h` files. Make sure your contributions are
formatted accordingly:

The `-i` flag will apply the format changes to the files listed.

```bash
clang-format -i */*.c */*.h
```

Note that differences from this formatting will result in a failed build until
they are fixed.

# License

See [LICENSE](LICENSE).

