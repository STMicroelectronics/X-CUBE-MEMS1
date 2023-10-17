Sensirion Gas Index Algorithm
=============================

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


Install
-------
.. sourcecode:: bash

    pip install sensirion-gas-index-algorithm

Recommended usage is within a virtualenv.

Usage VOC Algorithm
-------------------
.. sourcecode:: python

    from sensirion_gas_index_algorithm.voc_algorithm import VocAlgorithm

    voc_algorithm = VocAlgorithm()
    s_voc_raw = 27000  # read out from SGP41
    for _ in range(100):
        voc_index = voc_algorithm.process(s_voc_raw)
        print(voc_index)


Usage NOx Algorithm
-------------------
.. sourcecode:: python

    from sensirion_gas_index_algorithm.nox_algorithm import NoxAlgorithm

    nox_algorithm = NoxAlgorithm()
    s_nox_raw = 18000  # read out from SGP41
    for _ in range(100):
        nox_index = nox_algorithm.process(s_voc_raw)
        print(nox_index)
