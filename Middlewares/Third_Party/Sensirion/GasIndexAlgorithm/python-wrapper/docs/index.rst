Welcome to Sensirion Gas Index Algorithm's documentation!
=========================================================

SGP4x VOC/NOx Engine Algorithm applies a gain-offset normalization algorithm to a SGP tick signal. The algorithm 
assumes a humidity compensated raw tick signal, applies state estimation, tick-to-GasIndex conversion and an 
adaptive lowpass filter.

The main goal of the VOC/NOX Engine algorithm is to calculate a VOC/NOx Index signal that enables robust detection of ambient
VOC/NOx changes, with minimal sensor-to-sensor variations. The Algorithm Engine calculates the VOC/NOx Index signal 
recursively from a single raw tick value Sout that is measured by the SGP sensor at each time step, as well as internal 
states that are updated at each time step. These internal states are most importantly the recursively estimated mean and 
variance of the Sout signal, as well as some additional internal states such as uptime and other counters.


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   quickstart
   api

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
