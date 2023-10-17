import csv
import os
import pytest

from sensirion_gas_index_algorithm.voc_algorithm import VocAlgorithm


def test_voc_algorithm():
    algorithm = VocAlgorithm()
    algorithm_version = algorithm.get_version()
    assert isinstance(algorithm_version, str)
    out = algorithm.process(1)
    assert isinstance(out, int)


def test_voc_algorithm_with_sampling_interval():
    algorithm = VocAlgorithm(sampling_interval=2)
    algorithm_version = algorithm.get_version()
    assert isinstance(algorithm_version, str)
    out = algorithm.process(1)
    assert isinstance(out, int)
    assert algorithm.get_sampling_interval() == 2


def test_voc_algorithm_get_set_states():
    algorithm = VocAlgorithm()
    algorithm.set_states(1, 2)
    state0, state1 = algorithm.get_states()
    assert state0 == 1
    assert state1 == 2


def test_voc_algorithm_get_set_tuning_parameters():
    algorithm = VocAlgorithm()
    algorithm.set_tuning_parameters(1, 2, 3, 4, 5, 6)
    assert [1, 2, 3, 4, 5, 6] == algorithm.get_tuning_parameters()


def test_voc_algorithm_process_reaches_stable_state():
    algorithm = VocAlgorithm()
    # after a few 100 samples we should reach mean
    for n in range(200):
        algorithm.process(1337)
    index = algorithm.process(1337)
    assert index == 100


def test_get_sampling_interval():
    algorithm = VocAlgorithm()
    assert algorithm.get_sampling_interval() == 1


def test_voc_algorithm_process():
    algorithm = VocAlgorithm()
    # open the file in the read mode
    path = os.path.join(os.path.dirname(__file__), 'data', 'voc.csv')
    with open(path, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            assert int(row[1]) == pytest.approx(algorithm.process(int(row[0])), 1.0)
