CHANGELOG
---------

3.2.2
:::::

Fix type issue on non 32-bit platforms. If `mSraw_Minimum` parameter is
defined as int instead of explicitly as `int32_t` it can lead to an
overflow issue causing the algorithm to the maximal index.


3.2.1
:::::

- Use floating point version of Gas Index Algorithm

3.2.0
:::::

- Add low power examples
- Allow chaning of sampling interval

3.1.0
:::::

- Add VOC gas index algorithm version 3.1.0
- Add NOx gas index algorithm version 3.1.0

