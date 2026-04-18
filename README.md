# Бенчмарк решения СЛАУ

Консольный проект на C++ для сравнения трёх подходов к решению систем линейных алгебраических уравнений:

- метод Гаусса без выбора ведущего элемента
- метод Гаусса с частичным выбором ведущего элемента
- LU-разложение без перестановок

## Структура проекта

- `main.cpp` - интерфейс командной строки и запуск экспериментов
- `linear_algebra.h/.cpp` - тип матрицы, нормы, невязки, умножение матрицы на вектор
- `solvers.h/.cpp` - метод Гаусса, LU-разложение, прямая и обратная подстановки
- `generators.h/.cpp` - генерация случайных матриц и векторов, а также матрицы Гильберта
- `experiments.h/.cpp` - сценарии экспериментов по заданию
- `report_io.h/.cpp` - вывод таблиц в консоль и экспорт CSV
- `tests.cpp` - простые регрессионные тесты для реализованных методов

## Сборка

### g++

Основная программа:

```bash
g++ -O2 -std=c++17 -Wall -Wextra -Wpedantic ^
  main.cpp linear_algebra.cpp solvers.cpp generators.cpp experiments.cpp report_io.cpp ^
  -o slae_benchmark
```

Тесты:

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

Через CMake собираются два исполняемых файла:

- `slae_benchmark`
- `slae_tests`

## Запуск

Основная программа:

```bash
./slae_benchmark
```

Тесты:

```bash
./slae_tests
```

Полезные флаги для основной программы:

- `--quick` для сокращённого проверочного запуска
- `--single-sizes=100,200,500,1000`
- `--multi-size=500`
- `--multi-k=1,10,100`
- `--hilbert-sizes=5,10,15`
- `--seed=42`
- `--output-dir=results`

## Выходные данные

Программа выводит таблицы в консоль и сохраняет:

- `results/single_system.csv`
- `results/multiple_rhs.csv`
- `results/hilbert_accuracy.csv`

## Примечания

Случайные матрицы для бенчмарка генерируются строго диагонально преобладающими. Это делает эксперименты воспроизводимыми и гарантирует применимость LU-разложения без перестановок к сгенерированным системам.
