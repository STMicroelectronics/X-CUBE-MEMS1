#!/bin/bash
cp ../sensirion_gas_index_algorithm/sensirion_gas_index_algorithm.c swig/
cp ../sensirion_gas_index_algorithm/sensirion_gas_index_algorithm.h swig/
swig -Wall -python -py3 -outdir sensirion_gas_index_algorithm/ swig/sensirion_gas_index_algorithm.i
