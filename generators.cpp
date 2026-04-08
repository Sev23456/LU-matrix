#include "generators.h"

#include <cmath>

Matrix generateRandomMatrix(std::size_t n, std::mt19937_64& rng) {
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    Matrix matrix(n);

    for (std::size_t row = 0; row < n; ++row) {
        double rowSum = 0.0;
        for (std::size_t col = 0; col < n; ++col) {
            if (row == col) {
                continue;
            }

            const double value = distribution(rng);
            matrix(row, col) = value;
            rowSum += std::abs(value);
        }

        const double diagonalNoise = distribution(rng);
        matrix(row, row) = diagonalNoise >= 0.0
            ? diagonalNoise + rowSum + 1.0
            : diagonalNoise - rowSum - 1.0;
    }

    return matrix;
}

Vector generateRandomVector(std::size_t n, std::mt19937_64& rng) {
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    Vector vector(n, 0.0);

    for (double& value : vector) {
        value = distribution(rng);
    }

    return vector;
}

Matrix generateHilbertMatrix(std::size_t n) {
    Matrix matrix(n);

    for (std::size_t row = 0; row < n; ++row) {
        for (std::size_t col = 0; col < n; ++col) {
            matrix(row, col) = 1.0 / static_cast<double>(row + col + 1);
        }
    }

    return matrix;
}
