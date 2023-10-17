from sensirion_gas_index_algorithm.nox_algorithm import NoxAlgorithm


def test_nox_algorithm():
    algorithm = NoxAlgorithm()
    algorithm_version = algorithm.get_version()
    assert isinstance(algorithm_version, str)
    out = algorithm.process(1)
    assert isinstance(out, int)


def test_nox_algorithm_get_set_tuning_parameters():
    algorithm = NoxAlgorithm()
    algorithm.set_tuning_parameters(1, 2, 3, 4, 5, 6)
    assert [1, 2, 3, 4, 5, 6] == algorithm.get_tuning_parameters()


def test_nox_algorithm_process():
    algorithm = NoxAlgorithm()
    # after a few 100 samples we should reach mean
    for n in range(200):
        algorithm.process(1337)
    index = algorithm.process(1337)
    assert index == 1
