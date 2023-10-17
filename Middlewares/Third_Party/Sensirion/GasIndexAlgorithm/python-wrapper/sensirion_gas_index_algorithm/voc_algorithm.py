# -*- coding: utf-8 -*-

"""VocAlgorithm"""

import logging

from . import gas_index_algorithm, sensirion_gas_index_algorithm_wrap

log = logging.getLogger(__name__)


class VocAlgorithm(gas_index_algorithm.GasIndexAlgorithm):

    def __init__(self,
                 sampling_interval: int =
                 sensirion_gas_index_algorithm_wrap.GasIndexAlgorithm_DEFAULT_SAMPLING_INTERVAL):
        log.debug("Instance of VocAlgorithm created.")
        super().__init__(sensirion_gas_index_algorithm_wrap.GasIndexAlgorithm_ALGORITHM_TYPE_VOC, sampling_interval)

    def set_states(self, state0, state1):
        """
        Set previously retrieved algorithm states to resume operation after a short interruption, skipping initial
        learning phase. This feature should not be used after interruptions of more than 10 minutes. Call this once
        after init() and the optional set_tuning_parameters(), if desired. Otherwise, the algorithm will start with
        initial learning phase.

        :param state0:
            State0 to be restored
        :param state1:
            State1 to be restored
        """
        self._algorithm.set_states(state0, state1)

    def get_states(self):
        """
        Get current algorithm states. Retrieved values can be used in
        set_states() to resume operation after a short interruption, skipping
        initial learning phase. This feature can only be used after at least 3
        hours of continuous operation.

        :rtype: tuple
        :return:
            - state0
            - state1
        """
        return self._algorithm.get_states()
