#pragma once

#include "linear_algebra.h"

#include <cstddef>
#include <random>

Matrix generateRandomMatrix(std::size_t n, std::mt19937_64& rng);
Vector generateRandomVector(std::size_t n, std::mt19937_64& rng);
Matrix generateHilbertMatrix(std::size_t n);
