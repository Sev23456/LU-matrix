#pragma once

#include "linear_algebra.h"

#include <string>

struct SolveResult {
    bool success = false;
    std::string message;
    Vector x;
};

struct LUResult {
    bool success = false;
    std::string message;
    Matrix lu;
};

SolveResult backwardSubstitutionUpper(const Matrix& upper, const Vector& rhs);
SolveResult forwardSubstitutionUnitLower(const Matrix& lu, const Vector& rhs);
SolveResult solveGaussNoPivot(Matrix matrix, Vector rhs);
SolveResult solveGaussPartialPivot(Matrix matrix, Vector rhs);
LUResult luDecompose(Matrix matrix);
SolveResult solveFromLU(const Matrix& lu, const Vector& rhs);
