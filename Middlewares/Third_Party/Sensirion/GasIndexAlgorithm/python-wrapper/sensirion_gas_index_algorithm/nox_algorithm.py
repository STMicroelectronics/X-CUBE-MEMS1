# -*- coding: utf-8 -*-

"""NoxAlgorithm"""

import logging

from . import gas_index_algorithm, sensirion_gas_index_algorithm_wrap

log = logging.getLogger(__name__)


class NoxAlgorithm(gas_index_algorithm.GasIndexAlgorithm):

    def __init__(self):
        log.debug("Instance of NoxAlgorithm created.")
        super().__init__(sensirion_gas_index_algorithm_wrap.GasIndexAlgorithm_ALGORITHM_TYPE_NOX)
