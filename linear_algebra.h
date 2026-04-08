#pragma once

#include <cstddef>
#include <vector>

using Vector = std::vector<double>;

struct Matrix {
    std::size_t n = 0;
    std::vector<double> data;

    Matrix() = default;

    explicit Matrix(std::size_t size) : n(size), data(size * size, 0.0) {}

    double& operator()(std::size_t row, std::size_t col) {
        return data[row * n + col];
    }

    double operator()(std::size_t row, std::size_t col) const {
        return data[row * n + col];
    }
};

void swapRows(Matrix& matrix, std::size_t first, std::size_t second);
double l2Norm(const Vector& values);
Vector multiply(const Matrix& matrix, const Vector& vector);
double residualNorm(const Matrix& matrix, const Vector& x, const Vector& b);
double relativeError(const Vector& actual, const Vector& expected);
