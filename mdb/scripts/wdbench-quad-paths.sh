#!/bin/bash

for sem in all_shortest_trails all_shortest
do
    sync; echo 3 > /proc/sys/vm/drop_caches
    python3 scripts/wdbench-quad-paths.py data/benchmark_queries/wdbench/paths.txt 100000 $sem bfs
done

for sem in any any_trails all_trails
do
    sync; echo 3 > /proc/sys/vm/drop_caches
    python3 scripts/wdbench-quad-paths.py data/benchmark_queries/wdbench/paths.txt 100000 $sem bfs

    sync; echo 3 > /proc/sys/vm/drop_caches
    python3 scripts/wdbench-quad-paths.py data/benchmark_queries/wdbench/paths.txt 100000 $sem dfs
done