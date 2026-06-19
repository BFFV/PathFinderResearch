# PathFinder Research

Research for the paper: [PathFinder: A unified approach for handling paths in graph query languages](https://arxiv.org/pdf/2306.02194)

Main repository: [PathFinder Repository](https://github.com/BFFV/PathFinder)

## Benchmarking

The script used to benchmark **MillenniumDB** is located here: [MDB Benchmark Script](engines/scripts/benchmark/run_mdb.py)

The script has to be run with the following command: `python run_mdb.py [DB_NAME] [MODE] [TIMEOUT_MAX] [SEARCH_STRATEGY] [K_VALUE]`

- `[DB_NAME]` is the name of the database to use (`wdbench`, `pokec`, `diamond`, `diamond_check`)
- `[MODE]` is the path semantic to use, mapped to a specific integer number:
```
        0: ANY SHORTEST WALKS
        1: ALL SHORTEST WALKS
        2: ANY TRAILS (BFS/DFS)
        3: ALL TRAILS (BFS/DFS)
        4: ENDPOINTS (Pokec only)
        5: SHORTEST K WALKS
        6: SHORTEST K TRAILS
        7: SHORTEST K GROUPS WALKS
        8: SHORTEST K GROUPS TRAILS
        9: ANY WALKS (DFS) (WDBench only)
```
- `[TIMEOUT_MAX]` is the maximum amount of consecutive timeouts to allow for an experiment, defaults to **infinity**
- `[SEARCH_STRATEGY]` is the search algorithm to use when exploring the graph (`bfs`, `dfs`), defaults to `bfs`
- `[K_VALUE]` is the `K` value to use for `K` semantics, defaults to **1**

An example for using the **MillenniumDB** benchmark script: `python run_mdb.py wdbench 7 inf bfs 4` **(Shortest 4 Groups Walks with the WDBench database)**

* **Note:** Make sure to use a virtual environment with ``Python 3.8+`` for running the script, with all the libraries indicated in the [requirements](requirements.txt) file installed.

* **Note:** The script assumes that the paths to each **query file** and the **MillenniumDB** database engine are as defined in the code, which follows the same setup as the **PathFinder** paper experiments.

## Plot Results

The **Jupyter Notebooks** used to generate plots for the experimental results are located here: [Plot Generation](results/)

To work properly, make sure that the results are in `csv` format (which is the output of the benchmark script), and are located in the paths specified in the notebook cells.

* **Note:** Make sure to use a virtual environment with ``Python 3.8+`` for running the notebooks, with all the libraries indicated in the [requirements](requirements.txt) file installed.