# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [Unreleased]

## [3.2.2] - 2022-12-05

Fix type issue on non 32-bit platforms. If `mSraw_Minimum` parameter is
defined as int instead of explicitly as `int32_t` it can lead to an
overflow issue causing the algorithm to the maximal index.

## [3.2.1] - 2022-06-16

Add floating point version of algorithm and use it in python and
raspberry pi.

## [3.2.0] - 2022-05-19

Add low power examples and allow changing sampling interval on startup


## [3.1.0] - 2021-11-19

Sensirion VOC/NOx Engine algorithm, algorithm version 3.1.

[Unreleased]: https://github.com/Sensirion/gas-index-algorithm/compare/3.2.2..master
[3.2.2]: https://github.com/Sensirion/gas-index-algorithm/compare/3.2.1..3.2.2
[3.2.1]: https://github.com/Sensirion/gas-index-algorithm/compare/3.2.0..3.2.1
[3.2.0]: https://github.com/Sensirion/gas-index-algorithm/compare/3.1.0..3.2.0
[3.1.0]: https://github.com/Sensirion/gas-index-algorithm/releases/tag/3.1.0

