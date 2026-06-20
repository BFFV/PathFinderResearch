#####################
Compiling from Source
#####################

MillenniumDB should compile on most x86-64 Linux distribution.
On windows, WSL [#WSL]_ can be used.
For macOS and Windows without WSL, check out :ref:`docker-from-source` can be used.



Install Dependencies
####################

.. tabs::

   .. tab:: Linux

      MillenniumDB needs the following dependencies:

      - GCC >= 8.1
      - CMake >= 3.12
      - Git
      - OpenSSL
      - NCursesw and Less for the CLI
      - Python >= 3.8 with venv to run tests

      On current Debian and Ubuntu based distributions they can be installed by running::

         sudo apt-get update
         sudo apt-get -y install g++ cmake libssl-dev libncurses-dev \
                                 less python3 python3-venv

   .. tab:: macOS

      Install the **Xcode Command Line Tools** and Homebrew_.

      Then install the dependencies using Homebrew::

         brew install cmake ncurses openssl@3 icu4c




Clone the Repository
####################
Clone this repository, enter the repository root directory and set `MDB_HOME`::

   git clone https://github.com/MillenniumDB/MillenniumDB
   cd MillenniumDB-Dev
   export MDB_HOME=$(pwd)



Install Boost
#############
Download `boost_1_82_0.tar.gz`_ using a browser or wget::

   wget -q --show-progress https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.gz

and run the following in the directory where boost was downloaded::

   tar -xf boost_1_82_0.tar.gz
   mkdir -p $MDB_HOME/third_party/boost_1_82/include
   mv boost_1_82_0/boost $MDB_HOME/third_party/boost_1_82/include
   rm -r boost_1_82_0.tar.gz boost_1_82_0

.. Note::

   Since we only use header-only Boost libraries it is not necessary to compile Boost separately.



Compile MillenniumDB
####################
.. Tip::

   Before compiling we recommend setting the environment variable
   `CMAKE_BUILD_PARALLEL_LEVEL` so that compilation uses all your cores::

      export CMAKE_BUILD_PARALLEL_LEVEL=$(getconf _NPROCESSORS_ONLN)

   You can also add this to your **shell startup script** (eg. `.bashrc`).


From the repository root directory run:

.. tabs::

   .. tab:: Compile
      ::

         cmake -B build/Release -D CMAKE_BUILD_TYPE=Release
         cmake --build build/Release

      The binaries will be in `build/Release/bin`.

   .. tab:: Compile and Install
      ::

         cmake -B build/Release -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=.
         cmake --build build/Release --target install

      You can change the `CMAKE_INSTALL_PREFIX` value to install MillenniumDB where you want.
      By leaving it as `.` it will be installed into the current working directory.

      The binaries will be in `CMAKE_INSTALL_PREFIX/bin`.



Running the Binaries
####################
To run the binaries after compiling you have to use their absolute or relative paths.
For example, after compiling without installing, you would use the following to run `mdb server`::

   ./build/Release/bin/mdb server <db-directory>

.. Tip::

   You could also add the `bin` directory containing the binaries to your path
   to run them without having to use relative or absolute paths.


.. _docker-from-source:

Docker Image from Source
########################
To compile from source using Docker you can use the supplied Dockerfile.
From the repository root run::

   docker build -t millenniumdb .

This will compile and create a Docker image which can be ran as follows::

   docker run --rm -it --volume "$PWD/data:/data" \
                    -p 1234:1234 -p 4321:4321 \
                    millenniumdb



.. [#WSL] Windows Subsystem for Linux

.. _Homebrew: https://brew.sh/
.. _boost_1_82_0.tar.gz: https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.gz