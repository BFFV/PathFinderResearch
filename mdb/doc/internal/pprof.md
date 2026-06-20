# Profiling

This document explains how to profile CPU usage using pprof.

Useful links:

- https://github.com/google/pprof

- https://gperftools.github.io/gperftools/heapprofile.html

- https://gperftools.github.io/gperftools/cpuprofile.html

## Instructions:
- Install Go.
- `go install github.com/google/pprof@latest`
- `sudo apt install libgoogle-perftools-dev graphviz`
    - `linux-tools` search for the exact kernel version
- CMakeLists.txt already have a configuration for profiling:
    - The important thing is appending `tcmalloc` and `profiler` to target_link_libraries, sometimes is necessary to add `-Wl,-no-as-needed` before `profiler`.
- compile with: `cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release -DPROFILE=1 && cmake --build build/Release/`


Execute the command you want to profile, prefixing `env CPUPROFILE=out.prof`, where `out.prof` is the name of the file you want to create.


After the execution finished, you can analyze the output:

For plain text:
- `/home/crojas/go/bin/pprof --text build/Release/bin/profile-query out.prof`

For Web browser:
- `/home/crojas/go/bin/pprof -http=localhost:8080 build/Release/bin/profile-query out.prof`
- If you are in a server you can use an SSH tunnel to redirect the port:
    - example: `ssh crojas@s03.imfd.cl -p 201 -L 8080:localhost:8080`
