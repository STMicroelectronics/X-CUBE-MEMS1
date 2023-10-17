Quick Start
===========

SensorBridge Example
--------------------

Following example code shows how to use this driver with a Sensirion SGP41 and an
SHTC3 humidity and temperature sensor connected to the computer using a `Sensirion SEK-SensorBridge`_.
The driver for the SensorBridge and the base SHDLC driver used in the example can be installed with

.. sourcecode:: bash

    pip install sensirion-shdlc-sensorbridge sensirion-i2c-sht sensirion-i2c-sgp4x

.. _Sensirion SEK-SensorBridge: https://www.sensirion.com/sensorbridge/

.. literalinclude:: ../example_sensorbridge.py
    :language: python


Linux I²C Hardware
------------------

The following examples show how to use the SGP41 and SHTC3 sensors with a generic Linux I²C hardware
(e.g. attached to the Raspberry Pi I²C port 1).
The dependencies can be installed as follows:

.. sourcecode:: bash

    pip install sensirion-i2c-driver sensirion-i2c-sht sensirion-i2c-sgp4x


.. literalinclude:: ../example_linux.py
    :language: python
