#include "experiments.h"
#include "linear_algebra.h"
#include "solvers.h"

#include <cmath>
#include <initializer_list>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void expectNear(double actual, double expected, double eps, const std::string& message) {
    if (std::abs(actual - expected) > eps) {
        throw std::runtime_error(message);
    }
}

void expectVectorNear(const Vector& actual, const Vector& expected, double eps, const std::string& message) {
    require(actual.size() == expected.size(), message + " (size mismatch)");
    for (std::size_t i = 0; i < actual.size(); ++i) {
        if (std::abs(actual[i] - expected[i]) > eps) {
            throw std::runtime_error(message + " at index " + std::to_string(i));
        }
    }
}

Matrix buildMatrix(std::initializer_list<std::initializer_list<double>> rows) {
    Matrix matrix(rows.size());
    std::size_t rowIndex = 0;
    for (const auto& row : rows) {
        std::size_t colIndex = 0;
        for (double value : row) {
            matrix(rowIndex, colIndex) = value;
            ++colIndex;
        }
        ++rowIndex;
    }
    return matrix;
}

void testGaussWithoutPivot() {
    const Matrix matrix = buildMatrix({
        {2.0, 1.0, -1.0},
        {-3.0, -1.0, 2.0},
        {-2.0, 1.0, 2.0}
    });
    const Vector rhs {8.0, -11.0, -3.0};
    const Vector expected {2.0, 3.0, -1.0};

    const SolveResult result = solveGaussNoPivot(matrix, rhs);
    require(result.success, "Gauss without pivot should solve the reference system.");
    expectVectorNear(result.x, expected, 1e-9, "Gauss without pivot returned a wrong solution.");
}

void testGaussPartialPivotHandlesSwap() {
    const Matrix matrix = buildMatrix({
        {0.0, 2.0, 9.0},
        {1.0, -1.0, 2.0},
        {3.0, 2.0, -1.0}
    });
    const Vector expected {1.0, 2.0, 3.0};
    const Vector rhs = multiply(matrix, expected);

    const SolveResult noPivot = solveGaussNoPivot(matrix, rhs);
    require(!noPivot.success, "Gauss without pivot should fail when the first pivot is zero.");

    const SolveResult pivoted = solveGaussPartialPivot(matrix, rhs);
    require(pivoted.success, "Gauss with partial pivoting should solve the swapped system.");
    expectVectorNear(pivoted.x, expected, 1e-9, "Gauss with partial pivoting returned a wrong solution.");
}

void testLUSolve() {
    const Matrix matrix = buildMatrix({
        {4.0, 2.0, 0.0},
        {4.0, 4.0, 2.0},
        {2.0, 2.0, 3.0}
    });
    const Vector expected {1.0, -2.0, 3.0};
    const Vector rhs = multiply(matrix, expected);

    const LUResult lu = luDecompose(matrix);
    require(lu.success, "LU decomposition should succeed on the test matrix.");

    const SolveResult result = solveFromLU(lu.lu, rhs);
    require(result.success, "Solving from LU should succeed on the test matrix.");
    expectVectorNear(result.x, expected, 1e-9, "LU solve returned a wrong solution.");
    expectNear(residualNorm(matrix, result.x, rhs), 0.0, 1e-9, "LU solve residual is too large.");
}

void testExperimentSmoke() {
    std::mt19937_64 rng(42);
    const SingleSystemRow row = runSingleSystemExperiment(10, rng);

    require(row.gauss_no_pivot_ok, "Single-system experiment: Gauss without pivot failed.");
    require(row.gauss_pivot_ok, "Single-system experiment: Gauss with pivot failed.");
    require(row.lu_ok, "Single-system experiment: LU solve failed.");
}

}  // namespace

int main() {
    try {
        testGaussWithoutPivot();
        testGaussPartialPivotHandlesSwap();
        testLUSolve();
        testExperimentSmoke();

        std::cout << "All tests passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }
}
