# -*- coding: utf-8 -*-

"""GasIndexAlgorithm"""

import logging
import warnings

from . import sensirion_gas_index_algorithm_wrap

log = logging.getLogger(__name__)


class GasIndexAlgorithm(object):

    def __init__(self,
                 algorithm_type: int,
                 sampling_interval: int =
                 sensirion_gas_index_algorithm_wrap.GasIndexAlgorithm_DEFAULT_SAMPLING_INTERVAL):
        self._algorithm_type = algorithm_type
        self._algorithm = sensirion_gas_index_algorithm_wrap.GasIndexAlgorithmParams(algorithm_type, sampling_interval)

    def get_version(self):
        """Get the version of the engine."""
        return self._algorithm.get_version()

    def init(self,
             sampling_interval: int =
             sensirion_gas_index_algorithm_wrap.GasIndexAlgorithm_DEFAULT_SAMPLING_INTERVAL):
        """Re-initialize / reset the engine."""
        self._algorithm.init(self._algorithm_type, sampling_interval)

    def get_tuning_parameters(self):
        """
        Get tuning parameters. See set_tuning_parameters for a documentation of the parameters.

        .. note:: The parameters learning_time_gain_hours and std_initial can be ignored
                  for the NOx gas index algorithm.

        :rtype: tuple
        :return:
            - index_offset
            - learning_time_offset_hours
            - learning_time_gain_hours
            - gating_max_duration_minutes
            - std_initial
            - gain_factor
        """
        return self._algorithm.get_tuning_parameters()

    def set_tuning_parameters(self, index_offset: int, learning_time_offset_hours: int,
                              learning_time_gain_hours: int, gating_max_duration_minutes: int,
                              std_initial: int, gain_factor: int):
        """
        Set parameters to customize the algorithm. Call this once after init(), if desired. Otherwise, the default
        values will be used.

        :param index_offset:
            Index representing typical (average) conditions. (Range: 1..250; Default: 100 for VOC 1 for NOx)
        :param learning_time_offset_hours:
            Time constant to estimate the offset from the history in hours. Past events will be forgotten after about
            twice the learning time. (Range: 1..1000 [hours]; Default 12 [hours])
        :param learning_time_gain_hours:
            Time constant to estimate the gain from the history in hours. Past events will be forgotten after about
            twice the learning time. (Range: 1..1000 [hours]; Default 12 [hours])
        :param gating_max_duration_minutes:
            Maximum duration of gating (freeze of estimator during high VOC index signal). Set to 0 to disable gating
            (Range: 1..3000 [minutes]; Default: 180 for VOC and 720 for NOx)
        :param std_initial:
            Initial estimate for standard deviation. Lower value boosts events during initial learning period, but
            may result in larger device-to-device variations. (Range: 10..5000; Default: 50)
        :param gain_factor:
            Factor to amplify or to attenuate the Index output. (Range: 1..1000; Default 230)

        .. note:: The parameters learning_time_gain_hours and std_initial have no effect
                  for the NOx gas index algorithm.
        """
        self._algorithm.set_tuning_parameters(index_offset,
                                              learning_time_offset_hours,
                                              learning_time_gain_hours,
                                              gating_max_duration_minutes,
                                              std_initial, gain_factor)

    def get_sampling_interval(self):
        """
        Get the sampling interval used by the algorithm.

        :return:
            The sampling interval in seconds
        """
        return self._algorithm.get_sampling_interval()

    def process(self, s_sgp: int):
        """Feed the engine and return compensated value.

        :param s_sgp:
            Raw signal (ticks) from SGP sensor as integer

        :return:
            Compensated index
        """
        if not isinstance(s_sgp, int):
            warnings.warn("s_sgp should be an int, casting it.")
            s_sgp = int(s_sgp)

        return self._algorithm.process(s_sgp)
