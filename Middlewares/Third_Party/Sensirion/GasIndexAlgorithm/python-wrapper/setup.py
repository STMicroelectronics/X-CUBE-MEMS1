#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import absolute_import, division, print_function

import os
import re
from setuptools import setup, find_packages, Extension

################################
# Required developer input start
################################

# Python versions this package is compatible with
python_requires = '>=3.6,<4'

# Packages that this package imports. List everything apart from standard lib packages.
install_requires = [
    # <insert list of packages and versions here>
]

# Packages required for tests and docs
extras_require = {
    'test': [
        'flake8~=3.7.8',
        'pytest~=4.6.11',
        'pytest-cov~=2.12.1',
    ],
    'docs': [
        'sphinx~=2.2.1',
        'sphinx-rtd-theme~=1.0.0',
    ]
}

package_name = "sensirion-gas-index-algorithm"
author = 'Sensirion'
author_email = 'info@sensirion.com'
description = 'Sensirions public algorithms to calculate VOC and NOx gas indices.'
keywords = 'sensirion,gas-index,voc,nox,algorithm'


################################
# Required developer input end
################################

# Note: Remove this check only for old packages which do not (yet) have the
# "sensirion-" prefix for compatibility reasons.
assert(package_name.startswith("sensirion-"))

# It is important to NOT import anything, not even the version, from the package which is being built.
# Otherwise weird behavior is guaranteed.
version_line = open(os.path.join(package_name.replace("-", "_"), "version.py"), "rt").read()
result = re.search(r"^version = ['\"]([^'\"]*)['\"]", version_line, re.M)
if result:
    version_string = result.group(1)
else:
    raise RuntimeError("Unable to find version string")

long_description = open(os.path.join(os.path.dirname(__file__), 'README.rst')).read().strip() + "\n\n" + \
                   open(os.path.join(os.path.dirname(__file__), 'CHANGELOG.rst')).read().strip() + "\n"

swig_path = 'swig'

gas_index_algorithm_module = Extension(
    'sensirion_gas_index_algorithm._sensirion_gas_index_algorithm_wrap',
    include_dirs=[swig_path],
    library_dirs=[swig_path],
    sources=[
        os.path.join(swig_path, 'sensirion_gas_index_algorithm_wrap.c'),
        os.path.join(swig_path, 'sensirion_gas_index_algorithm.c')
    ],
    depends=[
        os.path.join(swig_path, 'sensirion_gas_index_algorithm.h')
    ],
)

setup(
    name=package_name,
    version=version_string,
    author=author,
    author_email=author_email,
    description=description,
    license='BSD',
    keywords=keywords,
    url='https://developers.sensirion.com',
    packages=find_packages(exclude=['tests', 'tests.*']),
    long_description=long_description,
    python_requires=python_requires,
    install_requires=install_requires,
    extras_require=extras_require,
    ext_modules=[gas_index_algorithm_module],
    classifiers=[
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Topic :: Software Development :: Libraries :: Python Modules'
    ]
)
