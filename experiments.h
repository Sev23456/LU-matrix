#pragma once

#include "solvers.h"

#include <cstddef>
#include <limits>
#include <random>
#include <string>
#include <vector>

struct SingleSystemRow {
    std::size_t n = 0;
    bool gauss_no_pivot_ok = false;
    bool gauss_pivot_ok = false;
    bool lu_ok = false;
    double gauss_no_pivot_ms = std::numeric_limits<double>::quiet_NaN();
    double gauss_no_pivot_residual = std::numeric_limits<double>::quiet_NaN();
    double gauss_pivot_ms = std::numeric_limits<double>::quiet_NaN();
    double gauss_pivot_residual = std::numeric_limits<double>::quiet_NaN();
    double lu_factor_ms = std::numeric_limits<double>::quiet_NaN();
    double lu_solve_ms = std::numeric_limits<double>::quiet_NaN();
    double lu_total_ms = std::numeric_limits<double>::quiet_NaN();
    double lu_residual = std::numeric_limits<double>::quiet_NaN();
};

struct MultipleRhsRow {
    std::size_t n = 0;
    std::size_t k = 0;
    bool gauss_ok = false;
    bool lu_ok = false;
    double gauss_total_ms = std::numeric_limits<double>::quiet_NaN();
    double gauss_max_residual = std::numeric_limits<double>::quiet_NaN();
    double lu_factor_ms = std::numeric_limits<double>::quiet_NaN();
    double lu_solves_ms = std::numeric_limits<double>::quiet_NaN();
    double lu_total_ms = std::numeric_limits<double>::quiet_NaN();
    double lu_max_residual = std::numeric_limits<double>::quiet_NaN();
};

struct HilbertRow {
    std::size_t n = 0;
    std::string method;
    bool success = false;
    double relative_error = std::numeric_limits<double>::quiet_NaN();
    double residual = std::numeric_limits<double>::quiet_NaN();
};

SingleSystemRow runSingleSystemExperiment(std::size_t n, std::mt19937_64& rng);
MultipleRhsRow runMultipleRhsExperiment(std::size_t n, std::size_t k, std::mt19937_64& rng);
std::vector<HilbertRow> runHilbertExperiments(const std::vector<std::size_t>& sizes);
