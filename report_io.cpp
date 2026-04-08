#include "report_io.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

std::string formatDouble(double value, int precision = 6, bool scientific = false) {
    if (std::isnan(value)) {
        return "nan";
    }

    std::ostringstream output;
    if (scientific) {
        output << std::scientific;
    } else {
        output << std::fixed;
    }
    output << std::setprecision(precision) << value;
    return output.str();
}

const char* statusText(bool ok) {
    return ok ? "OK" : "FAIL";
}

}  // namespace

void printSingleSystemTable(const std::vector<SingleSystemRow>& rows) {
    std::cout << "\nSingle system benchmark\n";
    std::cout << std::left
              << std::setw(8) << "n"
              << std::setw(12) << "G_no"
              << std::setw(16) << "G_no_ms"
              << std::setw(18) << "G_no_res"
              << std::setw(12) << "G_pivot"
              << std::setw(16) << "G_pivot_ms"
              << std::setw(18) << "G_pivot_res"
              << std::setw(14) << "LU"
              << std::setw(14) << "LU_dec_ms"
              << std::setw(14) << "LU_sol_ms"
              << std::setw(14) << "LU_tot_ms"
              << std::setw(18) << "LU_res"
              << '\n';

    for (const auto& row : rows) {
        std::cout << std::left
                  << std::setw(8) << row.n
                  << std::setw(12) << statusText(row.gauss_no_pivot_ok)
                  << std::setw(16) << formatDouble(row.gauss_no_pivot_ms, 3)
                  << std::setw(18) << formatDouble(row.gauss_no_pivot_residual, 3, true)
                  << std::setw(12) << statusText(row.gauss_pivot_ok)
                  << std::setw(16) << formatDouble(row.gauss_pivot_ms, 3)
                  << std::setw(18) << formatDouble(row.gauss_pivot_residual, 3, true)
                  << std::setw(14) << statusText(row.lu_ok)
                  << std::setw(14) << formatDouble(row.lu_factor_ms, 3)
                  << std::setw(14) << formatDouble(row.lu_solve_ms, 3)
                  << std::setw(14) << formatDouble(row.lu_total_ms, 3)
                  << std::setw(18) << formatDouble(row.lu_residual, 3, true)
                  << '\n';
    }
}

void printMultipleRhsTable(const std::vector<MultipleRhsRow>& rows) {
    std::cout << "\nMultiple RHS benchmark\n";
    std::cout << std::left
              << std::setw(8) << "n"
              << std::setw(8) << "k"
              << std::setw(12) << "Gauss"
              << std::setw(16) << "Gauss_ms"
              << std::setw(18) << "Gauss_res"
              << std::setw(10) << "LU"
              << std::setw(14) << "LU_dec_ms"
              << std::setw(16) << "LU_solve_ms"
              << std::setw(14) << "LU_tot_ms"
              << std::setw(18) << "LU_res"
              << '\n';

    for (const auto& row : rows) {
        std::cout << std::left
                  << std::setw(8) << row.n
                  << std::setw(8) << row.k
                  << std::setw(12) << statusText(row.gauss_ok)
                  << std::setw(16) << formatDouble(row.gauss_total_ms, 3)
                  << std::setw(18) << formatDouble(row.gauss_max_residual, 3, true)
                  << std::setw(10) << statusText(row.lu_ok)
                  << std::setw(14) << formatDouble(row.lu_factor_ms, 3)
                  << std::setw(16) << formatDouble(row.lu_solves_ms, 3)
                  << std::setw(14) << formatDouble(row.lu_total_ms, 3)
                  << std::setw(18) << formatDouble(row.lu_max_residual, 3, true)
                  << '\n';
    }
}

void printHilbertTable(const std::vector<HilbertRow>& rows) {
    std::cout << "\nHilbert matrix accuracy\n";
    std::cout << std::left
              << std::setw(8) << "n"
              << std::setw(24) << "method"
              << std::setw(12) << "status"
              << std::setw(20) << "rel_error"
              << std::setw(20) << "residual"
              << '\n';

    for (const auto& row : rows) {
        std::cout << std::left
                  << std::setw(8) << row.n
                  << std::setw(24) << row.method
                  << std::setw(12) << statusText(row.success)
                  << std::setw(20) << formatDouble(row.relative_error, 6, true)
                  << std::setw(20) << formatDouble(row.residual, 6, true)
                  << '\n';
    }
}

void writeSingleSystemCsv(const std::filesystem::path& path, const std::vector<SingleSystemRow>& rows) {
    std::ofstream file(path);
    file << "n,gauss_no_pivot_ok,gauss_no_pivot_ms,gauss_no_pivot_residual,"
            "gauss_pivot_ok,gauss_pivot_ms,gauss_pivot_residual,"
            "lu_ok,lu_factor_ms,lu_solve_ms,lu_total_ms,lu_residual\n";

    for (const auto& row : rows) {
        file << row.n << ','
             << row.gauss_no_pivot_ok << ','
             << row.gauss_no_pivot_ms << ','
             << row.gauss_no_pivot_residual << ','
             << row.gauss_pivot_ok << ','
             << row.gauss_pivot_ms << ','
             << row.gauss_pivot_residual << ','
             << row.lu_ok << ','
             << row.lu_factor_ms << ','
             << row.lu_solve_ms << ','
             << row.lu_total_ms << ','
             << row.lu_residual << '\n';
    }
}

void writeMultipleRhsCsv(const std::filesystem::path& path, const std::vector<MultipleRhsRow>& rows) {
    std::ofstream file(path);
    file << "n,k,gauss_ok,gauss_total_ms,gauss_max_residual,"
            "lu_ok,lu_factor_ms,lu_solves_ms,lu_total_ms,lu_max_residual\n";

    for (const auto& row : rows) {
        file << row.n << ','
             << row.k << ','
             << row.gauss_ok << ','
             << row.gauss_total_ms << ','
             << row.gauss_max_residual << ','
             << row.lu_ok << ','
             << row.lu_factor_ms << ','
             << row.lu_solves_ms << ','
             << row.lu_total_ms << ','
             << row.lu_max_residual << '\n';
    }
}

void writeHilbertCsv(const std::filesystem::path& path, const std::vector<HilbertRow>& rows) {
    std::ofstream file(path);
    file << "n,method,success,relative_error,residual\n";

    for (const auto& row : rows) {
        file << row.n << ','
             << row.method << ','
             << row.success << ','
             << row.relative_error << ','
             << row.residual << '\n';
    }
}
