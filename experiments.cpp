#include "experiments.h"

#include "generators.h"
#include "linear_algebra.h"

#include <algorithm>
#include <chrono>

namespace {

template <typename Func>
double measureMilliseconds(Func&& func) {
    const auto start = std::chrono::high_resolution_clock::now();
    func();
    const auto finish = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(finish - start).count();
}

}  // namespace

SingleSystemRow runSingleSystemExperiment(std::size_t n, std::mt19937_64& rng) {
    SingleSystemRow row;
    row.n = n;

    const Matrix matrix = generateRandomMatrix(n, rng);
    const Vector rhs = generateRandomVector(n, rng);

    SolveResult gaussNoPivot;
    row.gauss_no_pivot_ms = measureMilliseconds([&] {
        gaussNoPivot = solveGaussNoPivot(matrix, rhs);
    });
    row.gauss_no_pivot_ok = gaussNoPivot.success;
    if (gaussNoPivot.success) {
        row.gauss_no_pivot_residual = residualNorm(matrix, gaussNoPivot.x, rhs);
    }

    SolveResult gaussPivot;
    row.gauss_pivot_ms = measureMilliseconds([&] {
        gaussPivot = solveGaussPartialPivot(matrix, rhs);
    });
    row.gauss_pivot_ok = gaussPivot.success;
    if (gaussPivot.success) {
        row.gauss_pivot_residual = residualNorm(matrix, gaussPivot.x, rhs);
    }

    LUResult lu;
    row.lu_factor_ms = measureMilliseconds([&] {
        lu = luDecompose(matrix);
    });

    if (!lu.success) {
        return row;
    }

    SolveResult luSolution;
    row.lu_solve_ms = measureMilliseconds([&] {
        luSolution = solveFromLU(lu.lu, rhs);
    });

    row.lu_ok = luSolution.success;
    if (luSolution.success) {
        row.lu_residual = residualNorm(matrix, luSolution.x, rhs);
        row.lu_total_ms = row.lu_factor_ms + row.lu_solve_ms;
    }

    return row;
}

MultipleRhsRow runMultipleRhsExperiment(std::size_t n, std::size_t k, std::mt19937_64& rng) {
    MultipleRhsRow row;
    row.n = n;
    row.k = k;

    const Matrix matrix = generateRandomMatrix(n, rng);
    std::vector<Vector> rightHandSides;
    rightHandSides.reserve(k);
    for (std::size_t i = 0; i < k; ++i) {
        rightHandSides.push_back(generateRandomVector(n, rng));
    }

    std::vector<SolveResult> gaussResults(k);
    row.gauss_total_ms = measureMilliseconds([&] {
        for (std::size_t i = 0; i < k; ++i) {
            gaussResults[i] = solveGaussPartialPivot(matrix, rightHandSides[i]);
        }
    });

    row.gauss_ok = true;
    row.gauss_max_residual = 0.0;
    for (std::size_t i = 0; i < k; ++i) {
        if (!gaussResults[i].success) {
            row.gauss_ok = false;
            row.gauss_max_residual = std::numeric_limits<double>::quiet_NaN();
            break;
        }

        row.gauss_max_residual = std::max(
            row.gauss_max_residual,
            residualNorm(matrix, gaussResults[i].x, rightHandSides[i]));
    }

    LUResult lu;
    row.lu_factor_ms = measureMilliseconds([&] {
        lu = luDecompose(matrix);
    });

    if (!lu.success) {
        return row;
    }

    std::vector<SolveResult> luResults(k);
    row.lu_solves_ms = measureMilliseconds([&] {
        for (std::size_t i = 0; i < k; ++i) {
            luResults[i] = solveFromLU(lu.lu, rightHandSides[i]);
        }
    });

    row.lu_ok = true;
    row.lu_max_residual = 0.0;
    for (std::size_t i = 0; i < k; ++i) {
        if (!luResults[i].success) {
            row.lu_ok = false;
            row.lu_max_residual = std::numeric_limits<double>::quiet_NaN();
            break;
        }

        row.lu_max_residual = std::max(
            row.lu_max_residual,
            residualNorm(matrix, luResults[i].x, rightHandSides[i]));
    }

    row.lu_total_ms = row.lu_factor_ms + row.lu_solves_ms;
    return row;
}

std::vector<HilbertRow> runHilbertExperiments(const std::vector<std::size_t>& sizes) {
    std::vector<HilbertRow> rows;

    for (std::size_t n : sizes) {
        const Matrix matrix = generateHilbertMatrix(n);
        const Vector expectedX(n, 1.0);
        const Vector rhs = multiply(matrix, expectedX);

        const auto collect = [&](const std::string& method, const SolveResult& result) {
            HilbertRow row;
            row.n = n;
            row.method = method;
            row.success = result.success;
            if (result.success) {
                row.relative_error = relativeError(result.x, expectedX);
                row.residual = residualNorm(matrix, result.x, rhs);
            }
            rows.push_back(row);
        };

        collect("Gauss without pivot", solveGaussNoPivot(matrix, rhs));
        collect("Gauss partial pivot", solveGaussPartialPivot(matrix, rhs));

        LUResult lu = luDecompose(matrix);
        if (lu.success) {
            collect("LU without pivot", solveFromLU(lu.lu, rhs));
        } else {
            rows.push_back({n, "LU without pivot", false,
                            std::numeric_limits<double>::quiet_NaN(),
                            std::numeric_limits<double>::quiet_NaN()});
        }
    }

    return rows;
}
