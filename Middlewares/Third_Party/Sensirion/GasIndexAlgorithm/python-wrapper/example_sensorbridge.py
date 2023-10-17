"""
This example uses an SGP41 and SHTC3 connected to a Sensirion SEK-SensorBridge

Install the dependencies
pip install sensirion-shdlc-sensorbridge sensirion-i2c-sht sensirion-i2c-sgp4x
"""

import time
from sensirion_shdlc_driver import ShdlcSerialPort, ShdlcConnection
from sensirion_shdlc_sensorbridge import SensorBridgePort, \
    SensorBridgeShdlcDevice, SensorBridgeI2cProxy
from sensirion_i2c_driver import I2cConnection
from sensirion_i2c_sgp4x import Sgp41I2cDevice
from sensirion_i2c_sht.shtc3 import Shtc3I2cDevice
from sensirion_gas_index_algorithm.voc_algorithm import VocAlgorithm
from sensirion_gas_index_algorithm.nox_algorithm import NoxAlgorithm

voc_algorithm = VocAlgorithm()
nox_algorithm = NoxAlgorithm()

# Connect to the SensorBridge with default settings:
#  - baudrate:      460800
#  - slave address: 0
with ShdlcSerialPort(port='COM1', baudrate=460800) as port:
    bridge = SensorBridgeShdlcDevice(ShdlcConnection(port), slave_address=0)
    print("SensorBridge SN: {}".format(bridge.get_serial_number()))

    # Configure SensorBridge port 1 for SGP40
    bridge.set_i2c_frequency(SensorBridgePort.ONE, frequency=100e3)
    bridge.set_supply_voltage(SensorBridgePort.ONE, voltage=3.3)
    bridge.switch_supply_on(SensorBridgePort.ONE)

    # Create SGP40 device
    i2c_transceiver = SensorBridgeI2cProxy(bridge, port=SensorBridgePort.ONE)
    connection = I2cConnection(i2c_transceiver)

    sgp41 = Sgp41I2cDevice(connection)
    print("SGP41 Serial Number: {}".format(sgp41.get_serial_number()))

    shtc3 = Shtc3I2cDevice(connection)
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

    bridge.switch_supply_off(SensorBridgePort.ONE)
