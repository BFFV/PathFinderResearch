##############
Python Package
##############

We provide the pymilldb_ package which includes a Python MillenniumDB client.
Doxygen_ documentation is also available.

.. Todo::

   - Expand PyMillDB Documentation.
   - Does `pip install pymilldb` work and pull from PyPI?
   - Do we have to compile?
   - Are dependencies such as a C++ compiler needed?



Building the Client
###################

To build a Python client for MillenniumDB run the following::

   # Pull the pybind11 submodule
   git submodule update --init

   # Build and install the client
   pip install .


The package will then be available as `pymilldb`.



.. _pymilldb: https://pypi.org/project/pymilldb/
.. _Doxygen: https://millenniumdb.github.io/PyMillDB/