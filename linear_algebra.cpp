#include "linear_algebra.h"

#include <algorithm>
#include <cmath>

void swapRows(Matrix& matrix, std::size_t first, std::size_t second) {
    if (first == second) {
        return;
    }

    for (std::size_t col = 0; col < matrix.n; ++col) {
        std::swap(matrix(first, col), matrix(second, col));
    }
}

double l2Norm(const Vector& values) {
    double sum = 0.0;
    for (double value : values) {
        sum += value * value;
    }
    return std::sqrt(sum);
}

Vector multiply(const Matrix& matrix, const Vector& vector) {
    Vector result(matrix.n, 0.0);

    for (std::size_t row = 0; row < matrix.n; ++row) {
        double sum = 0.0;
        for (std::size_t col = 0; col < matrix.n; ++col) {
            sum += matrix(row, col) * vector[col];
        }
        result[row] = sum;
    }

    return result;
}

double residualNorm(const Matrix& matrix, const Vector& x, const Vector& b) {
    Vector ax = multiply(matrix, x);
    for (std::size_t i = 0; i < ax.size(); ++i) {
        ax[i] -= b[i];
    }
    return l2Norm(ax);
}

double relativeError(const Vector& actual, const Vector& expected) {
    Vector diff(actual.size(), 0.0);
    for (std::size_t i = 0; i < actual.size(); ++i) {
        diff[i] = actual[i] - expected[i];
    }

    const double denominator = l2Norm(expected);
    if (denominator == 0.0) {
        return 0.0;
    }

    return l2Norm(diff) / denominator;
}
