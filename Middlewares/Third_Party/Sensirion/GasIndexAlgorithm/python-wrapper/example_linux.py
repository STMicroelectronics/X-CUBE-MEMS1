import time
from sensirion_i2c_driver import LinuxI2cTransceiver, I2cConnection
from sensirion_i2c_sgp4x import Sgp41I2cDevice
from sensirion_i2c_sht.shtc3 import Shtc3I2cDevice
from sensirion_gas_index_algorithm.voc_algorithm import VocAlgorithm
from sensirion_gas_index_algorithm.nox_algorithm import NoxAlgorithm

voc_algorithm = VocAlgorithm()
nox_algorithm = NoxAlgorithm()

with I2cConnection(LinuxI2cTransceiver('/dev/i2c-1')) as i2c_connection:
    sgp41 = Sgp41I2cDevice(i2c_connection)
    print("SGP41 Serial Number: {}".format(sgp41.get_serial_number()))

    shtc3 = Shtc3I2cDevice(i2c_connection)
    print("SHTC3 Product ID: {}".format(shtc3.read_product_id()))
    shtc3.enter_sleep()

    # Run  conditioning for 10 seconds
    for _ in range(10):
        time.sleep(1)
        temperature, humidity = shtc3.measure()
        sraw_voc = sgp41.conditioning(temperature=temperature.degrees_celsius,
                                      relative_humidity=humidity.percent_rh)
        # use default formatting for printing output:
        voc_index = voc_algorithm.process(sraw_voc.ticks)
        print("VOC Index: {}\t\tNOx Index: conditioning".format(voc_index))

    # Measure every second for 2 minutes
    for _ in range(120):
        time.sleep(1)
        temperature, humidity = shtc3.measure()
        sraw_voc, sraw_nox = sgp41.measure_raw(temperature=temperature.degrees_celsius,
                                               relative_humidity=humidity.percent_rh)
        # use default formatting for printing output:
        voc_index = voc_algorithm.process(sraw_voc.ticks)
        nox_index = nox_algorithm.process(sraw_nox.ticks)
        print("VOC Index: {}\t\tNOx Index: {}".format(voc_index, nox_index))
