######
Docker
######
To launch the official Docker image run one of the following commands:

.. tabs::

   .. tab:: Linux/macOS
      ::

         docker run --rm -it --volume "$PWD/data:/data" \
                             -p 1234:1234 -p 4321:4321 \
                             imfd/millenniumdb

   .. tab:: Windows CMD
      ::

         docker run --rm -it --volume "%cd%/data:/data" ^
                             -p 1234:1234 -p 4321:4321 ^
                             imfd/millenniumdb

The directory `data` in the current working directory can be used
to persist or pass data to the Docker container.

The container will be running in interactive mode with all the MillenniumDB commands available.
You can create databases using `mdb import` and start a server using `mdb server`
or directly run the CLI inside the container using `mdb cli`.

Checkout :doc:`/docs/getting-started` for instructions.

If you are running a server inside the container you can send
queries to `localhost:1234` [#dest]_ from outside the container.

.. Note::

   If there are no images available on Docker Hub for your system you
   can build your own :ref:`docker-from-source`.

.. [#dest] Those are the default values used in our executables and scripts
