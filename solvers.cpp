#include "solvers.h"

#include <cmath>

namespace {

constexpr double kPivotEpsilon = 1e-18;

bool isZeroPivot(double value) {
    return std::abs(value) < kPivotEpsilon;
}

}  // namespace

SolveResult backwardSubstitutionUpper(const Matrix& upper, const Vector& rhs) {
    SolveResult result;
    result.success = true;
    result.x.assign(upper.n, 0.0);

    for (std::size_t idx = upper.n; idx-- > 0;) {
        double sum = rhs[idx];
        for (std::size_t col = idx + 1; col < upper.n; ++col) {
            sum -= upper(idx, col) * result.x[col];
        }

        const double pivot = upper(idx, idx);
        if (isZeroPivot(pivot)) {
            result.success = false;
            result.message = "Zero pivot during backward substitution.";
            result.x.clear();
            return result;
        }

        result.x[idx] = sum / pivot;
    }

    return result;
}

SolveResult forwardSubstitutionUnitLower(const Matrix& lu, const Vector& rhs) {
    SolveResult result;
    result.success = true;
    result.x.assign(lu.n, 0.0);

    for (std::size_t row = 0; row < lu.n; ++row) {
        double sum = rhs[row];
        for (std::size_t col = 0; col < row; ++col) {
            sum -= lu(row, col) * result.x[col];
        }
        result.x[row] = sum;
    }

    return result;
}

SolveResult solveGaussNoPivot(Matrix matrix, Vector rhs) {
    for (std::size_t pivotRow = 0; pivotRow < matrix.n; ++pivotRow) {
        const double pivot = matrix(pivotRow, pivotRow);
        if (isZeroPivot(pivot)) {
            return {false, "Zero pivot in Gauss without pivoting.", {}};
        }

        for (std::size_t row = pivotRow + 1; row < matrix.n; ++row) {
            const double factor = matrix(row, pivotRow) / pivot;
            matrix(row, pivotRow) = 0.0;

            for (std::size_t col = pivotRow + 1; col < matrix.n; ++col) {
                matrix(row, col) -= factor * matrix(pivotRow, col);
            }
            rhs[row] -= factor * rhs[pivotRow];
        }
    }

    return backwardSubstitutionUpper(matrix, rhs);
}

SolveResult solveGaussPartialPivot(Matrix matrix, Vector rhs) {
    for (std::size_t pivotCol = 0; pivotCol < matrix.n; ++pivotCol) {
        std::size_t bestRow = pivotCol;
        double bestValue = std::abs(matrix(pivotCol, pivotCol));

        for (std::size_t row = pivotCol + 1; row < matrix.n; ++row) {
            const double current = std::abs(matrix(row, pivotCol));
            if (current > bestValue) {
                bestValue = current;
                bestRow = row;
            }
        }

        if (isZeroPivot(matrix(bestRow, pivotCol))) {
            return {false, "Zero pivot in Gauss with partial pivoting.", {}};
        }

        swapRows(matrix, pivotCol, bestRow);
        std::swap(rhs[pivotCol], rhs[bestRow]);

        const double pivot = matrix(pivotCol, pivotCol);
        for (std::size_t row = pivotCol + 1; row < matrix.n; ++row) {
            const double factor = matrix(row, pivotCol) / pivot;
            matrix(row, pivotCol) = 0.0;

            for (std::size_t col = pivotCol + 1; col < matrix.n; ++col) {
                matrix(row, col) -= factor * matrix(pivotCol, col);
            }
            rhs[row] -= factor * rhs[pivotCol];
        }
    }

    return backwardSubstitutionUpper(matrix, rhs);
}

LUResult luDecompose(Matrix matrix) {
    for (std::size_t pivot = 0; pivot < matrix.n; ++pivot) {
        if (isZeroPivot(matrix(pivot, pivot))) {
            return {false, "Zero pivot in LU decomposition without permutations.", {}};
        }

        for (std::size_t row = pivot + 1; row < matrix.n; ++row) {
            matrix(row, pivot) /= matrix(pivot, pivot);
            const double factor = matrix(row, pivot);

            for (std::size_t col = pivot + 1; col < matrix.n; ++col) {
                matrix(row, col) -= factor * matrix(pivot, col);
            }
        }
    }

    return {true, {}, std::move(matrix)};
}

SolveResult solveFromLU(const Matrix& lu, const Vector& rhs) {
    SolveResult y = forwardSubstitutionUnitLower(lu, rhs);
    if (!y.success) {
        return y;
    }
    return backwardSubstitutionUpper(lu, y.x);
}
