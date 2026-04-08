# SLAE benchmark

Console C++ project for comparing three approaches to solving systems of linear algebraic equations:

- Gaussian elimination without pivoting
- Gaussian elimination with partial pivoting
- LU decomposition without permutations

## Project structure

- `main.cpp` - command-line interface and experiment launch
- `linear_algebra.h/.cpp` - matrix type, norms, residuals, matrix-vector multiplication
- `solvers.h/.cpp` - Gaussian elimination, LU decomposition, forward/backward substitution
- `generators.h/.cpp` - random matrices/vectors and Hilbert matrix generation
- `experiments.h/.cpp` - benchmark scenarios from the assignment
- `report_io.h/.cpp` - console tables and CSV export
- `tests.cpp` - simple regression tests for the implemented methods

## Build

### g++

Main program:

```bash
g++ -O2 -std=c++17 -Wall -Wextra -Wpedantic ^
  main.cpp linear_algebra.cpp solvers.cpp generators.cpp experiments.cpp report_io.cpp ^
  -o slae_benchmark
```

Tests:

```bash
g++ -O2 -std=c++17 -Wall -Wextra -Wpedantic ^
  tests.cpp linear_algebra.cpp solvers.cpp generators.cpp experiments.cpp report_io.cpp ^
  -o slae_tests
```

### CMake

```bash
cmake -S . -B build
cmake --build build --config Release
```

CMake builds two executables:

- `slae_benchmark`
- `slae_tests`

## Run

Main program:

```bash
./slae_benchmark
```

Tests:

```bash
./slae_tests
```

Useful flags for the main program:

- `--quick` for a shorter verification run
- `--single-sizes=100,200,500,1000`
- `--multi-size=500`
- `--multi-k=1,10,100`
- `--hilbert-sizes=5,10,15`
- `--seed=42`
- `--output-dir=results`

## Output

The benchmark prints tables to the console and writes:

- `results/single_system.csv`
- `results/multiple_rhs.csv`
- `results/hilbert_accuracy.csv`

## Notes

Random benchmark matrices are generated as strictly diagonally dominant. This keeps the experiments reproducible and ensures that LU decomposition without permutations is applicable on the generated systems.
