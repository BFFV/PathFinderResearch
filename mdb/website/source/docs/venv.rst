##########################
Python Virtual Environment
##########################

It is generally not a good idea to install Python packages
globally at the system level using pip (some distributions
are starting to disallow this).

Instead, the system package manager [#package_manager]_
should be used.
Another alternative is to use pip with the `--user` flag.

However, the recommended way is to use Python Virtual Environments.
The venv_ package is now part of the Python Standard Library.
To create a virtual environment run::

   python3 -m venv .venv

This creates a virtual environment in the directory `.venv`.

To activate it, run the command for your shell:

+----------+------------+---------------------------------+
| Platform | Platform   | Command                         |
+==========+============+=================================+
|          | Bash/Zsh   | `source .venv/bin/activate`     |
|          +------------+---------------------------------+
|          | Fish       | `source .venv/bin/activate.fish`|
| POSIX    +------------+---------------------------------+
|          | Csh/Tcsh   | `source .venv/bin/activate.csh` |
|          +------------+---------------------------------+
|          | PowerShell | `.venv/bin/Activate.ps1`        |
+----------+------------+---------------------------------+
|          | CMD.exe    | `.venv\Scripts\activate.bat`    |
| Windows  +------------+---------------------------------+
|          | PowerShell | `.venv\Scripts\Activate.ps1`    |
+----------+------------+---------------------------------+

After activating the environment you can install packages inside it using pip::

   python3 -m pip install <package>

.. Attention::

   For some reason `venv` is not included with Python on Debian/Ubuntu
   based distributions and has to be installed separately::

      apt-get -y install python3-venv

.. [#package_manager] such as `apt` on Debian/Ubuntu based distributions
.. _venv: https://docs.python.org/3/library/venv.html
