# Sensirion VOC and NOx Gas Index Algorithm

Sensirions public algorithms to calculate gas indices for VOC and NOx.

This [README.md](README.md) contains information for contributors of the
project. The package description is located in [README.rst](README.rst).


# Build and test package

To build the package, the swig wrapper has to be generated first.
The following example generates the required wrapper and builds a source package and wheel for your platform

```sh
./generate_swig_wrapper.sh
pip install -e.
python setup.py sdist bdist_wheel
```

We use pytest to run the tests. Install the package and test dependencies before running tests.

```
./generate_swig_wrapper.sh
pip install -e.[test]
pytest
```


# Contributions are welcome!

We develop and test this algorithm using our company internal tools (version
control, continuous integration, code review etc.) and automatically
synchronize the master branch with GitHub. But this doesn't mean that we don't
respond to issues or don't accept pull requests on GitHub. In fact, you're very
welcome to open issues or create pull requests :)
