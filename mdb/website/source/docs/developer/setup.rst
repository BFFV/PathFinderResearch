#######################
General Developer Setup
#######################



Clang Setup
###########
Sometimes when you install clang via the package manager,
it won't be recognized by cmake as a C++ alternative.
Here is an example to change gcc to clang as the default c++ compiler in Ubuntu/Linux Mint
(you may use different versions instead of **clang-15** and **libstdc++-12-dev**)::

   sudo apt install clang-15 libstdc++-12-dev
   sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-15 10
   sudo update-alternatives --config c++



VSCode Setup
############
If VSCode does not prompt you to install the recommended extensions you can go to the
extensions section in the sidebar and search for ``@recommended`` to show all the
extensions recommended by the workspace.



Running Tests
#############
To run the tests Python >= 3.8 with venv is needed.
On Debian and Ubuntu based distributions venv is not included
with Python and has to be installed separately::

   apt-get -y install python3 python3-venv


To compile and run all the tests run::

   ./scripts/run-tests



Official Docker Image
#####################
These instructions are for **building** and **publishing**
the official MillenniumDB Docker image.

To push to the DockerHub IMFD account::

   docker login

To **build** and **push** the Docker image run the respective commands:

.. tabs::

   .. tab:: amd64

      To **build** the Docker image run::

         docker build -t imfd/millenniumdb-amd64-linux:latest .


      Then push the image to DockerHub as follows::

         docker push imfd/millenniumdb-amd64-linux:latest

   .. tab:: arm64

      To **build** the Docker image run::

         docker build -t imfd/millenniumdb-arm64-linux:latest .


      Then push the image to DockerHub as follows::

         docker push imfd/millenniumdb-arm64-linux:latest

To **create a new manifest** run::

   docker manifest create imfd/millenniumdb:latest \
                          imfd/millenniumdb-arm64-linux:latest \
                          imfd/millenniumdb-amd64-linux:latest

To **push the manifest** to DockerHub run::

   docker manifest push imfd/millenniumdb:latest


.. Tip::

   To clean all Docker data and cache you can run::

      docker container prune -f
      docker image     prune -f --all 
      docker builder   prune -f
      docker network   prune -f
